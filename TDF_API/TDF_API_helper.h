#ifndef __TDF_API_HELPER_H__
#define __TDF_API_HELPER_H__

#include <string>

#ifdef _MSC_VER
//@ref https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(C4290);k(VS.ErrorList)&rd=true
#pragma warning(disable: 4290)
#endif

namespace TDF {

	void parseTdfHandle(K h, ::THANDLE& tdf) throw(std::string);

}//naemspace TDF

#endif//__TDF_API_HELPER_H__