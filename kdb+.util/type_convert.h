#ifndef __TYPE_CONVERT_H__
#define __TYPE_CONVERT_H__
#pragma comment(lib, "kdb+.util.lib")

#include "multilang.h"

#include <string>
#include <vector>
#include <map>
#include <OAIdl.h>	// ATL: DATE

#ifdef _MSC_VER
//@see https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(C4290);k(VS.ErrorList)&rd=true
#pragma warning(disable: 4290)
#endif

namespace q {

	struct tm_ext : std::tm {
		int tm_millis;
	};

	// Convert a decimal number in q to a C++ integer
	long long q2Dec(K data) throw(std::runtime_error);
	std::vector<long long> qList2Dec(K data) throw(std::runtime_error);

	// Convert a floating-point number in q to a C++ floating-point
	double q2Fp(K data) throw(std::runtime_error);
	std::vector<double> qList2Fp(K data) throw(std::runtime_error);

	// Convert a symbol or char list in q into a C++ string
	std::string q2String(K data) throw(std::runtime_error);
	std::vector<std::string> qList2String(K data) throw(std::runtime_error);

	std::wstring q2WString(K data, UINT frCP = DEFAULT_CP) throw(std::runtime_error);
	std::vector<std::wstring> qList2WString(K data, UINT frCP = DEFAULT_CP) throw(std::runtime_error);

	// Convert a dict or symbols or char lists in q into a C++ map
	std::map<std::string, std::string> qDict2StringMap(K data) throw(std::runtime_error);
	std::map<std::wstring, std::wstring> qDict2WStringMap(K data, UINT frCP = DEFAULT_CP) throw(std::runtime_error);

	// Convert a date or time or datetime in q into a C++ std::tm
	tm_ext q2tm(K data) throw(std::runtime_error);
	std::vector<tm_ext> qList2tm(K data) throw(std::runtime_error);

	// Convert a C++ string vector into q symbol list
	K String2qList(std::vector<std::string> const& list);

	// Convert anything to Windows VARIANT type
	::VARIANT q2Variant(K data) throw(std::runtime_error);
	// Convert Windows VARIANT type to q type
	K Variant2q(::VARIANT const& data) throw();

	// Convert a C++ date string to a q date
	I date2q(int yyyymmdd);
	I date2q(char const* dateStr) throw(std::runtime_error);
	I date2q(std::string const& dateStr) throw(std::runtime_error);

	// Convert an ATL DATE value to a q datetime
	F DATE2q(::DATE date) throw(std::runtime_error);
	// Convert a q datetime to an ATL DATE value
	::DATE q2DATE(K data) throw(std::runtime_error);

	// Convert a C++ time string to a q time
	I time2q(int hhmmssfff);

}//namespace q

#include "Cookbook.inl"

#endif//__TYPE_CONVERT_H__