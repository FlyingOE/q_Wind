#include "stdafx.h"
#include "TDB_API.h"

#include "TDB_API_helper.h"

#include "win32.util/EnumUtil.h"
#include "kdb+.util/K_ptr.h"
#include "kdb+.util/type_convert.h"

namespace TDB {

	// Heper to assist symbolic retrieval of tick data fields from TDBDefine_TickAB.
	struct TickAB {

		// Data fields -- always keep in sync with TDBDefine_TickAB
		enum Field {
			NIL = 0,		//`NULL' for this enum
			WindCode,		//万得代码(AG1312.SHF)
			Code,			//交易所代码(ag1312)
			Date,			//日期（自然日）格式YYMMDD
			Time,			//时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
			Price,			//成交价
			Volume,			//成交量
			Turover,		//成交额(元)
			MatchItems,		//成交笔数
			Interest,		//IOPV(基金)、利息(债券)
			TradeFlag,		//成交标志
			BSFlag,			//BS标志
			AccVolume,		//当日累计成交量
			AccTurover,		//当日成交额(元)
			High,			//最高
			Low,			//最低
			Open,			//开盘
			PreClose,		//前收盘
			AskPrices,		//叫卖价
			AskVolumes,		//叫卖量
			BidPrices,		//叫买价
			BidVolumes,		//叫买量
			AskAvPrice,		//加权平均叫卖价(上海L2)
			BidAvPrice,		//加权平均叫买价(上海L2)
			TotalAskVolume,	//叫卖总量(上海L2)
			TotalBidVolume,	//叫买总量(上海L2)
			Index,			//（指数）不加权指数
			Stocks,			//（指数）品种总数
			Ups,			//（指数）上涨品种数
			Downs,			//（指数）下跌品种数
			HoldLines,		//（指数）持平品种数
		};

		typedef ::TDBDefine_TickAB tdb_result_type;
		typedef FieldAccessor<tdb_result_type> field_accessor_type;
		typedef CharAccessor<tdb_result_type> CharAccessor_;
		typedef DateAccessor<tdb_result_type> DateAccessor_;
		typedef TimeAccessor<tdb_result_type> TimeAccessor_;
		typedef IntAccessor<tdb_result_type, I> IntAccessor_;
		template <typename Str> using SymbolAccessor_ = SymbolAccessor<tdb_result_type, Str>;
		template <typename Val> using FloatAccessor_ = FloatAccessor<tdb_result_type, Val>;
		template <typename Vals> using FloatsAccessor_ = FloatsAccessor<tdb_result_type, Vals>;

		static std::map<Field, std::unique_ptr<field_accessor_type> > Accessors;

		// Data field names
		struct FieldName :
			public enum_util::EnumBase<FieldName, Field>
		{
			static void registerAll() {
#				define TICKAB_FIELD(name, accessor)	\
					ENUM_STRING(name);	\
					TickAB::Accessors.insert(std::make_pair(name, std::unique_ptr<field_accessor_type>(accessor)))

				TICKAB_FIELD(WindCode, new SymbolAccessor_<char[32]>(&tdb_result_type::chWindCode));
				TICKAB_FIELD(Code, new SymbolAccessor_<char[32]>(&tdb_result_type::chCode));
				TICKAB_FIELD(Date, new DateAccessor_(&tdb_result_type::nDate));
				TICKAB_FIELD(Time, new TimeAccessor_(&tdb_result_type::nTime));
				TICKAB_FIELD(Price, new FloatAccessor_<int>(&tdb_result_type::nPrice, .0001));
				TICKAB_FIELD(Volume, new FloatAccessor_<__int64>(&tdb_result_type::iVolume));
				TICKAB_FIELD(Turover, new FloatAccessor_<__int64>(&tdb_result_type::iTurover));
				TICKAB_FIELD(MatchItems, new IntAccessor_(&tdb_result_type::nMatchItems));
				TICKAB_FIELD(Interest, new FloatAccessor_<int>(&tdb_result_type::nInterest));
				TICKAB_FIELD(TradeFlag, new CharAccessor_(&tdb_result_type::chTradeFlag));
				TICKAB_FIELD(BSFlag, new CharAccessor_(&tdb_result_type::chBSFlag));
				TICKAB_FIELD(AccVolume, new FloatAccessor_<__int64>(&tdb_result_type::iAccVolume));
				TICKAB_FIELD(AccTurover, new FloatAccessor_<__int64>(&tdb_result_type::iAccTurover));
				TICKAB_FIELD(High, new FloatAccessor_<int>(&tdb_result_type::nHigh, .0001));
				TICKAB_FIELD(Low, new FloatAccessor_<int>(&tdb_result_type::nLow, .0001));
				TICKAB_FIELD(Open, new FloatAccessor_<int>(&tdb_result_type::nOpen, .0001));
				TICKAB_FIELD(PreClose, new FloatAccessor_<int>(&tdb_result_type::nPreClose, .0001));
				TICKAB_FIELD(AskPrices, new FloatsAccessor_<int[10]>(&tdb_result_type::nAskPrice, .0001));
				TICKAB_FIELD(AskVolumes, new FloatsAccessor_<unsigned[10]>(&tdb_result_type::nAskVolume));
				TICKAB_FIELD(BidPrices, new FloatsAccessor_<int[10]>(&tdb_result_type::nBidPrice, .0001));
				TICKAB_FIELD(BidVolumes, new FloatsAccessor_<unsigned[10]>(&tdb_result_type::nBidVolume));
				TICKAB_FIELD(AskAvPrice, new FloatAccessor_<int>(&tdb_result_type::nAskAvPrice, .0001));
				TICKAB_FIELD(BidAvPrice, new FloatAccessor_<int>(&tdb_result_type::nBidAvPrice, .0001));
				TICKAB_FIELD(TotalAskVolume, new FloatAccessor_<__int64>(&tdb_result_type::iTotalAskVolume));
				TICKAB_FIELD(TotalBidVolume, new FloatAccessor_<__int64>(&tdb_result_type::iTotalBidVolume));
				TICKAB_FIELD(Index, new IntAccessor_(&tdb_result_type::nIndex));
				TICKAB_FIELD(Stocks, new IntAccessor_(&tdb_result_type::nStocks));
				TICKAB_FIELD(Ups, new IntAccessor_(&tdb_result_type::nUps));
				TICKAB_FIELD(Downs, new IntAccessor_(&tdb_result_type::nDowns));
				TICKAB_FIELD(HoldLines, new IntAccessor_(&tdb_result_type::nHoldLines));
			}
		};
	};

}//namespace TDB


std::map<TDB::TickAB::Field, std::unique_ptr<TDB::TickAB::field_accessor_type> >
	TDB::TickAB::Accessors;

TDB_API K K_DECL TDB_tickAB_fields(K _) {
	return TDB::getFields<TDB::TickAB>();
}

TDB_API K K_DECL TDB_tickAB(K h, K windCode, K indicators, K begin, K end) {
	::THANDLE tdb = NULL;
	std::vector<TDB::TickAB::field_accessor_type const*> indis;
	::TDBDefine_ReqTick req = { 0 };
	try {
		TDB::parseTdbHandle(h, tdb);
		TDB::parseIndicators<TDB::TickAB>(indicators, indis);
		TDB::parseTdbReq(windCode, begin, end, req);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	return TDB::runQuery<TDB::TickAB, ::TDBDefine_ReqTick>(tdb, req, indis, &::TDB_GetTickAB);
}