#ifndef __TDB_API_HELPER_H__
static_assert(0, "Include TDB_API/TDB_API_helper.h instead!");
#endif

#include "util.h"

#include "kdb+.util/type_convert.h"
#include <cassert>

template <typename FieldTraits>
void TDB::parseIndicators(K indicators, std::vector<typename FieldTraits::field_accessor const*>& accessors)
	throw(std::string)
{
	assert(accessors.empty());
	std::vector<std::string> const list = q::qList2String(indicators);
	accessors.reserve(list.size());
	for (auto i = list.cbegin(); i != list.cend(); ++i) {
		auto const accessor = (*FieldTraits::accessor_map::getInstance())[*i];
		assert(accessor != NULL);
		accessors.push_back(accessor);
	}
}

template <typename TdbReq>
void TDB::parseTdbReq(K windCode, K begin, K end, TdbReq& req) throw(std::string) {
	std::memset(&req, 0, sizeof(TdbReq));

	std::string const code = q::q2String(windCode);
	if (code.size() >= sizeof(req.chCode)) {
		throw std::string("windCode too long");
	}
	std::copy(code.begin(), code.end(), req.chCode);
	req.chCode[code.size()] = '\0';

	util::tm2DateTime(q::q2tm(begin), req.nBeginDate, req.nBeginTime);
	util::tm2DateTime(q::q2tm(end), req.nEndDate, req.nEndTime);
}

template <typename FieldTraits, typename TdbReq>
K TDB::runQuery(::THANDLE tdb, TdbReq const& req,
	std::vector<typename FieldTraits::field_accessor const*> const& indis,
	int(*tdbCall)(::THANDLE, TdbReq const*, typename FieldTraits::tdb_result_type**, int*))
{
	int arrayLen = 0;
	typename FieldTraits::tdb_result_type* dataArray = NULL;
	::TDB_ERROR const result = static_cast<::TDB_ERROR>(tdbCall(tdb, &req, &dataArray, &arrayLen));
	TDB::Ptr<typename FieldTraits::tdb_result_type> data(dataArray);
	if (result != TDB_SUCCESS) {
		return q::error2q(TDB::getError(result));
	}
	assert(arrayLen >= 0);
	assert(data);

	// Convert each requested field
	q::K_ptr out(ktn(0, indis.size()));
	for (size_t i = 0; i < indis.size(); ++i) {
		kK(out)[i] = indis[i]->extract(dataArray, arrayLen);
	}
	return out.release();
}