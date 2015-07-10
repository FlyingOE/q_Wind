#ifndef __TDB_API_HELPER_H__
static_assert(0, "Include TDB_API/TDB_API_helper.h instead!");
#endif

#include "util.h"

#include <cassert>

template <typename TDBDefine_Wrap>
void TDB::parseIndicators(K indicators,
	std::vector<typename TDBDefine_Wrap::field_accessor_type const*>& indis)
	throw(std::string)
{
	assert(indis.empty());
	std::vector<std::string> const is = q::qList2String(indicators);
	indis.reserve(is.size());
	for (auto i = is.cbegin(); i != is.cend(); ++i) {
		auto const f = TDBDefine_Wrap::FieldName::fromString(*i);
		if (f == TDBDefine_Wrap::NIL) {
			throw *i;
		}
		indis.push_back(TDBDefine_Wrap::Accessors[f].get());
		assert(*indis.rbegin() != NULL);
	}
}

template <typename TDBDefine_ReqT>
void TDB::parseTdbReq(K windCode, K begin, K end, TDBDefine_ReqT& req) throw(std::string) {
	std::memset(&req, 0, sizeof(TDBDefine_ReqT));

	std::string const code = q::q2String(windCode);
	std::copy(code.begin(), code.end(), req.chCode);
	req.chCode[code.size()] = '\0';

	util::tm2DateTime(q::q2tm(begin), req.nBeginDate, req.nBeginTime);
	util::tm2DateTime(q::q2tm(end), req.nEndDate, req.nEndTime);
}

template <typename TDBDefine_Wrap>
K TDB::getFields() {
	std::vector<std::string> const fieldNames = TDBDefine_Wrap::FieldName::getAllStrings();
	q::K_ptr result(ktn(KS, fieldNames.size()));
	for (std::size_t i = 0; i < fieldNames.size(); ++i) {
		kS(result.get())[i] = ss(const_cast<S>(fieldNames[i].c_str()));
	}
	return result.release();
}

template <typename TDBDefine_Wrap, typename TDBDefine_ReqT>
K TDB::runQuery(::THANDLE tdb, TDBDefine_ReqT const& req,
	std::vector<typename TDBDefine_Wrap::field_accessor_type const*>& indis,
	int(*tdbCall)(::THANDLE, TDBDefine_ReqT const*, typename TDBDefine_Wrap::tdb_result_type**, int*))
{
	int arrayLen = 0;
	typename TDBDefine_Wrap::tdb_result_type* dataArray = NULL;
	int const result = tdbCall(tdb, &req, &dataArray, &arrayLen);
	TDB::Ptr<typename TDBDefine_Wrap::tdb_result_type> data(dataArray);
	if (result != TDB_SUCCESS) {
		return q::error2q(TDB::getError(result));
	}
	assert(arrayLen >= 0);
	assert(data);

	// Convert each requested field
	q::K_ptr out(ktn(0, indis.size()));
	for (std::size_t i = 0; i < indis.size(); ++i) {
		kK(out)[i] = indis[i]->extract(dataArray, arrayLen);
	}
	return out.release();
}