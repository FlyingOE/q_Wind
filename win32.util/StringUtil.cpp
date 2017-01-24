#include "stdafx.h"
#include "StringUtil.h"

template <>
char util::literal_traits<char>::choose(char narrow, wchar_t wide) {
	return narrow;
}

template <>
char const* util::literal_traits<char>::choose(char const* narrow, wchar_t const* wide) {
	return narrow;
}

template <>
wchar_t util::literal_traits<wchar_t>::choose(char narrow, wchar_t wide) {
	return wide;
}

template <>
wchar_t const* util::literal_traits<wchar_t>::choose(char const* narrow, wchar_t const* wide) {
	return wide;
}