#ifndef __KDB__UTIL_H__
#define __KDB__UTIL_H__

#include "multilang.h"

#include <string>

namespace q {

	// Convert error or system errors into an K error object
	K error2q(char const* error, bool isSystemError = false) throw();
	K error2q(std::string const& error, bool isSystemError = false) throw();

	K error2q(wchar_t const* error, bool isSystemError = false, UINT toCP = DEFAULT_CP) throw();
	K error2q(std::wstring const& error, bool isSystemError = false, UINT toCP = DEFAULT_CP) throw();

	K version(char const* gitId) throw();

	template <typename Char, typename Traits>
	std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, std::tm const& tm);

}//namespace q

#include "util.inl"

#ifdef NDEBUG
#	pragma comment(lib, "kdb+.util.lib")
#else
#	pragma comment(lib, "kdb+.utild.lib")
#endif

#endif//__KDB__UTIL_H__