#include "stdafx.h"
#include "TDB_API.h"

#include "TDB_API_helper.h"

#include <algorithm>
#include <locale>
#include "win32.util/Singleton.h"
#include "win32.util/EnumUtil.h"
#include "kdb+.util/util.h"
#include "kdb+.util/K_ptr.h"
#include "kdb+.util/type_convert.h"
#include "Wind.util/FieldMapper.h"

BEGIN_ENUM_STRING(::REFILLFLAG)
{
	ENUM_STRING2(::REFILL_NONE, "");
	ENUM_STRING2(::REFILL_BACKWARD, "B");
	ENUM_STRING2(::REFILL_FORWARD, "F");
}
END_ENUM_STRING;

BEGIN_ENUM_STRING(::CYCTYPE)
{
	ENUM_STRING2(::CYC_SECOND, "V");
	ENUM_STRING2(::CYC_MINUTE, "U");
	ENUM_STRING2(::CYC_DAY, "D");
	ENUM_STRING2(::CYC_WEEK, "W");
	ENUM_STRING2(::CYC_MONTH, "M");
	ENUM_STRING2(::CYC_SEASON, "Q");
	ENUM_STRING2(::CYC_HAFLYEAR, "H");
	ENUM_STRING2(::CYC_YEAR, "Y");
	ENUM_STRING2(::CYC_TICKBAR, "T");
}
END_ENUM_STRING;

namespace TDB {
	namespace Enum {
		//ծȯȫ�۱�־
		enum PriceType {
			Net  = 0,	//����
			Full = 1	//ȫ��
		};

		//�Զ������־
		enum AutoFill {
			NoFill = 0,
			ToFill = 1
		};
	}
}
BEGIN_ENUM_STRING(TDB::Enum::PriceType)
{
	ENUM_STRING2(TDB::Enum::Net,  "NET" );
	ENUM_STRING2(TDB::Enum::Full, "FULL");
}
END_ENUM_STRING;
BEGIN_ENUM_STRING(TDB::Enum::AutoFill)
{
	ENUM_STRING2(TDB::Enum::NoFill, "0");
	ENUM_STRING2(TDB::Enum::NoFill, "F");
	ENUM_STRING2(TDB::Enum::NoFill, "N");
	ENUM_STRING2(TDB::Enum::ToFill, "1");
	ENUM_STRING2(TDB::Enum::ToFill, "T");
	ENUM_STRING2(TDB::Enum::ToFill, "Y");
}
END_ENUM_STRING;

namespace TDB {
	namespace traits {

		// Traits to assist symbolic retrieval of OHLC data fields from TDBDefine_KLine.
		struct OHLC : public Wind::mapper::Fields<::TDBDefine_KLine> {
			typedef Singleton<OHLC> accessor_map;
			typedef ::TDBDefine_KLine tdb_result_type;

			OHLC() : Wind::mapper::Fields<tdb_result_type>() { registerAllFields(); }

			typedef Wind::accessor::SymbolAccessor<tdb_result_type, char[32]> SymbolAccessor;
			typedef Wind::accessor::DateAccessor<tdb_result_type> DateAccessor;
			typedef Wind::accessor::TimeAccessor<tdb_result_type> TimeAccessor;
			typedef Wind::accessor::IntAccessor<tdb_result_type, I> IntAccessor;
			typedef Wind::accessor::CharAccessor<tdb_result_type> CharAccessor;
			template <typename FieldT>
			using FloatAccessor = Wind::accessor::FloatAccessor<tdb_result_type, FieldT>;
			template <typename FieldT>
			using FloatsAccessor = Wind::accessor::FloatsAccessor<tdb_result_type, FieldT>;

			// Data fields -- always keep in sync with TDBDefine_KLine
			void registerAllFields() {
				// ��ô���(AG1312.SHF)
				addField("WindCode", new SymbolAccessor(&tdb_result_type::chWindCode));
				// ����������(ag1312)
				addField("Code", new SymbolAccessor(&tdb_result_type::chCode));
				// ���ڣ���Ȼ�գ���ʽYYMMDD
				addField("Date", new DateAccessor(&tdb_result_type::nDate));
				// ʱ�䣨HHMMSSmmm������94500000 ��ʾ 9��45��00��000����
				addField("Time", new TimeAccessor(&tdb_result_type::nTime));
				// ����
				addField("Open", new FloatAccessor<int>(&tdb_result_type::nOpen, .0001));
				// ���
				addField("High", new FloatAccessor<int>(&tdb_result_type::nHigh, .0001));
				// ���
				addField("Low", new FloatAccessor<int>(&tdb_result_type::nLow, .0001));
				// ����
				addField("Close", new FloatAccessor<int>(&tdb_result_type::nClose, .0001));
				// �ɽ�����
				addField("Size", new FloatAccessor<__int64>(&tdb_result_type::iVolume));
				// �ɽ���(Ԫ)
				addField("Amount", new FloatAccessor<__int64>(&tdb_result_type::iTurover));
				// �ɽ�����
				addField("MatchCount", new IntAccessor(&tdb_result_type::nMatchItems));
				// �ֲ���(�ڻ�)��IOPV(����)����Ϣ(ծȯ)
				addField("Interest", new FloatAccessor<int>(&tdb_result_type::nInterest));
			}
		};
	}//namespace TDB::traits

	void parseTdbOHLCReqCycle(K cycle, ::TDBDefine_ReqKLine& req) throw(std::runtime_error) {
		if (cycle == K_NIL) {
			throw std::runtime_error("nil cycle spec");
		}

		switch (cycle->t) {
		case -KV:
			req.nCycType = ::CYC_SECOND;
			req.nCycDef = cycle->i;
			return;
		case -KU:
			req.nCycType = ::CYC_MINUTE;
			req.nCycDef = cycle->i;
			return;
		default:
			;//fall-through
		}
		std::string const cycleDef = q::q2String(cycle);

		char const cycleType[2] = { std::toupper(*cycleDef.crbegin(), std::locale()), '\0' };
		if (!::util::Enum<::CYCTYPE>::isValidString(cycleType)) {
			throw std::runtime_error("invalid cycle type");
		}
		req.nCycType = ::util::Enum<::CYCTYPE>::fromString(cycleType);

		std::stringstream buffer(cycleDef);
		buffer >> req.nCycDef;
	}

	void parseTdbOHLCReqParams(K params, ::TDBDefine_ReqKLine& req) throw(std::runtime_error) {
		if (params == K_NIL) {
			throw std::runtime_error("nil dict");
		}

		std::vector<std::string> keys;
		switch (params->t) {
		case 0:
			if (params->n != 0) {
				throw std::runtime_error("not a dict or empty list");
			}
			return;	//nothing to process
		case XT:
			return parseTdbOHLCReqParams(params->k, req);
		case XD:
			//expected data type, to be processed below...
			break;
		default:
			throw std::runtime_error("not a dict");
		}

		assert(params->n == 2);
#		pragma warning(disable: 6201)	//QUIRK: k.h k0::G0 type
		keys = q::qList2String(kK(params)[0]);
#		pragma warning(default: 6201)
		std::transform(keys.begin(), keys.end(), keys.begin(), [](std::string& key) {
			std::transform(key.begin(), key.end(), key.begin(),
				[](char c) { return std::toupper(c, std::locale()); });
			return key;
		});

#		pragma warning(disable: 6201)	//QUIRK: k.h k0::G0 type
		assert(keys.size() == kK(params)[1]->n);
		K const values = kK(params)[1];
#		pragma warning(default: 6201)
		for (size_t i = 0; i < keys.size(); ++i) {
			if (keys[i] == "PRICEADJ") {
				std::string priceAdj;
				switch (values->t) {
				case 0:
					priceAdj = q::q2String(kK(values)[i]);
					break;
				case KS:
					priceAdj = kS(values)[i];
					break;
				case KC:
					priceAdj = " ";
					priceAdj[0] = kC(values)[i];
					break;
				default:
					throw std::runtime_error("invalid parameter PRICEADJ");
				}
				std::transform(priceAdj.begin(), priceAdj.end(), priceAdj.begin(),
					[](char c) { return std::toupper(c, std::locale()); });
				if (!::util::Enum<::REFILLFLAG>::isValidString(priceAdj)) {
					throw std::runtime_error("incorrect parameter PRICEADJ");
				}
				else {
					req.nCQFlag = ::util::Enum<::REFILLFLAG>::fromString(priceAdj);
				}
			}
			else if (keys[i] == "PRICEADJDATE") {
				int time;
				q::K_ptr d;
				switch (values->t) {
				case 0:
					util::tm2DateTime(q::q2tm(kK(values)[i]), req.nCQDate, time);
					break;
				case KD:
					d.reset(kd(kI(values)[i]));
					util::tm2DateTime(q::q2tm(d.get()), req.nCQDate, time);
					break;
				default:
					throw std::runtime_error("invalid parameter PRICEADJDATE");
				}
			}
			else if (keys[i] == "PRICETYPE") {
				std::string priceType;
				switch (values->t) {
				case 0:
					priceType = q::q2String(kK(values)[i]);
					break;
				case KS:
					priceType = kS(values)[i];
					break;
				case KC:
					priceType = " ";
					priceType[0] = kC(values)[i];
					break;
				default:
					throw std::runtime_error("invalid parameter PRICETYPE");
				}
				std::transform(priceType.begin(), priceType.end(), priceType.begin(),
					[](char c) { return std::toupper(c, std::locale()); });
				if (!::util::Enum<TDB::Enum::PriceType>::isValidString(priceType)) {
					throw std::runtime_error("incorrect parameter PRICETYPE");
				}
				else {
					req.nQJFlag = ::util::Enum<TDB::Enum::PriceType>::fromString(priceType);
				}
			}
			else if (keys[i] == "FILL") {
				std::string fill;
				switch (values->t) {
				case 0:
					fill = q::q2String(kK(values)[i]);
					break;
				case KS:
					fill = kS(values)[i];
					break;
				case KC:
					fill = " ";
					fill[0] = kC(values)[i];
					break;
				case KB:
					fill = !!kG(values)[i] ? "1" : "0";
					break;
				default:
					throw std::runtime_error("invalid parameter FILL");
				}
				std::transform(fill.begin(), fill.end(), fill.begin(),
					[](char c) { return std::toupper(c, std::locale()); });
				if (!::util::Enum<TDB::Enum::AutoFill>::isValidString(fill)) {
					throw std::runtime_error("incorrect parameter FILL");
				}
				else {
					req.nAutoComplete = ::util::Enum<TDB::Enum::AutoFill>::fromString(fill);
				}
			}
		}
	}

}//namespace TDB

TDB_API K K_DECL TDB_ohlc_fields(K _) {
	return TDB::traits::OHLC::accessor_map::getInstance()->getFields();
}

TDB_API K K_DECL TDB_ohlc(K h, K windCode, K indicators, K beginDT, K endDT, K cycle, K params) {
	::THANDLE tdb = NULL;
	std::vector<TDB::traits::OHLC::field_accessor const*> indis;
	::TDBDefine_ReqKLine req = { 0 };
	try {
		TDB::parseTdbHandle(h, tdb);
		TDB::parseIndicators<TDB::traits::OHLC>(indicators, indis);
		TDB::parseTdbReqCode(tdb, windCode, req);
		TDB::parseTdbReqTime(beginDT, endDT, req);
		TDB::parseTdbOHLCReqCycle(cycle, req);
		TDB::parseTdbOHLCReqParams(params, req);
	}
	catch (std::runtime_error const& error) {
		return q::error2q(error.what());
	}

	return TDB::runQuery<TDB::traits::OHLC, ::TDBDefine_ReqKLine>(tdb, req, indis, &::TDB_GetKLine);
}