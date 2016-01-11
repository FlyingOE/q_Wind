#include "stdafx.h"
#include "TDB_API.h"

#include "TDB_API_helper.h"

#include <algorithm>
#include <locale>
#include <set>
#include "win32.util/Singleton.h"
#include "kdb+.util/util.h"
#include "kdb+.util/K_ptr.h"
#include "kdb+.util/type_convert.h"
#include "Wind.util/FieldMapper.h"

namespace TDB {
	namespace traits {

		// Traits to assist symbolic retrieval of tick data fields from TDBDefine_Tick.
		struct Tick : public Wind::mapper::Fields<::TDBDefine_Tick> {
			typedef Singleton<Tick> accessor_map;
			typedef ::TDBDefine_Tick tdb_result_type;

			Tick() : Wind::mapper::Fields<tdb_result_type>() { registerAllFields(); }

			typedef Wind::accessor::SymbolAccessor<tdb_result_type, char[32]> SymbolAccessor;
			typedef Wind::accessor::DateAccessor<tdb_result_type> DateAccessor;
			typedef Wind::accessor::TimeAccessor<tdb_result_type> TimeAccessor;
			typedef Wind::accessor::IntAccessor<tdb_result_type, I> IntAccessor;
			typedef Wind::accessor::CharAccessor<tdb_result_type> CharAccessor;
			template <typename FieldT>
			using FloatAccessor = Wind::accessor::FloatAccessor<tdb_result_type, FieldT>;
			template <typename FieldT>
			using FloatsAccessor = Wind::accessor::FloatsAccessor<tdb_result_type, FieldT>;

			// Data fields -- always keep in sync with TDBDefine_Tick
			void registerAllFields() {
				// 万得代码(AG1312.SHF)
				addField("WindCode", new SymbolAccessor(&tdb_result_type::chWindCode));
				// 交易所代码(ag1312)
				addField("Code", new SymbolAccessor(&tdb_result_type::chCode));
				// 日期（自然日）格式YYMMDD
				addField("Date", new DateAccessor(&tdb_result_type::nDate));
				// 时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
				addField("Time", new TimeAccessor(&tdb_result_type::nTime));
				// 成交价格
				addField("Price", new FloatAccessor<int>(&tdb_result_type::nPrice, .0001));
				// 成交数量
				addField("Size", new FloatAccessor<__int64>(&tdb_result_type::iVolume));
				// 成交额(元)
				addField("Amount", new FloatAccessor<__int64>(&tdb_result_type::iTurover));
				// 成交笔数
				addField("MatchCount", new IntAccessor(&tdb_result_type::nMatchItems));
				// IOPV(基金)、利息(债券)
				addField("Interest", new FloatAccessor<int>(&tdb_result_type::nInterest));
				// 成交标志
				addField("TradeFlag", new CharAccessor(&tdb_result_type::chTradeFlag));
				// BS标志
				addField("Side", new CharAccessor(&tdb_result_type::chBSFlag));
				// 当日累计成交量
				addField("AccumSize", new FloatAccessor<__int64>(&tdb_result_type::iAccVolume));
				// 当日成交额(元)
				addField("AccumAmount", new FloatAccessor<__int64>(&tdb_result_type::iAccTurover));
				// 最高
				addField("High", new FloatAccessor<int>(&tdb_result_type::nHigh, .0001));
				// 最低
				addField("Low", new FloatAccessor<int>(&tdb_result_type::nLow, .0001));
				// 开盘
				addField("Open", new FloatAccessor<int>(&tdb_result_type::nOpen, .0001));
				// 前收盘
				addField("PreClose", new FloatAccessor<int>(&tdb_result_type::nPreClose, .0001));

				registerFuturesFields();
				registerBidAskFields();
				registerIndexFields();

				addField("RESERVED", "reserve1", new FloatAccessor<int>(&tdb_result_type::nResv1));
				addField("RESERVED", "reserve2", new FloatAccessor<int>(&tdb_result_type::nResv2));
				addField("RESERVED", "reserve3", new FloatAccessor<int>(&tdb_result_type::nResv3));
			}

			void registerFuturesFields() {
				// 结算价
				addField("FUTURES", "SettlePrice", new FloatAccessor<int>(&tdb_result_type::nSettle, .0001));
				// 持仓量
				addField("FUTURES", "Position", new FloatAccessor<int>(&tdb_result_type::nPosition));
				// 虚实度
				addField("FUTURES", "Delta", new FloatAccessor<int>(&tdb_result_type::nCurDelta));
				// 昨结算
				addField("FUTURES", "PreSettlePrice", new FloatAccessor<int>(&tdb_result_type::nPreSettle, .0001));
				// 昨持仓
				addField("FUTURES", "PrePosition", new FloatAccessor<int>(&tdb_result_type::nPrePosition));
			}

			void registerBidAskFields() {
				// 叫卖价
				addField("BIDASK", "Asks", new FloatsAccessor<int[10]>(&tdb_result_type::nAskPrice, .0001));
				// 叫卖量
				addField("BIDASK", "AskSizes", new FloatsAccessor<unsigned int[10]>(&tdb_result_type::nAskVolume));
				// 叫买价
				addField("BIDASK", "Bids", new FloatsAccessor<int[10]>(&tdb_result_type::nBidPrice, .0001));
				// 叫买量
				addField("BIDASK", "BidSizes", new FloatsAccessor<unsigned int[10]>(&tdb_result_type::nBidVolume));
				// 加权平均叫卖价(上海L2)
				addField("BIDASK", "AvgAsk", new FloatAccessor<int>(&tdb_result_type::nAskAvPrice, .0001));
				// 加权平均叫买价(上海L2)
				addField("BIDASK", "AvgBid", new FloatAccessor<int>(&tdb_result_type::nBidAvPrice, .0001));
				// 叫卖总量(上海L2)
				addField("BIDASK", "TotalAskSize", new FloatAccessor<__int64>(&tdb_result_type::iTotalAskVolume));
				// 叫买总量(上海L2)
				addField("BIDASK", "TotalBidSize", new FloatAccessor<__int64>(&tdb_result_type::iTotalBidVolume));
			}

			void registerIndexFields() {
				// （指数）不加权指数
				addField("INDEX", "Index", new IntAccessor(&tdb_result_type::nIndex));
				// （指数）品种总数
				addField("INDEX", "StockCount", new IntAccessor(&tdb_result_type::nStocks));
				// （指数）上涨品种数
				addField("INDEX", "UpCount", new IntAccessor(&tdb_result_type::nUps));
				// （指数）下跌品种数
				addField("INDEX", "DownCount", new IntAccessor(&tdb_result_type::nDowns));
				// （指数）持平品种数
				addField("INDEX", "HoldCount", new IntAccessor(&tdb_result_type::nHoldLines));
			}
		};

	}//namespace TDB::traits
}//namespace TDB

TDB_API K K_DECL TDB_tick_fields(K category) {
	std::string cat;
	try {
		cat = q::q2String(category);
		std::transform(cat.begin(), cat.end(), cat.begin(),
			[](char x) { return std::toupper(x, std::locale()); }
		);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	auto const mapper = TDB::traits::Tick::accessor_map::getInstance();
	if (!mapper->hasCategory(cat)) {
		std::vector<std::string> categories;
		mapper->getCategories(categories);
		std::ostringstream buffer;
		for (auto c = categories.cbegin(); c != categories.cend(); ++c) {
			buffer << "`" << *c;
		}
		return q::error2q(buffer.str());
	}
	else {
		return mapper->getFields(cat);
	}
}

TDB_API K K_DECL TDB_tick(K h, K windCode, K indicators, K date, K begin, K end) {
	::THANDLE tdb = NULL;
	std::vector<TDB::traits::Tick::field_accessor const*> indis;
	::TDBDefine_ReqTick req = { 0 };
	try {
		TDB::parseTdbHandle(h, tdb);
		TDB::parseIndicators<TDB::traits::Tick>(indicators, indis);
		TDB::parseTdbReqCode(tdb, windCode, req);
		TDB::parseTdbReqTime(date, begin, end, req);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	req.nAutoComplete = 0;

	return TDB::runQuery<TDB::traits::Tick, ::TDBDefine_ReqTick>(tdb, req, indis, &::TDB_GetTick);
}