#include "stdafx.h"
#include "TDB_API.h"

#include "TDB_API_helper.h"

#include "win32.util/Singleton.h"
#include "kdb+.util/util.h"
#include "kdb+.util/K_ptr.h"
#include "kdb+.util/type_convert.h"
#include "Wind.util/FieldMapper.h"

namespace TDB {
	namespace traits {

		// Traits to assist symbolic retrieval of tick data fields from TDBDefine_Transaction.
		struct Transaction : public Wind::mapper::Fields<::TDBDefine_Transaction> {
			typedef Singleton<Transaction> accessor_map;
			typedef ::TDBDefine_Transaction tdb_result_type;

			Transaction() : Wind::mapper::Fields<tdb_result_type>() { registerAllFields(); }

			typedef Wind::accessor::SymbolAccessor<tdb_result_type, char[32]> SymbolAccessor;
			typedef Wind::accessor::DateAccessor<tdb_result_type> DateAccessor;
			typedef Wind::accessor::TimeAccessor<tdb_result_type> TimeAccessor;
			typedef Wind::accessor::IntAccessor<tdb_result_type, I> IntAccessor;
			typedef Wind::accessor::CharAccessor<tdb_result_type> CharAccessor;
			typedef Wind::accessor::FloatAccessor<tdb_result_type, int> FloatAccessor;

			// Data fields -- always keep in sync with TDBDefine_Transaction
			void registerAllFields() {
				// 万得代码(AG1312.SHF)
				addField("WindCode", new SymbolAccessor(&tdb_result_type::chWindCode));
				// 交易所代码(ag1312)
				addField("Code", new SymbolAccessor(&tdb_result_type::chCode));
				// 日期（自然日）格式YYMMDD
				addField("Date", new DateAccessor(&tdb_result_type::nDate));
				// 时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
				addField("Time", new TimeAccessor(&tdb_result_type::nTime));
				// 成交编号(从1开始，递增1)
				addField("Index", new IntAccessor(&tdb_result_type::nIndex));
				// 成交代码（8931）: 'C', '0'
				addField("Function", new CharAccessor(&tdb_result_type::chFunctionCode));
				// 委托类别（8930）
				addField("OrderType", new CharAccessor(&tdb_result_type::chOrderKind));
				// BS标志
				addField("Side", new CharAccessor(&tdb_result_type::chBSFlag));
				// 成交价格
				addField("Price", new FloatAccessor(&tdb_result_type::nTradePrice, .0001));
				// 成交数量
				addField("Size", new FloatAccessor(&tdb_result_type::nTradeVolume));
				// 叫卖序号
				addField("AskID", new IntAccessor(&tdb_result_type::nAskOrder));
				// 叫买序号
				addField("BidID", new IntAccessor(&tdb_result_type::nBidOrder));
			}
		};

	}//namespace TDB::traits
}//namespace TDB

TDB_API K K_DECL TDB_transaction_fields(K _) {
	return TDB::traits::Transaction::accessor_map::getInstance()->getFields();
}

TDB_API K K_DECL TDB_transaction(K h, K windCode, K indicators, K date, K begin, K end) {
	::THANDLE tdb = NULL;
	std::vector<TDB::traits::Transaction::field_accessor const*> indis;
	::TDBDefine_ReqTransaction req = { 0 };
	try {
		TDB::parseTdbHandle(h, tdb);
		TDB::parseIndicators<TDB::traits::Transaction>(indicators, indis);
		TDB::parseTdbReq(windCode, date, begin, end, req);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}
	return TDB::runQuery<TDB::traits::Transaction, ::TDBDefine_ReqTransaction>(tdb, req, indis, &::TDB_GetTransaction);
}