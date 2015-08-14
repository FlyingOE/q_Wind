#ifndef __STRINGUTIL_H__
#define __STRINGUTIL_H__

namespace util {

	template <typename Char, typename Traits, typename Alloc>
	std::basic_string<Char, Traits, Alloc>& replaceAll(std::basic_string<Char, Traits, Alloc>& str,
		Char const* probe, Char const* repl);

	template <typename Char, typename Traits, typename Alloc>
	std::basic_string<Char, Traits, Alloc>& replaceAll(std::basic_string<Char, Traits, Alloc>& str,
		std::basic_string<Char, Traits, Alloc> const& probe, std::basic_string<Char, Traits, Alloc> const& repl);

}//namespace util

#include "StringUtil.inl"

#endif//__STRINGUTIL_H__