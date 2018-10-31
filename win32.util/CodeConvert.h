#ifndef __CODECONVERT_H__
#define __CODECONVERT_H__

#include <string>
#include <stdexcept>

#ifdef _MSC_VER
//@see https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(C4290);k(VS.ErrorList)&rd=true
#pragma warning(disable: 4290)
#endif

namespace ml {

	extern UINT const CP_GB18030;

	// Convert inStr from code page frCP to UTF-16
	std::wstring convert(UINT frCP, char const* inStr) throw(std::runtime_error);

	// Convert inUcs from UTF-16 to code page toCP 
	std::string  convert(UINT toCP, wchar_t const* inUcs) throw(std::runtime_error);

	// Convert inStr from code page frCP to code page toCP
	std::string  convert(UINT frCP, UINT toCP, char const* inStr) throw(std::runtime_error);

}//namespace ml

#ifdef NDEBUG
#	pragma comment(lib, "win32.util.lib")
#else
#	pragma comment(lib, "win32.utild.lib")
#endif

#endif//__CODECONVERT_H__