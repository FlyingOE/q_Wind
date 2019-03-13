#include "stdafx.h"
#include "kdb+.h"

#include "kdb+.util/util.h"
#include "kdb+.util/K_ptr.h"
#include "kdb+.util/type_convert.h"
#include <cassert>
#include <vector>
#include <sstream>

#include <zlib.h>
#include <zconf.h>
#pragma comment(lib, "zlibwapi.lib")
static_assert(sizeof(::uInt) <= sizeof(J), "type oversize: uInt vs J");
static_assert(sizeof(::Bytef) == sizeof(G), "type mismatch: Bytef vs G");

namespace {

	char const* zlibError(int errCode) {
		switch (errCode) {
#		define ZLIB_ERROR(x)  case (x): return (#x)
			ZLIB_ERROR(Z_OK);
			ZLIB_ERROR(Z_STREAM_END);
			ZLIB_ERROR(Z_NEED_DICT);
			ZLIB_ERROR(Z_ERRNO);
			ZLIB_ERROR(Z_STREAM_ERROR);
			ZLIB_ERROR(Z_DATA_ERROR);
			ZLIB_ERROR(Z_MEM_ERROR);
			ZLIB_ERROR(Z_BUF_ERROR);
			ZLIB_ERROR(Z_VERSION_ERROR);
#		undef ZLIB_ERROR
		default:
			return "<BUG> unexpected ZLIB error";
		}
	}

	std::string zlibError(int errCode, char const* errMsg) {
		std::ostringstream buffer;
		buffer << zlibError(errCode);
		if (NULL != errMsg) {
			buffer << " (" << errMsg << ')';
		}
		return buffer.str();
	}

	size_t q2BlockSize(K blockSize) throw(std::runtime_error) {
		long long const chunkSize = q::q2Dec(blockSize);
		if (chunkSize < 4 * 1024) {
			throw std::runtime_error("blockSize too small (min 2^12)");
		}
		else if (chunkSize > 1024 * 1024) {
			throw std::runtime_error("blockSize too large (max 2^20)");
		}
		else {
			return static_cast<size_t>(chunkSize);
		}
	}

	int q2Level(K level) throw(std::runtime_error) {
		long long const lvl = q::q2Dec(level);
		if ((0 > lvl) || (lvl > 9)) {
			throw std::runtime_error("level must be within [0, 9]");
		}
		else {
			return static_cast<int>(lvl);
		}
	}

	Bytef* q2Bytes(K bytes, size_t& length) throw(std::runtime_error) {
		if (bytes == K_NIL) {
			length = 0;
			return Z_NULL;
		}
		switch (bytes->t) {
		case KG:
		case KC:
			assert((0 <= bytes->n) && (bytes->n <= std::numeric_limits<size_t>::max()));
			length = static_cast<size_t>(bytes->n);
			return kG(bytes);
		default:
			throw std::runtime_error("not bytes or chars");
		}
	}

}//namespace /*anonymous*/

//@see https://www.zlib.net/zlib_how.html
KDB_API K K_DECL gzipCompress(K blockSize, K cprzLevel, K bytes) {
	// Input sanity check
	Bytef* input;
	size_t length, chunkSize;
	int level;
	try {
		chunkSize = q2BlockSize(blockSize);
		level = q2Level(cprzLevel);
		input = q2Bytes(bytes, length);
	}
	catch (std::runtime_error const& ex) {
		return q::error2q(ex.what());
	}
	if (length > std::numeric_limits<uInt>::max()) {
		return q::error2q("input stream too long (zlib)");
	}
	assert(!!input);

	// Initialize zlib for deflate
	::z_stream stream;
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.avail_in = 0;
	stream.next_in = Z_NULL;
	int status = ::deflateInit2(&stream, level, Z_DEFLATED, 16 + MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
	if (Z_OK != status) {
		return q::error2q(zlibError(status, stream.msg));
	}

	// Deflate chunk by chunk
	std::vector<G> chunk(chunkSize, '\0');
	std::vector<G> output;
	stream.avail_in = static_cast<uInt>(length);	//already sanity checked
	stream.next_in = kG(bytes);
	do {
		do {
			stream.avail_out = chunkSize;
			stream.next_out = &chunk[0];
			status = ::deflate(&stream, (stream.avail_in == 0) ? Z_FINISH : Z_NO_FLUSH);
			assert(status != Z_STREAM_ERROR);	//zlib state not clobbered
			switch (status) {
			case Z_STREAM_ERROR:
				return q::error2q("Z_STREAM_ERROR: zlib state clobbered!");
			case Z_NEED_DICT:
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				::deflateEnd(&stream);
				return q::error2q(zlibError(status, stream.msg));
			default:
				;//no-op
			}
			size_t const outSize = chunkSize - stream.avail_out;
			size_t const prevSize = output.size();
			assert(stream.total_out == prevSize + outSize);
			output.resize(stream.total_out);
			std::copy(chunk.cbegin(), chunk.cbegin() + outSize, output.begin() + prevSize);
		} while (stream.avail_out == 0);
	} while (status != Z_STREAM_END);
	::deflateEnd(&stream);
	chunk.clear();
	
	q::K_ptr result(ktn(bytes->t, output.size()));
	assert(!!result);
	std::copy(output.cbegin(), output.cend(), kG(result.get()));
	return result.release();
}

//@see https://www.zlib.net/zlib_how.html
//@see http://windrealm.org/tutorials/decompress-gzip-stream.php
//@see https://github.com/lmartinking/qgz
KDB_API K K_DECL gzipDecompress(K blockSize, K isRaw, K bytes) {
	// Input sanity check
	Bytef* input;
	size_t length, chunkSize;
	bool raw;
	try {
		chunkSize = q2BlockSize(blockSize);
		raw = !!q::q2Dec(isRaw);
		input = q2Bytes(bytes, length);
	}
	catch (std::runtime_error const& ex) {
		return q::error2q(ex.what());
	}
	if (length < 10 + 8) {
		return q::error2q("compressed stream too short");
	}
	else if (length > std::numeric_limits<uInt>::max()) {
		return q::error2q("compressed stream too long (zlib)");
	}
	assert(!!input);

	// Initialize zlib for inflate
	::z_stream stream;
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.avail_in = 0;
	stream.next_in = Z_NULL;
	int status = ::inflateInit2(&stream, raw ? (-MAX_WBITS) : (16 + MAX_WBITS));
	if (Z_OK != status) {
		return q::error2q(zlibError(status, stream.msg));
	}

	// Inflate chunk by chunk
	std::vector<G> chunk(chunkSize, '\0');
	std::vector<G> output;
	stream.avail_in = static_cast<uInt>(length);	//already sanity checked
	stream.next_in = kG(bytes);
	do {
		do {
			stream.avail_out = chunkSize;
			stream.next_out = &chunk[0];
			status = ::inflate(&stream, Z_NO_FLUSH);
			assert(status != Z_STREAM_ERROR);	//zlib state not clobbered
			switch (status) {
			case Z_STREAM_ERROR:
				return q::error2q("Z_STREAM_ERROR: zlib state clobbered!");
			case Z_NEED_DICT:
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				::inflateEnd(&stream);
				return q::error2q(zlibError(status, stream.msg));
			default:
				;//no-op
			}
			size_t const outSize = chunkSize - stream.avail_out;
			size_t const prevSize = output.size();
			assert(stream.total_out == prevSize + outSize);
			output.resize(stream.total_out);
			std::copy(chunk.cbegin(), chunk.cbegin() + outSize, output.begin() + prevSize);
		} while (stream.avail_out == 0);
	} while (status != Z_STREAM_END);
	::inflateEnd(&stream);
	chunk.clear();
	
	q::K_ptr result(ktn(bytes->t, output.size()));
	assert(!!result);
	std::copy(output.cbegin(), output.cend(), kG(result.get()));
	return result.release();
}