#include "stdafx.h"
#include "TDB_API.h"

#include "TDB_API_helper.h"

#include "win32.util/EnumUtil.h"
#include "kdb+.util/K_ptr.h"
#include "kdb+.util/type_convert.h"

namespace TDB {

	// Heper to assist symbolic retrieval of tick data fields from TDBDefine_Order.
	struct Order {

		// Data fields -- always keep in sync with TDBDefine_Order
		enum Field {
			NIL = 0,		//`NULL' for this enum
			WindCode,		//万得代码(AG1312.SHF)
			Code,			//交易所代码(ag1312)
			Date,			//日期（自然日）格式YYMMDD
			Time,			//时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
			Index,			//成交编号(从1开始，递增1)
			OrderId,		//交易所委托号
			OrderKind,      //委托类别
			FunctionCode,	//委托代码, B, S, C
			OrderPrice,		//委托价格((a double number + 0.00005) *10000)
			OrderVolume,	//委托数量
		};

		typedef ::TDBDefine_Order tdb_result_type;
		typedef FieldAccessor<tdb_result_type> field_accessor_type;
		typedef CharAccessor<tdb_result_type> CharAccessor_;
		typedef DateAccessor<tdb_result_type> DateAccessor_;
		typedef TimeAccessor<tdb_result_type> TimeAccessor_;
		typedef IntAccessor<tdb_result_type, I> IntAccessor_;
		template <typename Str> using SymbolAccessor_ = SymbolAccessor<tdb_result_type, Str>;
		template <typename Str, typename Encoder> using StringAccessor_ = StringAccessor<tdb_result_type, Str, Encoder>;
		template <typename Val> using FloatAccessor_ = FloatAccessor<tdb_result_type, Val>;

		static std::map<Field, std::unique_ptr<field_accessor_type> > Accessors;

		// Data field names
		struct FieldName : public enum_util::EnumBase<FieldName, Field>
		{
			static void registerAll() {
#				define ORDER_FIELD(name, accessor)	\
					ENUM_STRING(name);	\
					Order::Accessors.insert(std::make_pair(name, std::unique_ptr<field_accessor_type>(accessor)))

				ORDER_FIELD(WindCode, new SymbolAccessor_<char[32]>(&tdb_result_type::chWindCode));
				ORDER_FIELD(Code, new SymbolAccessor_<char[32]>(&tdb_result_type::chCode));
				ORDER_FIELD(Date, new DateAccessor_(&tdb_result_type::nDate));
				ORDER_FIELD(Time, new TimeAccessor_(&tdb_result_type::nTime));
				ORDER_FIELD(Index, new IntAccessor_(&tdb_result_type::nIndex));
				ORDER_FIELD(OrderId, (new StringAccessor_<int, util::StringizeEncoder>(&tdb_result_type::nOrder)));
				ORDER_FIELD(OrderKind, new CharAccessor_(&tdb_result_type::chOrderKind));
				ORDER_FIELD(FunctionCode, new CharAccessor_(&tdb_result_type::chFunctionCode));
				ORDER_FIELD(OrderPrice, new FloatAccessor_<int>(&tdb_result_type::nOrderPrice, .0001));
				ORDER_FIELD(OrderVolume, new FloatAccessor_<int>(&tdb_result_type::nOrderVolume));
			}
		};
	};

	// Heper to assist symbolic retrieval of tick data fields from TDBDefine_OrderQueue.
	struct OrderQueue {

		// Data fields -- always keep in sync with TDBDefine_OrderQueue
		enum Field {
			NIL = 0,		//`NULL' for this enum
			WindCode,		//万得代码(AG1312.SHF)
			Code,			//交易所代码(ag1312)
			Date,			//日期（自然日）格式YYMMDD
			Time,			//时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
			Side,			//买卖方向('B':Bid 'A':Ask)
			Price,			//成交价格
			OrderItems,		//订单数量
			ABVolumes,		//订单明细
		};

		typedef ::TDBDefine_OrderQueue tdb_result_type;
		typedef FieldAccessor<tdb_result_type> field_accessor_type;
		typedef DateAccessor<tdb_result_type> DateAccessor_;
		typedef TimeAccessor<tdb_result_type> TimeAccessor_;
		typedef IntAccessor<tdb_result_type, I> IntAccessor_;
		template <typename Str> using SymbolAccessor_ = SymbolAccessor<tdb_result_type, Str>;
		template <typename Val> using FloatAccessor_ = FloatAccessor<tdb_result_type, Val>;
		
		struct SideAccessor_ : public AccessorBase<tdb_result_type, int> {
			SideAccessor_(field_accessor field)
				: AccessorBase<tdb_result_type, int>(field, KC) {}
		protected:
			virtual void setElement(K out, tdb_result_type const* dataArray, std::size_t index) const override;
		};

		template <typename Vals>
		struct VarFloatsAccessor_ : public AccessorBase<tdb_result_type, Vals> {
			VarFloatsAccessor_(field_accessor field, int const tdb_result_type::*count, double scalar = 1.)
				: AccessorBase<tdb_result_type, Vals>(field, 0), count_(count), scalar_(scalar) {}
		protected:
			virtual void setElement(K out, tdb_result_type const* dataArray, std::size_t index) const override;
		private:
			int const tdb_result_type::*count_;
			double const scalar_;
		};

		static std::map<Field, std::unique_ptr<field_accessor_type> > Accessors;

		// Data field names
		struct FieldName : public enum_util::EnumBase<FieldName, Field>
		{
			static void registerAll() {
#				define ORDERQUEUE_FIELD(name, accessor)	\
					ENUM_STRING(name);	\
					OrderQueue::Accessors.insert(std::make_pair(name, std::unique_ptr<field_accessor_type>(accessor)))

				ORDERQUEUE_FIELD(WindCode, new SymbolAccessor_<char[32]>(&tdb_result_type::chWindCode));
				ORDERQUEUE_FIELD(Code, new SymbolAccessor_<char[32]>(&tdb_result_type::chCode));
				ORDERQUEUE_FIELD(Date, new DateAccessor_(&tdb_result_type::nDate));
				ORDERQUEUE_FIELD(Time, new TimeAccessor_(&tdb_result_type::nTime));
				ORDERQUEUE_FIELD(Side, new SideAccessor_(&tdb_result_type::nSide));
				ORDERQUEUE_FIELD(Price, new FloatAccessor_<int>(&tdb_result_type::nPrice, .0001));
				ORDERQUEUE_FIELD(OrderItems, new IntAccessor_(&tdb_result_type::nOrderItems));
				ORDERQUEUE_FIELD(ABVolumes,
					new VarFloatsAccessor_<int[50]>(&tdb_result_type::nABVolume, &tdb_result_type::nABItems));
			}
		};
	};

}//namespace TDB


std::map<TDB::Order::Field, std::unique_ptr<TDB::Order::field_accessor_type> >
	TDB::Order::Accessors;

TDB_API K K_DECL TDB_order_fields(K _) {
	return TDB::getFields<TDB::Order>();
}

TDB_API K K_DECL TDB_order(K h, K windCode, K indicators, K begin, K end) {
	::THANDLE tdb = NULL;
	std::vector<TDB::Order::field_accessor_type const*> indis;
	::TDBDefine_ReqOrder req = { 0 };
	try {
		TDB::parseTdbHandle(h, tdb);
		TDB::parseIndicators<TDB::Order>(indicators, indis);
		TDB::parseTdbReq(windCode, begin, end, req);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	return TDB::runQuery<TDB::Order, ::TDBDefine_ReqOrder>(tdb, req, indis, &::TDB_GetOrder);
}


void TDB::OrderQueue::SideAccessor_::setElement(K out,
	TDB::OrderQueue::tdb_result_type const* dataArray, std::size_t index) const
{
	C& c = q::type_traits<C>::index(out)[index];
	switch (dataArray[index].*field_) {
	case 'A':
		c = 'A';
		break;
	case 'B':
		c = 'B';
		break;
	default:
		c = q::type_traits<C>::NULL_VAL;
	}
}

template <typename Vals>
void TDB::OrderQueue::VarFloatsAccessor_<Vals>::setElement(K out,
	typename TDB::OrderQueue::tdb_result_type const* dataArray, std::size_t index) const
{
	std::size_t const size = dataArray[index].*count_;
	assert(size <= (std::extent<Vals, 0>::value));
	K dst = q::type_traits<void>::index(out)[index] = ktn(KF, size);
	Vals const& src = dataArray[index].*field_;
	for (std::size_t j = 0; j < size; ++j) {
		kF(dst)[j] = scalar_ * src[j];
	}
}

std::map<TDB::OrderQueue::Field, std::unique_ptr<TDB::OrderQueue::field_accessor_type> >
	TDB::OrderQueue::Accessors;

TDB_API K K_DECL TDB_orderQueue_fields(K _) {
	return TDB::getFields<TDB::OrderQueue>();
}

TDB_API K K_DECL TDB_orderQueue(K h, K windCode, K indicators, K begin, K end) {
	::THANDLE tdb = NULL;
	std::vector<TDB::OrderQueue::field_accessor_type const*> indis;
	::TDBDefine_ReqOrderQueue req = { 0 };
	try {
		TDB::parseTdbHandle(h, tdb);
		TDB::parseIndicators<TDB::OrderQueue>(indicators, indis);
		TDB::parseTdbReq(windCode, begin, end, req);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	return TDB::runQuery<TDB::OrderQueue, ::TDBDefine_ReqOrderQueue>(tdb, req, indis, &::TDB_GetOrderQueue);
}