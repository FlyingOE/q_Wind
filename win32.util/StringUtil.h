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

	//@ref http://stackoverflow.com/questions/4261673/templates-and-string-literals-and-unicode
#	define LITERAL(Char, x) ::util::literal_traits<Char>::choose(x, L##x)
	template <typename Char>
	struct literal_traits {
		typedef Char char_type;
		static char_type choose(char narrow, wchar_t wide);
		static char_type const* choose(char const* narrow, wchar_t const* wide);
	};

}//namespace util

#include "StringUtil.inl"

#endif//__STRINGUTIL_H__