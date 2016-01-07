#ifndef __STRINGUTIL_H__
#define __STRINGUTIL_H__

#include <vector>

namespace util {

	template <typename Char, typename Traits, typename Alloc>
	std::basic_string<Char, Traits, Alloc>& replaceAll(std::basic_string<Char, Traits, Alloc>& str,
		Char const* probe, Char const* repl);

	template <typename Char, typename Traits, typename Alloc>
	std::basic_string<Char, Traits, Alloc>& replaceAll(std::basic_string<Char, Traits, Alloc>& str,
		std::basic_string<Char, Traits, Alloc> const& probe, std::basic_string<Char, Traits, Alloc> const& repl);

	template <typename Char, typename Traits, typename Alloc>
	std::vector<std::basic_string<Char, Traits, Alloc> > split(
		std::basic_string<Char, Traits, Alloc> const& str, Char delim);

	template <typename Char>
	std::vector<std::basic_string<Char> > split(Char const* str, Char delim);

}//namespace util

#include "StringUtil.inl"

#endif//__STRINGUTIL_H__