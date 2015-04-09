#ifndef __WIN32_UTIL__UTIL_H__
static_assert(0, "Include win32.util/util.h instead!");
#endif

#include <iomanip>

template <typename It>
struct util::hexByteGenerator {
	It begin_, end_;
	bool reverse_;
	hexByteGenerator(It begin, It end, bool reverse = false)
		: begin_(begin), end_(end), reverse_(reverse) {}
};

template <typename It>
util::hexByteGenerator<It> util::hexBytes(It begin, It end) {
	return hexByteGenerator<It>(begin, end);
}

template <typename T>
util::hexByteGenerator<char const*> util::hexBytes(T const& x) {
	return hexBytes(reinterpret_cast<char const*>(&x), sizeof(x), true);
}

template <typename T>
util::hexByteGenerator<T const*> util::hexBytes(T const* p, std::size_t n, bool reverse) {
	return hexByteGenerator<T const*>(p, p + n, reverse);
}

template <typename Char, typename Traits, typename It>
std::basic_ostream<Char, Traits>& util::operator<<(std::basic_ostream<Char, Traits>& os, util::hexByteGenerator<It> const& hex) {
	if (hex.reverse_)
		for (It p = hex.end_; p != hex.begin_; --p) {
			os << std::setiosflags(std::ios::uppercase) << std::setfill('0') << std::setw(2)
				<< std::hex << static_cast<int>(*(p - 1));
		}
	else
		for (It p = hex.begin_; p != hex.end_; ++p) {
			os << std::setiosflags(std::ios::uppercase) << std::setfill('0') << std::setw(2)
				<< std::hex << static_cast<int>(*p);
		}
	return os;
}