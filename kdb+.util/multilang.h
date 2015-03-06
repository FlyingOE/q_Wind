#ifndef __MULTILANG_H__
#define __MULTILANG_H__
#pragma comment(lib, "kdb+.util.lib")

#include <string>

namespace ml {

	extern UINT const CP_GB18030;

	// Convert inStr from code page frCP to UTF-16
	std::wstring convert(UINT frCP, char const* inStr);

	// Convert inUcs from UTF-16 to code page toCP 
	std::string  convert(UINT toCP, wchar_t const* inUcs);

	// Convert inStr from code page frCP to code page toCP
	std::string  convert(UINT frCP, UINT toCP, char const* inStr);

}//namespace ml

namespace q {

	std::string convert(UINT frCP, UINT toCP, char const* str);
	std::string convert(UINT frCP, UINT toCP, std::string const& str);

	K convert(UINT frCP, UINT toCP, K str) throw();

}//namespace q

#endif//__MULTILANG_H__