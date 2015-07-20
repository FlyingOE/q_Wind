#include "stdafx.h"
#include "TDF_API.h"

#include "util.h"
#include "TDF_API_helper.h"

#include "kdb+.util/K_ptr.h"
#include "kdb+.util/type_convert.h"
#include "Wind.util/FieldAccessors.h"

TDF_API K K_DECL TDF_codeTable(K h, K market) {
	::THANDLE tdf = NULL;
	std::string mkt;
	try {
		TDF::parseTdfHandle(h, tdf);
		mkt = q::q2String(market);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	unsigned int codeCount = 0;
	::TDF_CODE* t = NULL;
	::TDF_ERR const result = static_cast<::TDF_ERR>(::TDF_GetCodeTable(tdf, mkt.c_str(), &t, &codeCount));
	TDF::Ptr<::TDF_CODE> codes(t);
	if (result != TDF_ERR_SUCCESS) {
		return q::error2q(TDF::getError(result));
	}
	assert(codes);
	assert(codeCount >= 0);

	q::K_ptr data(ktn(0, 6));
	kK(data.get())[0] =		//Wind Code: AG1312.SHF
		Wind::accessor::SymbolAccessor<::TDF_CODE, char[32]>(&::TDF_CODE::szWindCode).extract(codes.get(), codeCount);
	kK(data.get())[1] =		//market code: SHF
		Wind::accessor::SymbolAccessor<::TDF_CODE, char[ 8]>(&::TDF_CODE::szMarket).extract(codes.get(), codeCount);
	kK(data.get())[2] =		//original code: ag1312
		Wind::accessor::SymbolAccessor<::TDF_CODE, char[32]>(&::TDF_CODE::szCode).extract(codes.get(), codeCount);
	kK(data.get())[3] =
		Wind::accessor::SymbolAccessor<::TDF_CODE, char[32]>(&::TDF_CODE::szENName).extract(codes.get(), codeCount);
	kK(data.get())[4] =		//Chinese name: »¦Òø1302
		Wind::accessor::SymbolAccessor<::TDF_CODE, char[32], Wind::encoder::GB18030_UTF8>(&::TDF_CODE::szCNName).extract(codes.get(), codeCount);
	kK(data.get())[5] =
		Wind::accessor::IntAccessor<::TDF_CODE, G>(&::TDF_CODE::nType).extract(codes.get(), codeCount);
	return data.release();
}

TDF_API K K_DECL TDF_optionCodeInfo(K h, K windCode) {
	::THANDLE tdf = NULL;
	std::string code;
	try {
		TDF::parseTdfHandle(h, tdf);
		code = q::q2String(windCode);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	::TDF_OPTION_CODE info = { 0 };
	::TDF_ERR result = static_cast<::TDF_ERR>(::TDF_GetOptionCodeInfo(tdf, code.c_str(), &info));
	if (result != TDF_ERR_SUCCESS) {
		return q::error2q(::TDF::getError(result));
	}

	q::K_ptr data(ktn(0, 6 + 12));
	kK(data.get())[0 +  0] = ks(const_cast<S>(info.basicCode.szWindCode));
	kK(data.get())[0 +  1] = ks(const_cast<S>(info.basicCode.szMarket));
	kK(data.get())[0 +  2] = ks(const_cast<S>(info.basicCode.szCode));
	kK(data.get())[0 +  3] = ks(const_cast<S>(info.basicCode.szENName));
	kK(data.get())[0 +  4] = ks(const_cast<S>(Wind::encoder::GB18030_UTF8::encode(info.basicCode.szCNName).c_str()));
	kK(data.get())[0 +  5] = kg(info.basicCode.nType);
	kK(data.get())[6 +  0] = ks(const_cast<S>(info.szContractID));
	kK(data.get())[6 +  1] = ks(const_cast<S>(info.szUnderlyingSecurityID));
	kK(data.get())[6 +  2] = kc(info.chCallOrPut);
	kK(data.get())[6 +  3] = kd(q::date2q(info.nExerciseDate));
	kK(data.get())[6 +  4] = kc(info.chUnderlyingType);
	kK(data.get())[6 +  5] = kc(info.chOptionType);
	kK(data.get())[6 +  6] = kc(info.chPriceLimitType);
	kK(data.get())[6 +  7] = ki(info.nContractMultiplierUnit);
	kK(data.get())[6 +  8] = kf(info.nExercisePrice);
	kK(data.get())[6 +  9] = kd(q::date2q(info.nStartDate));
	kK(data.get())[6 + 10] = kd(q::date2q(info.nEndDate));
	kK(data.get())[6 + 11] = kd(q::date2q(info.nExpireDate));
	return data.release();
}