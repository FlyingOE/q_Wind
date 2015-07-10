#include "stdafx.h"
#include "TDB_API.h"

#include "TDB_API_helper.h"

#include "kdb+.util/util.h"
#include "kdb+.util/K_ptr.h"
#include "kdb+.util/type_traits.h"
#include "kdb+.util/type_convert.h"

#include <cassert>
#include <string>

::OPEN_SETTINGS TDB::SETTINGS = {
	"localhost",	//host
	"10222",		//port
	"user",			//userName
	"password",		//password
	30,				//timeout (sec)
	2,				//retries
	0				//retry delay
};

#ifdef _MSC_VER
//@ref https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(C4290);k(VS.ErrorList)&rd=true
#pragma warning(disable: 4290)
#endif

namespace TDB {
	namespace util {

		// Helper for ::setTimeout(...)
		template <typename T>
		T& setSetting(K data, T& setting) throw(std::string) {
			if (data == K_NIL) return setting;

			long long nullValue;
			switch (data->t) {
			case -KH:
				nullValue = q::type_traits<H>::NULL_VAL;
				break;
			case -KI:
				nullValue = q::type_traits<I>::NULL_VAL;
				break;
			case -KJ:
				nullValue = q::type_traits<J>::NULL_VAL;
				break;
			default:
				throw std::string("not an integral value");
			}

			long long value = q::q2Dec(data);
			if (value == nullValue) {
				return setting;
			}
			else if ((value < std::numeric_limits<T>::min()) || (std::numeric_limits<T>::max() < value)) {
				throw std::string("out of bounds");
			}
			else {
				return setting = static_cast<T>(value);
			}
		}

	}//namespace util
}//namespace TDB


TDB_API K K_DECL getTimeout(K _) {
	return knk(3, ki(TDB::SETTINGS.nTimeOutVal), ki(TDB::SETTINGS.nRetryCount), ki(TDB::SETTINGS.nRetryGap));
}

TDB_API K K_DECL setTimeout(K timeout, K retries, K delay) {
	try {
		TDB::util::setSetting(timeout, TDB::SETTINGS.nTimeOutVal);
		TDB::util::setSetting(retries, TDB::SETTINGS.nRetryCount);
		TDB::util::setSetting(delay, TDB::SETTINGS.nRetryGap);
	}
	catch (std::string const &error) {
		return q::error2q(error);
	}
	return getTimeout(K_NIL);
}

TDB_API K K_DECL TDB_codeTable(K h, K market) {
	::THANDLE tdb = NULL;
	std::string mkt;
	try {
		tdb = reinterpret_cast<::THANDLE>(q::q2Dec(h));
		mkt = q::q2String(market);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}
	if (!tdb) {
		return q::error2q("null THANDLE");
	}

	int codeCount = 0;
	::TDBDefine_Code* t = NULL;
	int const result = ::TDB_GetCodeTable(tdb, mkt.c_str(), &t, &codeCount);
	TDB::Ptr<::TDBDefine_Code> codes(t);
	if (result != TDB_SUCCESS) {
		return q::error2q(TDB::getError(result));
	}
	assert(codeCount >= 0);

	q::K_ptr data(ktn(0, 6));
	typedef TDB::SymbolAccessor<::TDBDefine_Code, char[32]> SymbolAccessor_;
	kK(data.get())[0] =		//万得代码(AG1312.SHF)
		SymbolAccessor_(&::TDBDefine_Code::chWindCode).extract(codes.get(), codeCount);
	kK(data.get())[1] =		//交易所代码(ag1312)
		SymbolAccessor_(&::TDBDefine_Code::chCode).extract(codes.get(), codeCount);
	kK(data.get())[2] =		//市场代码(SHF)
		TDB::SymbolAccessor<::TDBDefine_Code, char[8]>(&::TDBDefine_Code::chMarket).extract(codes.get(), codeCount);
	kK(data.get())[3] =		//证券中文名称
		TDB::SymbolAccessor<::TDBDefine_Code, char[32], TDB::util::GB18030Encoder>(
					&::TDBDefine_Code::chCNName).extract(codes.get(), codeCount);
	kK(data.get())[4] =		//证券英文名称
		SymbolAccessor_(&::TDBDefine_Code::chENName).extract(codes.get(), codeCount);
	kK(data.get())[5] =		//证券类型
		TDB::IntAccessor<::TDBDefine_Code, G>(&::TDBDefine_Code::nType).extract(codes.get(), codeCount);
	return data.release();
}