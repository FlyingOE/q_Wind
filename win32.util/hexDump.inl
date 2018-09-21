#ifndef __HEXDUMP_H__
static_assert(0, "Include win32.util/hexDump.h instead!");
#endif

#include "StringUtil.h"

template <typename T>
util::hexByteGenerator util::hexBytes(T* const& p) {
	return hexBytes(reinterpret_cast<hexByteGenerator::iterator>(&p), sizeof(p), true);
}

template <typename T>
util::hexByteGenerator util::hexBytes(T const& p) {
	return hexBytes(reinterpret_cast<hexByteGenerator::iterator>(&p), sizeof(p), true);
}

//@see http://stackoverflow.com/questions/10599068/how-do-i-print-bytes-as-hexadecimal
template <typename Char, typename Traits>
std::basic_ostream<Char, Traits>& util::operator<<(std::basic_ostream<Char, Traits>& os, util::hexByteGenerator const& hex) {
	hexByteGenerator::iterator begin, end;
	size_t increment;
	if (hex.reverse_) {
		begin = hex.end_ - 1;
		end = hex.begin_ - 1;
		increment = -1;
	}
	else {
		begin = hex.begin_;
		end = hex.end_;
		increment = 1;
	}

	static Char const* nibble2hex = LITERAL(Char, "0123456789ABCDEF");
	for (auto p = begin; p != end; p += increment) {
		os << nibble2hex[*p >> 4] << nibble2hex[*p & 0x0F];
	}
	return os;
}