#include "stdafx.h"
#include "Wind_API.h"

#include "util.h"
#include "Wind_strike.h"

#include "kdb+.util/util.h"
#include "kdb+.util/type_convert.h"

WIND_API K K_DECL Wind_wupf(K portfolioName, K tradeDates, K windCodes, K quantities, K costPrices, K params) {
	std::wstring portf, dates, codes, qtys, costs, paras;
	try {
		portf = q::q2WString(portfolioName);
		dates = Wind::util::qList2DateStrJoin(tradeDates, L',');
		codes = Wind::util::qList2WStringJoin(windCodes, L',');
		qtys = Wind::util::qList2DecStrJoin(quantities, L',');
		costs = Wind::util::qList2FpStrJoin(costPrices, L',');
		paras = Wind::util::qDict2WStringMapJoin(params, L';', L'=');
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	Wind::callback::Result result;
	::WQID const qid = ::WUPF(portf.c_str(), dates.c_str(), codes.c_str(), qtys.c_str(), costs.c_str(), paras.c_str(),
		&Wind::callback::strike, result.dup());
	return result.waitFor(qid);
}