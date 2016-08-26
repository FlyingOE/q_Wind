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

		// Traits to assist symbolic retrieval of tick data fields from TDBDefine_Order.
		struct Order : public Wind::mapper::Fields<::TDBDefine_Order> {
			typedef Singleton<Order> accessor_map;
			typedef ::TDBDefine_Order tdb_result_type;

			Order() : Wind::mapper::Fields<tdb_result_type>() { registerAllFields(); }

			typedef Wind::accessor::SymbolAccessor<tdb_result_type, char[32]> SymbolAccessor;
			typedef Wind::accessor::DateAccessor<tdb_result_type> DateAccessor;
			typedef Wind::accessor::TimeAccessor<tdb_result_type> TimeAccessor;
			typedef Wind::accessor::IntAccessor<tdb_result_type, I> IntAccessor;
			typedef Wind::accessor::CharAccessor<tdb_result_type> CharAccessor;
			typedef Wind::accessor::FloatAccessor<tdb_result_type, int> FloatAccessor;

			// Data fields -- always keep in sync with TDBDefine_Order
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
				// 交易所委托号
				addField("OrderID", new IntAccessor(&tdb_result_type::nOrder));
				// 委托类别
				addField("OrderType", new CharAccessor(&tdb_result_type::chOrderKind));
				// 委托代码, B, S, C
				addField("Function", new CharAccessor(&tdb_result_type::chFunctionCode));
				// 成交价格
				addField("Price", new FloatAccessor(&tdb_result_type::nOrderPrice, .0001));
				// 成交数量
				addField("Size", new FloatAccessor(&tdb_result_type::nOrderVolume));
			}
		};

	}//namespace TDB::traits
}//namespace TDB

TDB_API K K_DECL TDB_order_fields(K _) {
	return TDB::traits::Order::accessor_map::getInstance()->getFields();
}

TDB_API K K_DECL TDB_order(K h, K windCode, K indicators, K date, K begin, K end) {
	::THANDLE tdb = NULL;
	std::vector<TDB::traits::Order::field_accessor const*> indis;
	::TDBDefine_ReqOrder req = { 0 };
	try {
		TDB::parseTdbHandle(h, tdb);
		TDB::parseIndicators<TDB::traits::Order>(indicators, indis);
		TDB::parseTdbReqCode(tdb, windCode, req);
		TDB::parseTdbReqTime(date, begin, end, req);
	}
	catch (std::runtime_error const& error) {
		return q::error2q(error.what());
	}

	return TDB::runQuery<TDB::traits::Order, ::TDBDefine_ReqOrder>(tdb, req, indis, &::TDB_GetOrder);
}