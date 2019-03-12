#include "stdafx.h"
#include "kdb+.h"

#include "kdb+.util/util.h"
#include "kdb+.util/K_ptr.h"
#include <cassert>
#include <vector>
#include <sstream>

#include <zlib.h>
#include <zconf.h>
#pragma comment(lib, "zlibwapi.lib")

namespace {

	size_t const ZLIB_CHUNK_SIZE = 128 * 1024;	//128K

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

}//namespace /*anonymous*/

//@see https://www.zlib.net/zlib_how.html
//@see http://windrealm.org/tutorials/decompress-gzip-stream.php
//@see https://github.com/lmartinking/qgz
KDB_API K K_DECL decompress(K bytes) {
	static_assert(sizeof(::uInt) <= sizeof(J), "type oversize: uInt vs J");
	static_assert(sizeof(::Bytef) == sizeof(G), "type mismatch: Bytef vs G");

	// Input sanity check
	if (bytes == K_NIL) {
		return K_NIL;
	}
	switch (bytes->t) {
	case KG:
	case KC:
		assert(bytes->n >= 0);
		if (bytes->n < 10 + 8) {
			return q::error2q("compressed stream too short");
		}
		else if (bytes->n > std::numeric_limits<uInt>::max()) {
			return q::error2q("compressed stream too long (zlib)");
		}
		break;
	default:
		return q::error2q("not bytes or chars");
	}

	// Initialize zlib for inflate
	::z_stream stream;
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.avail_in = 0;
	stream.next_in = Z_NULL;
	int status = ::inflateInit2(&stream, 16 + MAX_WBITS);
	if (Z_OK != status) {
		return q::error2q(zlibError(status, stream.msg));
	}

	// Inflate chunk by chunk
	std::vector<G> chunk(ZLIB_CHUNK_SIZE, '\0');
	std::vector<G> output;
	stream.avail_in = static_cast<uInt>(bytes->n);	//already sanity checked
	stream.next_in = kG(bytes);
	do {
		do {
			stream.avail_out = ZLIB_CHUNK_SIZE;
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
			}
			size_t const outSize = ZLIB_CHUNK_SIZE - stream.avail_out;
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