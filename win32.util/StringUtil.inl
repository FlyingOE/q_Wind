#ifndef __STRINGUTIL_H__
static_assert(0, "Include win32.util/StringUtil.h instead!");
#endif

#include <sstream>

template <typename Char, typename Traits, typename Alloc>
std::basic_string<Char, Traits, Alloc>& util::replaceAll(std::basic_string<Char, Traits, Alloc>& str, Char const* probe, Char const* repl) {
	return replaceAll(str, std::basic_string<Char, Traits, Alloc>(probe), std::basic_string<Char, Traits, Alloc>(repl));
}

//@ref http://stackoverflow.com/questions/1494399/how-do-i-search-find-and-replace-in-a-standard-string
template <typename Char, typename Traits, typename Alloc>
std::basic_string<Char, Traits, Alloc>& util::replaceAll(
	std::basic_string<Char, Traits, Alloc>& str,
	std::basic_string<Char, Traits, Alloc> const& probe, std::basic_string<Char, Traits, Alloc> const& repl)
{
	typename std::basic_string<Char, Traits, Alloc>::size_type pos = 0;
	while ((pos = str.find(probe, pos)) != std::string::npos) {
		str.replace(pos, probe.length(), repl);
		pos += repl.length();
	}
	return str;
}

//@ref http://stackoverflow.com/questions/236129/split-a-string-in-c
template <typename Char, typename Traits, typename Alloc>
std::vector<std::basic_string<Char, Traits, Alloc> > util::split(
	std::basic_string<Char, Traits, Alloc> const&str, Char delim)
{
	std::vector<std::basic_string<Char, Traits, Alloc> > tokens;
	std::basic_string<Char, Traits, Alloc> token;
	std::basic_stringstream<Char, Traits, Alloc> buffer(str);
	while (std::getline(buffer, token, delim)) {
		tokens.push_back(token);
	}
	return tokens;
}

template <typename Char>
std::vector<std::basic_string<Char> > util::split(Char const* str, Char delim)
{
	return split(std::basic_string<Char>(str), delim);
}