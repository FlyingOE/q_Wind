#ifndef __TDB_API_HELPER_H__
#define __TDB_API_HELPER_H__

#include "kdb+.util/type_traits.h"

#include <stdexcept>
#include <string>
#include <vector>

#ifdef _MSC_VER
//@see https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(C4290);k(VS.ErrorList)&rd=true
#pragma warning(disable: 4290)
#endif

namespace TDB {

	void parseTdbHandle(K h, ::THANDLE& tdb) throw(std::runtime_error);

	std::string getMarketId(::THANDLE tdb, char const* market) throw(std::runtime_error);
	std::string getMarketId(::THANDLE tdb, std::string const& market) throw(std::runtime_error);

	template <typename FieldTraits>
	void parseIndicators(K indicators,
		std::vector<typename FieldTraits::field_accessor const*>& indis) throw(std::runtime_error);

	template <typename TdbReq>
	void parseTdbReqCode(::THANDLE const tdb, K windCode, TdbReq& req) throw(std::runtime_error);
	template <typename TdbReq>
	void parseTdbReqTime(K beginDT, K endDT, TdbReq& req) throw(std::runtime_error);
	template <typename TdbReq>
	void parseTdbReqTime(K date, K begin, K end, TdbReq& req) throw(std::runtime_error);

	template <typename FieldTraits, typename TdbReq>
	K runQuery(::THANDLE tdb, TdbReq const& req,
		std::vector<typename FieldTraits::field_accessor const*> const& indis,
		int(*tdbCall)(::THANDLE, TdbReq const*, typename FieldTraits::tdb_result_type**, int*));

	template <typename T,typename QType> struct MarketInfoAccessor;
	template <typename T> using MarketIdAccessor = MarketInfoAccessor<T, S>;

}//namespace TDB

#include "TDB_API_helper.inl"

#endif//__TDB_API_HELPER_H__