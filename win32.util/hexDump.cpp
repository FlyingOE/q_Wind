#include "stdafx.h"
#include "hexDump.h"

util::hexByteGenerator util::hexBytes(char const* p, size_t n, bool reverse) {
	return hexBytes(reinterpret_cast<unsigned char const*>(p), n, reverse);
}

util::hexByteGenerator util::hexBytes(unsigned char const* p, size_t n, bool reverse) {
	return hexByteGenerator(p, p + n, reverse);
}