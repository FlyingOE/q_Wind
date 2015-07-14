#ifndef __TDB_API_HELPER_H__
#define __TDB_API_HELPER_H__

#include "kdb+.util/type_traits.h"

#include <string>
#include <vector>

#ifdef _MSC_VER
//@ref https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(C4290);k(VS.ErrorList)&rd=true
#pragma warning(disable: 4290)
#endif

namespace TDB {

	void parseTdbHandle(K h, ::THANDLE& tdb) throw(std::string);

	template <typename Traits>
	void parseIndicators(K indicators,
		std::vector<typename Traits::field_accessor_type const*>& indis) throw(std::string);

	template <typename TdbReq>
	void parseTdbReq(K windCode, K begin, K end, TdbReq& req) throw(std::string);

	template <typename Traits>
	K getFields();

	template <typename Traits, typename TdbReq>
	K runQuery(::THANDLE tdb, TdbReq const& req,
		std::vector<typename Traits::field_accessor_type const*>& indis,
		int(*tdbCall)(::THANDLE, TdbReq const*, typename Traits::tdb_result_type**, int*));

}//namespace TDB

#include "TDB_API_helper.inl"

#endif//__TDB_API_HELPER_H__