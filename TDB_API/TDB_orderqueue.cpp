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

		// Traits to assist symbolic retrieval of tick data fields from TDBDefine_OrderQueue.
		struct OrderQueue : public Wind::mapper::Fields<::TDBDefine_OrderQueue> {
			typedef Singleton<OrderQueue> accessor_map;
			typedef ::TDBDefine_OrderQueue tdb_result_type;

			OrderQueue() : Wind::mapper::Fields<tdb_result_type>() { registerAllFields(); }

			typedef Wind::accessor::SymbolAccessor<tdb_result_type, char[32]> SymbolAccessor;
			typedef Wind::accessor::DateAccessor<tdb_result_type> DateAccessor;
			typedef Wind::accessor::TimeAccessor<tdb_result_type> TimeAccessor;
			typedef Wind::accessor::IntAccessor<tdb_result_type, C> SideAccessor;
			typedef Wind::accessor::IntAccessor<tdb_result_type, I> IntAccessor;
			typedef Wind::accessor::CharAccessor<tdb_result_type> CharAccessor;
			typedef Wind::accessor::FloatAccessor<tdb_result_type, int> FloatAccessor;
			template <typename FieldT>
			using FloatsAccessor = Wind::accessor::VarFloatsAccessor<tdb_result_type, FieldT>;

			// Data fields -- always keep in sync with TDBDefine_OrderQueue
			void registerAllFields() {
				// 万得代码(AG1312.SHF)
				addField("WindCode", new SymbolAccessor(&tdb_result_type::chWindCode));
				// 交易所代码(ag1312)
				addField("Code", new SymbolAccessor(&tdb_result_type::chCode));
				// 日期（自然日）格式YYMMDD
				addField("Date", new DateAccessor(&tdb_result_type::nDate));
				// 时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
				addField("Time", new TimeAccessor(&tdb_result_type::nTime));
				// 买卖方向('B':Bid 'A' : Ask)
				addField("Side", new SideAccessor(&tdb_result_type::nSide));
				// 成交价格
				addField("Price", new FloatAccessor(&tdb_result_type::nPrice, .0001));
				// 订单数量
				addField("OrderCount", new IntAccessor(&tdb_result_type::nOrderItems));
				// 订单明细
				addField("Sizes", new FloatsAccessor<int[50]>(&tdb_result_type::nABVolume, &tdb_result_type::nABItems));
			}
		};

	}//namespace TDB::traits
}//namespace TDB

TDB_API K K_DECL TDB_orderQueue_fields(K _) {
	return TDB::traits::OrderQueue::accessor_map::getInstance()->getFields();
}

TDB_API K K_DECL TDB_orderQueue(K h, K windCode, K indicators, K begin, K end) {
	::THANDLE tdb = NULL;
	std::vector<TDB::traits::OrderQueue::field_accessor const*> indis;
	::TDBDefine_ReqOrderQueue req = { 0 };
	try {
		TDB::parseTdbHandle(h, tdb);
		TDB::parseIndicators<TDB::traits::OrderQueue>(indicators, indis);
		TDB::parseTdbReq(windCode, begin, end, req);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	return TDB::runQuery<TDB::traits::OrderQueue, ::TDBDefine_ReqOrderQueue>(tdb, req, indis, &::TDB_GetOrderQueue);
}