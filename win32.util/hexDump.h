#ifndef __HEXDUMP_H__
#define __HEXDUMP_H__
#pragma comment(lib, "win32.util.lib")

#include "msvc.h"

#include <iosfwd>

namespace util {

	struct hexByteGenerator;

	template <typename Char, typename Traits>
	std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, hexByteGenerator const& hex);


	struct hexByteGenerator {
		typedef unsigned char const* iterator;
		hexByteGenerator(iterator begin, iterator end, bool reverse)
			: begin_(begin), end_(end), reverse_(reverse) {}
	private:
		iterator const begin_, end_;
		bool const reverse_;

		template <typename Char, typename Traits>
		friend std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, hexByteGenerator const& hex);
	};

	hexByteGenerator hexBytes(char const* p, size_t n, bool reverse = false);
	hexByteGenerator hexBytes(unsigned char const* p, size_t n, bool reverse = false);
	template <typename T>
	hexByteGenerator hexBytes(T* const& p);
	template <typename T>
	hexByteGenerator hexBytes(T const& x);

}//namespace util

#include "hexDump.inl"

#endif//__HEXDUMP_H__