#ifndef __MULTILANG_H__
#define __MULTILANG_H__
#pragma comment(lib, "kdb+.util.lib")

#include <string>

#ifdef _MSC_VER
//@ref https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(C4290);k(VS.ErrorList)&rd=true
#pragma warning(disable: 4290)
#endif

namespace q {

	// Default code page used by kdb+
	extern UINT const DEFAULT_CP;

	std::string convert(UINT frCP, UINT toCP, char const* str) throw(std::runtime_error);
	std::string convert(UINT frCP, UINT toCP, std::string const& str) throw(std::runtime_error);

	K convert(UINT frCP, UINT toCP, K str) throw();

}//namespace q

namespace ml {

	extern UINT const CP_GB18030;

	// Convert inStr from code page frCP to UTF-16
	std::wstring convert(UINT frCP, char const* inStr) throw(std::runtime_error);

	// Convert inUcs from UTF-16 to code page toCP 
	std::string  convert(UINT toCP, wchar_t const* inUcs) throw(std::runtime_error);

	// Convert inStr from code page frCP to code page toCP
	std::string  convert(UINT frCP, UINT toCP, char const* inStr) throw(std::runtime_error);

}//namespace ml

#endif//__MULTILANG_H__