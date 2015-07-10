#include "stdafx.h"
#include "TDB_API.h"

#include "util.h"

#include "win32.util/EnumUtil.h"
#include "kdb+.util/K_ptr.h"
#include "kdb+.util/type_convert.h"

namespace TDB {

	// Heper to assist symbolic retrieval of tick data fields from TDBDefine_Transaction.
	struct Transaction {

		// Data fields -- always keep in sync with TDBDefine_Transaction
		enum Field {
			NIL = 0,		//`NULL' for this enum
			WindCode,		//万得代码(AG1312.SHF)
			Code,			//交易所代码(ag1312)
			Date,			//日期（自然日）格式YYMMDD
			Time,			//时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
			Index,			//成交编号(从1开始，递增1)
			FunctionCode,	//成交代码: 'C', 0
			OrderKind,		//委托类别
			BSFlag,			//BS标志
			TradePrice,		//成交价格
			TradeVolume,	//成交数量
			AskOrder,		//叫卖序号
			BidOrder,		//叫买序号
		};

		typedef FieldAccessor<::TDBDefine_Transaction> FieldAccessor_;
		typedef CharAccessor<::TDBDefine_Transaction> CharAccessor_;
		typedef DateAccessor<::TDBDefine_Transaction> DateAccessor_;
		typedef TimeAccessor<::TDBDefine_Transaction> TimeAccessor_;
		typedef IntAccessor<::TDBDefine_Transaction, I> IntAccessor_;
		template <typename Str> using SymbolAccessor_ = SymbolAccessor<::TDBDefine_Transaction, Str>;
		template <typename Val> using FloatAccessor_ = FloatAccessor<::TDBDefine_Transaction, Val>;

		static std::map<Field, std::unique_ptr<FieldAccessor_> > accessors_;

		// Data field names
		struct FieldName :
			public enum_util::EnumBase<FieldName, Field>
		{
			static void registerAll() {
#				define TRANSACTION_FIELD(name, accessor)	\
					ENUM_STRING(name);	\
					Transaction::accessors_.insert(std::make_pair(name, std::unique_ptr<FieldAccessor_>(accessor)))

				TRANSACTION_FIELD(WindCode, new SymbolAccessor_<char[32]>(&::TDBDefine_Transaction::chWindCode));
				TRANSACTION_FIELD(Code, new SymbolAccessor_<char[32]>(&::TDBDefine_Transaction::chCode));
				TRANSACTION_FIELD(Date, new DateAccessor_(&::TDBDefine_Transaction::nDate));
				TRANSACTION_FIELD(Time, new TimeAccessor_(&::TDBDefine_Transaction::nTime));
				TRANSACTION_FIELD(Index, new IntAccessor_(&::TDBDefine_Transaction::nIndex));
				TRANSACTION_FIELD(FunctionCode, new CharAccessor_(&::TDBDefine_Transaction::chFunctionCode));
				TRANSACTION_FIELD(OrderKind, new CharAccessor_(&::TDBDefine_Transaction::chOrderKind));
				TRANSACTION_FIELD(BSFlag, new CharAccessor_(&::TDBDefine_Transaction::chBSFlag));
				TRANSACTION_FIELD(TradePrice, new FloatAccessor_<int>(&::TDBDefine_Transaction::nTradePrice, .0001));
				TRANSACTION_FIELD(TradeVolume, new FloatAccessor_<int>(&::TDBDefine_Transaction::nTradeVolume));
				TRANSACTION_FIELD(AskOrder, new IntAccessor_(&::TDBDefine_Transaction::nAskOrder));
				TRANSACTION_FIELD(BidOrder, new IntAccessor_(&::TDBDefine_Transaction::nBidOrder));
			}
		};
	};

}//namespace TDB


std::map<TDB::Transaction::Field, std::unique_ptr<TDB::Transaction::FieldAccessor_> >
TDB::Transaction::accessors_;

TDB_API K K_DECL TDB_transaction_fields(K _) {
	std::vector<std::string> const fieldNames = TDB::Transaction::FieldName::getAllStrings();
	q::K_ptr result(ktn(KS, fieldNames.size()));
	for (std::size_t i = 0; i < fieldNames.size(); ++i) {
		kS(result.get())[i] = ss(const_cast<S>(fieldNames[i].c_str()));
	}
	return result.release();
}

TDB_API K K_DECL TDB_transaction(K h, K windCode, K indicators, K begin, K end) {
	::THANDLE tdb = NULL;
	std::vector<std::string> indis;
	::TDBDefine_ReqTransaction req = { 0 };
	try {
		tdb = reinterpret_cast<::THANDLE>(q::q2Dec(h));

		std::string const code = q::q2String(windCode);
		std::copy(code.begin(), code.end(), req.chCode);
		req.chCode[code.size()] = '\0';

		indis = q::qList2String(indicators);

		TDB::util::tm2DateTime(q::q2tm(begin), req.nBeginDate, req.nBeginTime);
		TDB::util::tm2DateTime(q::q2tm(end), req.nEndDate, req.nEndTime);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}
	if (!tdb) {
		return q::error2q("null THANDLE");
	}

	// Get all requested field accessors
	std::vector<TDB::Transaction::FieldAccessor_ const*> fields;
	fields.reserve(indis.size());
	for (auto i = indis.cbegin(); i != indis.cend(); ++i) {
		auto const f = TDB::Transaction::FieldName::fromString(*i);
		if (f == TDB::Transaction::NIL) {
			return q::error2q(*i);
		}
		fields.push_back(TDB::Transaction::accessors_[f].get());
	}

	// Query
	int txnCount = 0;
	::TDBDefine_Transaction* t = NULL;
	int const result = ::TDB_GetTransaction(tdb, &req, &t, &txnCount);
	TDB::Ptr<::TDBDefine_Transaction> txns(t);
	if (result != TDB_SUCCESS) {
		return q::error2q(TDB::getError(result));
	}
	assert(txnCount >= 0);
	assert(txns);

	// Convert each requested field
	q::K_ptr data(ktn(0, fields.size()));
	for (std::size_t i = 0; i < fields.size(); ++i) {
		kK(data.get())[i] = fields[i]->extract(txns.get(), txnCount);
	}
	return data.release();
}