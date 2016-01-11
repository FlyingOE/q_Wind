#include "stdafx.h"
#include "TDB_API.h"

#include "TDB_API_helper.h"

#include "kdb+.util/util.h"
#include "kdb+.util/K_ptr.h"

TDB_API K K_DECL TDB_codeTable(K h, K market) {
	::THANDLE tdb = NULL;
	std::string mkt;
	try {
		TDB::parseTdbHandle(h, tdb);
		mkt = q::q2String(market);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	typedef ::TDBDefine_Code tdb_result_type;
	int codeCount = 0;
	tdb_result_type* t = NULL;
	::TDB_ERROR const result = static_cast<::TDB_ERROR>(::TDB_GetCodeTable(tdb, mkt.c_str(), &t, &codeCount));
	TDB::Ptr<tdb_result_type> codes(t);
	if (result != TDB_SUCCESS) {
		return q::error2q(TDB::getError(result));
	}
	assert(codes);
	assert(codeCount >= 0);

	typedef Wind::accessor::SymbolAccessor<tdb_result_type, char[32]> SymbolAccessor;
	typedef TDB::MarketIdAccessor<tdb_result_type> MarketIdAccessor;
	typedef TDB::MarketInfoAccessor<tdb_result_type, H> MarketInfoAccessor;
	typedef Wind::accessor::SymbolAccessor<tdb_result_type, char[32], Wind::encoder::GB18030_UTF8> CnNameAccessor;
	typedef Wind::accessor::IntAccessor<tdb_result_type, G> TypeAccessor;

	q::K_ptr data(ktn(0, 8));
	//万得代码(AG1312.SHF)
	kK(data.get())[0] = SymbolAccessor(&tdb_result_type::chWindCode).extract(codes.get(), codeCount);
	//交易所代码(ag1312)
	kK(data.get())[1] = SymbolAccessor(&tdb_result_type::chCode).extract(codes.get(), codeCount);
	//市场代码(SHF
	kK(data.get())[2] = MarketIdAccessor(&tdb_result_type::chMarket).extract(codes.get(), codeCount);
	//证券中文名称
	kK(data.get())[3] = CnNameAccessor(&tdb_result_type::chCNName).extract(codes.get(), codeCount);
	//证券英文名称
	kK(data.get())[4] = SymbolAccessor(&tdb_result_type::chENName).extract(codes.get(), codeCount);
	//证券类型
	kK(data.get())[5] = TypeAccessor(&tdb_result_type::nType).extract(codes.get(), codeCount);
	//Market level
	kK(data.get())[6] = MarketInfoAccessor(&tdb_result_type::chMarket, 1).extract(codes.get(), codeCount);
	//Data source ID
	kK(data.get())[7] = MarketInfoAccessor(&tdb_result_type::chMarket, 2).extract(codes.get(), codeCount);
	return data.release();
}

TDB_API K K_DECL TDB_codeInfo(K h, K windCode, K market) {
	::THANDLE tdb = NULL;
	std::string code, mkt;
	try {
		TDB::parseTdbHandle(h, tdb);
		code = q::q2String(windCode);
		mkt = TDB::getMarketId(tdb, q::q2String(market));
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	typedef ::TDBDefine_Code tdb_result_type;
	tdb_result_type const* info = ::TDB_GetCodeInfo(tdb, code.c_str(), mkt.c_str());
	if (info == NULL) {
		return q::error2q(TDB::getError(TDB_NETWORK_ERROR));
	}

	q::K_ptr data(ktn(0, 8));
	std::vector<std::string> mktTokens = util::split(info->chMarket, '-');
	kK(data.get())[0] = ks(const_cast<S>(info->chWindCode));
	kK(data.get())[1] = ks(const_cast<S>(info->chCode));
	kK(data.get())[2] = ks((mktTokens.size() <= 0) ? q::type_traits<S>::NULL_VAL : const_cast<S>(mktTokens[0].c_str()));
	kK(data.get())[3] = ks(const_cast<S>(Wind::encoder::GB18030_UTF8::encode(info->chCNName).c_str()));
	kK(data.get())[4] = ks(const_cast<S>(info->chENName));
	kK(data.get())[5] =	kg(info->nType);
	long const level = (mktTokens.size() <= 1) ? q::type_traits<H>::NULL_VAL : std::strtol(mktTokens[1].c_str(), NULL, 10);
	kK(data.get())[6] = kh(static_cast<H>(level));
	long const src =   (mktTokens.size() <= 2) ? q::type_traits<H>::NULL_VAL : std::strtol(mktTokens[2].c_str(), NULL, 10);
	kK(data.get())[7] = kh(static_cast<H>(src));
	return data.release();
}