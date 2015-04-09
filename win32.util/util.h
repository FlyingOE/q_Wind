#ifndef __WIN32_UTIL__UTIL_H__
#define __WIN32_UTIL__UTIL_H__

#include <iosfwd>

namespace util {

	template <typename It>
	struct hexByteGenerator;
	template <typename It>
	hexByteGenerator<It> hexBytes(It begin, It end);
	template <typename T>
	hexByteGenerator<char const*> hexBytes(T const& x);
	template <typename T>
	hexByteGenerator<T const*> hexBytes(T const* p, std::size_t n, bool reverse = false);
	template <typename Char, typename Traits, typename It>
	std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, hexByteGenerator<It> const& hex);

}//namespace util

#include "util.inl"

#endif//__WIN32_UTIL__UTIL_H__