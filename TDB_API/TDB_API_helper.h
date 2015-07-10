#ifndef __TDB_API_HELPER_H__
#define __TDB_API_HELPER_H__

#include <string>
#include <vector>

#ifdef _MSC_VER
//@ref https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(C4290);k(VS.ErrorList)&rd=true
#pragma warning(disable: 4290)
#endif

namespace TDB {

	void parseTdbHandle(K h, ::THANDLE& tdb) throw(std::string);

	template <typename TDBDefine_Wrap>
	void parseIndicators(K indicators,
		std::vector<typename TDBDefine_Wrap::field_accessor_type const*>& indis) throw(std::string);

	template <typename TDBDefine_ReqT>
	void parseTdbReq(K windCode, K begin, K end, TDBDefine_ReqT& req) throw(std::string);

	template <typename TDBDefine_Wrap>
	K getFields();

	template <typename TDBDefine_Wrap, typename TDBDefine_ReqT>
	K runQuery(::THANDLE tdb, TDBDefine_ReqT const& req,
		std::vector<typename TDBDefine_Wrap::field_accessor_type const*>& indis,
		int(*tdbCall)(::THANDLE, TDBDefine_ReqT const*, typename TDBDefine_Wrap::tdb_result_type**, int*));

}//namespace TDB

#include "TDB_API_helper.inl"

#endif//__TDB_API_HELPER_H__