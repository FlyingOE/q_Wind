#ifndef __MULTILANG_H__
#define __MULTILANG_H__
#pragma comment(lib, "kdb+.util.lib")

#include <string>

#ifdef _MSC_VER
//@see https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(C4290);k(VS.ErrorList)&rd=true
#pragma warning(disable: 4290)
#endif

namespace q {

	// Default code page used by kdb+
	extern UINT const DEFAULT_CP;

	std::string convert(UINT frCP, UINT toCP, char const* str) throw(std::runtime_error);
	std::string convert(UINT frCP, UINT toCP, std::string const& str) throw(std::runtime_error);

	K convert(UINT frCP, UINT toCP, K str) throw();

}//namespace q

#endif//__MULTILANG_H__