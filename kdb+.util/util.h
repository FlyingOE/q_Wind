#ifndef __KDB__UTIL_H__
#define __KDB__UTIL_H__
#pragma comment(lib, "kdb+.util.lib")
#pragma comment(lib, "q.lib")

#include <string>

#define K_NIL	(static_cast<K>(0))

namespace q {

	// Default code page used by kdb+
	extern UINT const DEFAULT_CP;

	// Convert error or system errors into an K error object
	K error2q(char const* error, bool isSystemError = false) throw();
	K error2q(std::string const& error, bool isSystemError = false) throw();

	K error2q(wchar_t const* error, bool isSystemError = false, UINT toCP = DEFAULT_CP) throw();
	K error2q(std::wstring const& error, bool isSystemError = false, UINT toCP = DEFAULT_CP) throw();

	template <typename Char, typename Traits>
	std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, std::tm const& tm);

}//namespace q

#include "util.inl"

#endif//__KDB__UTIL_H__