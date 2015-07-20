#ifndef __TDF_API_HELPER_H__
#define __TDF_API_HELPER_H__

#include "win32.util/Singleton.h"
#include "kdb+.util/K_ptr.h"
#include "kdb+.util/type_convert.h"
#include "Wind.util/FieldMapper.h"

#ifdef _MSC_VER
//@ref https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(C4290);k(VS.ErrorList)&rd=true
#pragma warning(disable: 4290)
#endif

namespace TDF {

	void parseTdfHandle(K h, ::THANDLE& tdf) throw(std::string);

	namespace accessor {
		
		template <typename T>
		struct CodeInfoAccessor : public Wind::accessor::NestedAccessor<T, ::TDF_CODE_INFO*> {
			typedef Wind::accessor::NestedAccessor<T, ::TDF_CODE_INFO*> base_type;
			CodeInfoAccessor(typename base_type::field_accessor_type* field)
				: base_type(field, &T::pCodeInfo) {}
		};

		typedef Wind::accessor::IntAccessor<::TDF_CODE_INFO, G> CodeTypeAccessor;
		typedef Wind::accessor::SymbolAccessor<::TDF_CODE_INFO, char[64], Wind::encoder::GB18030_UTF8> CodeNameAccessor;

		template <typename T>
		struct ExCodeInfoAccessor : public Wind::accessor::NestedAccessor<T, ::TDF_CODE_INFO*, ::TD_EXCODE_INFO> {
			typedef Wind::accessor::NestedAccessor<T, ::TDF_CODE_INFO*, ::TD_EXCODE_INFO> base_type;
			ExCodeInfoAccessor(typename base_type::field_accessor_type* field)
				: base_type(field, &T::pCodeInfo, &::TDF_CODE_INFO::exCodeInfo) {}
		};

		typedef Wind::accessor::DateAccessor<::TD_EXCODE_INFO> ExCodeDateAccessor;
		typedef Wind::accessor::CharAccessor<::TD_EXCODE_INFO> ExCodeCharAccessor;
		typedef Wind::accessor::IntAccessor<::TD_EXCODE_INFO, I> ExCodeIntAccessor;
		typedef Wind::accessor::FloatAccessor<::TD_EXCODE_INFO, int> ExCodeFloatAccessor;
		typedef Wind::accessor::SymbolAccessor<::TD_EXCODE_INFO, char[32]> ExCodeSymbolAccessor;

	}//namespace TDF::accessor

	namespace traits {

		// Traits to assist symbolic retrieval of tick data fields from TDF_INDEX_DATA.
		struct Index : public Wind::mapper::Fields<::TDF_INDEX_DATA> {
			typedef Singleton<Index> accessor_map;
			typedef ::TDF_INDEX_DATA tdf_result_type;

			Index() : Wind::mapper::Fields<tdf_result_type>() { registerAllFields(); }

			typedef Wind::accessor::SymbolAccessor<tdf_result_type, char[32]> SymbolAccessor;
			typedef Wind::accessor::DateAccessor<tdf_result_type> DateAccessor;
			typedef Wind::accessor::TimeAccessor<tdf_result_type> TimeAccessor;
			template <typename FieldT>
			using FloatAccessor = Wind::accessor::FloatAccessor<tdf_result_type, FieldT>;

			typedef TDF::accessor::CodeInfoAccessor<tdf_result_type> CodeInfoAccessor;
			typedef TDF::accessor::CodeTypeAccessor CodeTypeAccessor;
			typedef TDF::accessor::CodeNameAccessor CodeNameAccessor;

			// Data fields -- always keep in sync with TDF_INDEX_DATA
			void registerAllFields() {
				// 600001.SH
				addField("WindCode", new SymbolAccessor(&tdf_result_type::szWindCode));
				// 原始Code
				addField("Code", new SymbolAccessor(&tdf_result_type::szCode));
				// 业务发生日(自然日)
				addField("Date", new DateAccessor(&tdf_result_type::nActionDay));
				// 交易日
				addField("TradeDate", new DateAccessor(&tdf_result_type::nTradingDay));
				// 时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
				addField("Time", new TimeAccessor(&tdf_result_type::nTime));
				// 今开盘指数
				addField("Open", new FloatAccessor<int>(&tdf_result_type::nOpenIndex));
				// 最高指数
				addField("High", new FloatAccessor<int>(&tdf_result_type::nHighIndex));
				// 最低指数
				addField("Low", new FloatAccessor<int>(&tdf_result_type::nLowIndex));
				// 最新指数
				addField("Last", new FloatAccessor<int>(&tdf_result_type::nLastIndex));
				// 参与计算相应指数的交易数量
				addField("Volume", new FloatAccessor<__int64>(&tdf_result_type::iTotalVolume));
				// 参与计算相应指数的成交金额
				addField("Turnover", new FloatAccessor<__int64>(&tdf_result_type::iTurnover, .01));
				// 前盘指数
				addField("PreClose", new FloatAccessor<int>(&tdf_result_type::nPreCloseIndex));
				//（TDF_CODE_INFO）证券类型
				addField("CodeType", new CodeInfoAccessor(new CodeTypeAccessor(&::TDF_CODE_INFO::nType)));
				//（TDF_CODE_INFO）汉语证券名称
				addField("CodeName", new CodeInfoAccessor(new CodeNameAccessor(&::TDF_CODE_INFO::chName)));
			}
		};

		// Traits to assist symbolic retrieval of tick data fields from TDF_MARKET_DATA.
		struct Market : public Wind::mapper::Fields<::TDF_MARKET_DATA> {
			typedef Singleton<Index> accessor_map;
			typedef ::TDF_MARKET_DATA tdf_result_type;

			Market() : Wind::mapper::Fields<tdf_result_type>() { registerAllFields(); }

			typedef Wind::accessor::SymbolAccessor<tdf_result_type, char[32]> SymbolAccessor;
			typedef Wind::accessor::DateAccessor<tdf_result_type> DateAccessor;
			typedef Wind::accessor::TimeAccessor<tdf_result_type> TimeAccessor;
			template <typename FieldT>
			using FloatAccessor = Wind::accessor::FloatAccessor<tdf_result_type, FieldT>;
			template <typename FieldT>
			using FloatAccessors = Wind::accessor::FloatsAccessor<tdf_result_type, FieldT>;

			typedef TDF::accessor::CodeInfoAccessor<tdf_result_type> CodeInfoAccessor;
			typedef TDF::accessor::CodeTypeAccessor CodeTypeAccessor;
			typedef TDF::accessor::CodeNameAccessor CodeNameAccessor;

			// Data fields -- always keep in sync with TDF_MARKET_DATA
			void registerAllFields() {
				// 600001.SH
				addField("WindCode", new SymbolAccessor(&tdf_result_type::szWindCode));
				// 原始Code
				addField("Code", new SymbolAccessor(&tdf_result_type::szCode));
				// 业务发生日(自然日)
				addField("Date", new DateAccessor(&tdf_result_type::nActionDay));
				// 交易日
				addField("TradeDate", new DateAccessor(&tdf_result_type::nTradingDay));
				// 时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
				addField("Time", new TimeAccessor(&tdf_result_type::nTime));
				/*
				int			 nStatus;				//状态

				// 前收盘价
				addField("PreClose", new FloatAccessor<unsigned int>(&tdf_result_type::nPreClose));

				unsigned int nOpen;					//开盘价
				unsigned int nHigh;					//最高价
				unsigned int nLow;					//最低价
				unsigned int nMatch;				//最新价
				unsigned int nAskPrice[10];			//申卖价
				unsigned int nAskVol[10];			//申卖量
				unsigned int nBidPrice[10];			//申买价
				unsigned int nBidVol[10];			//申买量
				unsigned int nNumTrades;			//成交笔数
				__int64		 iVolume;				//成交总量
				__int64		 iTurnover;				//成交总金额
				__int64		 nTotalBidVol;			//委托买入总量
				__int64		 nTotalAskVol;			//委托卖出总量
				unsigned int nWeightedAvgBidPrice;	//加权平均委买价格
				unsigned int nWeightedAvgAskPrice;  //加权平均委卖价格
				int			 nIOPV;					//IOPV净值估值
				int			 nYieldToMaturity;		//到期收益率
				unsigned int nHighLimited;			//涨停价
				unsigned int nLowLimited;			//跌停价
				char		 chPrefix[4];			//证券信息前缀
				int			 nSyl1;					//市盈率1
				int			 nSyl2;					//市盈率2
				int			 nSD2;					//升跌2（对比上一笔）


				// 今开盘指数
				addField("Open", new FloatAccessor<int>(&tdf_result_type::nOpenIndex));
				// 最高指数
				addField("High", new FloatAccessor<int>(&tdf_result_type::nHighIndex));
				// 最低指数
				addField("Low", new FloatAccessor<int>(&tdf_result_type::nLowIndex));
				// 最新指数
				addField("Last", new FloatAccessor<int>(&tdf_result_type::nLastIndex));
				// 参与计算相应指数的交易数量
				addField("Volume", new FloatAccessor<__int64>(&tdf_result_type::iTotalVolume));
				// 参与计算相应指数的成交金额
				addField("Turnover", new FloatAccessor<__int64>(&tdf_result_type::iTurnover));
				// 前盘指数
				addField("PreClose", new FloatAccessor<int>(&tdf_result_type::nPreCloseIndex));
				//（TDF_CODE_INFO）证券类型
				addField("CodeType", new CodeInfoAccessor(new CodeTypeAccessor(&::TDF_CODE_INFO::nType)));
				//（TDF_CODE_INFO）汉语证券名称
				addField("CodeName", new CodeInfoAccessor(new CodeNameAccessor(&::TDF_CODE_INFO::chName)));
				*/
			}
		};

	}//nmespace TDF::traits

	/*
	namespace traits {

		// Traits to assist symbolic retrieval of tick data fields from TD_EXCODE_INFO::TD_OptionCodeInfo.
		struct OptionCodeInfo : public Wind::mapper::Fields<::TD_EXCODE_INFO::TD_OptionCodeInfo> {
			typedef Singleton<OptionCodeInfo> accessor_map;
			typedef ::TD_EXCODE_INFO::TD_OptionCodeInfo tdf_result_type;

			OptionCodeInfo() : Wind::mapper::Fields<tdf_result_type>() { registerAllFields(); }

			typedef Wind::accessor::SymbolAccessor<tdf_result_type, char[32]> SymbolAccessor;
			typedef Wind::accessor::DateAccessor<tdf_result_type> DateAccessor;
			typedef Wind::accessor::IntAccessor<tdf_result_type, I> IntAccessor;
			typedef Wind::accessor::CharAccessor<tdf_result_type> CharAccessor;
			typedef Wind::accessor::FloatAccessor<tdf_result_type, int> FloatAccessor;

			// Data fields -- always keep in sync with TD_EXCODE_INFO::TD_OptionCodeInfo
			void registerAllFields() {
				//（Option）期权合约代码C19
				addField("Contract", new SymbolAccessor(&tdf_result_type::chContractID));
				//（Option）标的证券代码
				addField("Underlying", new SymbolAccessor(&tdf_result_type::szUnderlyingSecurityID));
				//（Option）标的证券类型C3    0-A股 1-ETF (EBS – ETF， ASH – A 股)
				addField("UnderlyingType", new CharAccessor(&tdf_result_type::chUnderlyingType));
				//（Option）欧式美式C1        若为欧式期权，则本字段为“E”；若为美式期权，则本字段为“A”
				addField("OptionType", new CharAccessor(&tdf_result_type::chOptionType));
				//（Option）认购认沽C1        认购，则本字段为“C”；若为认沽，则本字段为“P”
				addField("CallPut", new CharAccessor(&tdf_result_type::chCallOrPut));
				//（Option）涨跌幅限制类型C1 ‘N’表示有涨跌幅限制类型, ‘R’表示无涨跌幅限制类型
				addField("PriceLimitType", new CharAccessor(&tdf_result_type::chPriceLimitType));
				//（Option）合约单位,         经过除权除息调整后的合约单位, 一定是整数
				addField("Multiplier", new IntAccessor(&tdf_result_type::nContractMultiplierUnit));
				//（Option）期权行权价,       经过除权除息调整后的期权行权价，右对齐，精确到厘
				addField("ExePrice", new FloatAccessor(&tdf_result_type::nExercisePrice, .001));
				//（Option）期权首个交易日,YYYYMMDD
				addField("StartDate", new DateAccessor(&tdf_result_type::nStartDate));
				//（Option）期权最后交易日/行权日，YYYYMMDD
				addField("EndDate", new DateAccessor(&tdf_result_type::nEndDate));
				//（Option）期权行权日，YYYYMMDD
				addField("ExeDate", new DateAccessor(&tdf_result_type::nExerciseDate));
				//（Option）期权到期日，YYYYMMDD
				addField("ExpDate", new DateAccessor(&tdf_result_type::nExpireDate));
			}
		};

		// Traits to assist symbolic retrieval of tick data fields from TD_EXCODE_INFO.
		struct ExCodeInfo : public Wind::mapper::Fields<::TD_EXCODE_INFO> {
			typedef Singleton<ExCodeInfo> accessor_map;
			typedef ::TD_EXCODE_INFO tdf_result_type;

			ExCodeInfo() : Wind::mapper::Fields<tdf_result_type>() { registerAllFields(); }

			// Data fields -- always keep in sync with TD_EXCODE_INFO
			void registerAllFields() {
				addField("Option",
					new Wind::accessor::NestedAccessor<tdf_result_type, OptionCodeInfo::tdf_result_type>(
						&tdf_result_type::Option));
			}
		};

		// Traits to assist symbolic retrieval of tick data fields from TDF_CODE_INFO.
		struct CodeInfo : public Wind::mapper::Fields<::TDF_CODE_INFO> {
			typedef Singleton<CodeInfo> accessor_map;
			typedef ::TDF_CODE_INFO tdf_result_type;

			CodeInfo() : Wind::mapper::Fields<tdf_result_type>() { registerAllFields(); }

			typedef Wind::accessor::IntAccessor<tdf_result_type, G> TypeAccessor;
			template <typename FieldT, typename Encoder = Wind::encoder::Passthrough>
			using SymbolAccessor = Wind::accessor::SymbolAccessor<tdf_result_type, FieldT, Encoder>;
			typedef Wind::accessor::NestedAccessor<tdf_result_type, OptionCodeInfo> NestedAccessor;

			// Data fields -- always keep in sync with TDF_CODE_INFO
			void registerAllFields() {
				// 证券类型
				addField("Type", new TypeAccessor(&tdf_result_type::nType));
				// 证券代码
				addField("Code", new SymbolAccessor<char[32]>(&tdf_result_type::chCode));
				// 汉语证券名称
				addField("Name", new SymbolAccessor<char[64], Wind::encoder::GB18030_UTF8>(&tdf_result_type::chName));
				//>> OptionCodeInfo
				//OptionCodeInfo const* optionCode = OptionCodeInfo::accessor_map::getInstance();
				/*
				addField("Contract", new NestedAccessor(&tdf_result_type::chContractID));
				addField("Underlying", new SymbolAccessor(&tdf_result_type::szUnderlyingSecurityID));
				addField("UnderlyingType", new CharAccessor(&tdf_result_type::chUnderlyingType));
				addField("OptionType", new CharAccessor(&tdf_result_type::chOptionType));
				addField("CallPut", new CharAccessor(&tdf_result_type::chCallOrPut));
				addField("PriceLimitType", new CharAccessor(&tdf_result_type::chPriceLimitType));
				addField("Multiplier", new IntAccessor(&tdf_result_type::nContractMultiplierUnit));
				addField("ExePrice", new FloatAccessor(&tdf_result_type::nExercisePrice, .001));
				addField("StartDate", new DateAccessor(&tdf_result_type::nStartDate));
				addField("EndDate", new DateAccessor(&tdf_result_type::nEndDate));
				addField("ExeDate", new DateAccessor(&tdf_result_type::nExerciseDate));
				addField("ExpDate", new DateAccessor(&tdf_result_type::nExpireDate));
				* /
			}
		};

	}//namespace TDB::traits
	*/
}//naemspace TDF

#endif//__TDF_API_HELPER_H__