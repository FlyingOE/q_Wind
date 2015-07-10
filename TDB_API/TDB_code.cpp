#include "stdafx.h"
#include "TDB_API.h"

#include "TDB_API_helper.h"

#include "kdb+.util/K_ptr.h"
#include <string>

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

	int codeCount = 0;
	::TDBDefine_Code* t = NULL;
	int const result = ::TDB_GetCodeTable(tdb, mkt.c_str(), &t, &codeCount);
	TDB::Ptr<::TDBDefine_Code> codes(t);
	if (result != TDB_SUCCESS) {
		return q::error2q(TDB::getError(result));
	}
	assert(codes);
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

TDB_API K K_DECL TDB_codeInfo(K h, K windCode) {
	::THANDLE tdb = NULL;
	std::string code;
	try {
		TDB::parseTdbHandle(h, tdb);
		code = q::q2String(windCode);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}


	::TDBDefine_Code const* info = ::TDB_GetCodeInfo(tdb, code.c_str());
	if (info == NULL) {
		return q::error2q(TDB::getError(TDB_NETWORK_ERROR));
	}

	q::K_ptr data(ktn(0, 6));
	typedef TDB::SymbolAccessor<::TDBDefine_Code, char[32]> SymbolAccessor_;
	kK(data.get())[0] = ks(const_cast<S>(info->chWindCode));
	kK(data.get())[1] = ks(const_cast<S>(info->chCode));
	kK(data.get())[2] = ks(const_cast<S>(info->chMarket));
	kK(data.get())[3] = ks(const_cast<S>(TDB::util::GB18030Encoder()(info->chCNName).c_str()));
	kK(data.get())[4] = ks(const_cast<S>(info->chENName));
	kK(data.get())[5] =	kg(info->nType);
	return data.release();
}
