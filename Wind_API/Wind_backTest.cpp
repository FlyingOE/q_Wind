#include "stdafx.h"
#include "Wind_API.h"

//////////////////////////////////////////////////////////////////////////////
// Accordingly to Wind, BKT is not recommended (buggy, not maintained)...
//////////////////////////////////////////////////////////////////////////////

/*
#include "util.h"
#include "Wind_strike.h"

#include "kdb+.util/type_convert.h"
#include "Wind.util/EnumUtil.h"

#include <sstream>

namespace Wind {
	namespace backtest {

		enum Side {
			Buy,	// 证券买入/期货买入开仓
			Sell,	// 证券卖出/期货卖出平仓
			Short,	// 期货卖出开仓
			Cover	// 期货买入平仓
		};

	}//namespace Wind::backtest
}//namespace Wind

BEGIN_ENUM_STRING(Wind::backtest::Side)
{

}
END_ENUM_STRING;

WIND_API K K_DECL Wind_bktStart(K strategyName, K beginDate, K endDate, K params) {
	std::wstring strategy, begin, end, paras;
	try {
		strategy = q::q2WString(strategyName);
		begin = Wind::util::q2DateStr(beginDate);
		end = Wind::util::q2DateStr(endDate);
		paras = Wind::util::qDict2WStringMapJoin(params, L';', L'=');
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	Wind::callback::Result result;
	::WQID const qid = ::BktStart(strategy.c_str(), begin.c_str(), end.c_str(), paras.c_str(),
		&Wind::callback::strike, result.dup());
	return result.waitFor(qid);
}

WIND_API K K_DECL Wind_bktEnd(K params) {
	std::wstring paras;
	try {
		paras = Wind::util::qDict2WStringMapJoin(params, L';', L'=');
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	Wind::callback::Result result;
	::WQID const qid = ::BktEnd(paras.c_str(), &Wind::callback::strike, result.dup());
	return result.waitFor(qid);
}

WIND_API K K_DECL Wind_bktOrder(K time, K windCode, K side, K volume, K params) {
	std::wstring ts, code, op, size, paras;
	try {
		ts = Wind::util::q2DateTimeStr(time);
		code = q::q2WString(windCode);
		op = q::q2WString(side);
		std::wostringstream buffer;
		buffer << q::q2Fp(volume);
		size = buffer.str();
		paras = Wind::util::qDict2WStringMapJoin(params, L';', L'=');
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	Wind::callback::Result result;
	::WQID const qid = ::BktOrder(ts.c_str(), code.c_str(), op.c_str(), size.c_str(), paras.c_str(),
		&Wind::callback::strike, result.dup());
	return result.waitFor(qid);
}
*/