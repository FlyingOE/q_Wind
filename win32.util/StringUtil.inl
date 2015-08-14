#ifndef __STRINGUTIL_H__
static_assert(0, "Include win32.util/StringUtil.h instead!");
#endif

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
