#ifndef __TDB_API_HELPER_H__
static_assert(0, "Include TDB_API/TDB_API_helper.h instead!");
#endif

#include "util.h"

#include "win32.util/StringUtil.h"
#include "kdb+.util/type_convert.h"
#include "Wind.util/FieldAccessors.h"
#include <cassert>

template <typename FieldTraits>
void TDB::parseIndicators(K indicators, std::vector<typename FieldTraits::field_accessor const*>& accessors)
throw(std::runtime_error)
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
void TDB::parseTdbReqCode(::THANDLE const tdb, K windCode, TdbReq& req) throw(std::runtime_error) {
	std::string const code = q::q2String(windCode);

	size_t const dot = code.rfind('.');
	if (dot == std::string::npos) {
		throw std::runtime_error("windCode missing suffix");
	}
	std::ostringstream buffer;
	buffer << std::string(code.cbegin() + dot + 1, code.cend())
		<< '-' << TDB::LEVELS[tdb]
		<< '-' << TDB::DATA_SRC;
	std::string const market = buffer.str();
	if (market.size() >= sizeof(req.chMarketKey)) {
		throw std::runtime_error("windCode suffix too long");
	}
	std::copy(market.begin(), market.end(), req.chMarketKey);
	req.chMarketKey[market.size()] = '\0';

	if (code.size() >= sizeof(req.chCode)) {
		throw std::runtime_error("windCode too long");
	}
	std::copy(code.begin(), code.end(), req.chCode);
	req.chCode[code.size()] = '\0';
}

template <typename TdbReq>
void TDB::parseTdbReqTime(K beginDT, K endDT, TdbReq& req) throw(std::runtime_error) {
	util::fillDateTime(beginDT, req.nBeginDate, req.nBeginTime);
	util::fillDateTime(endDT,   req.nEndDate,   req.nEndTime  );
}

template <typename TdbReq>
void TDB::parseTdbReqTime(K date, K begin, K end, TdbReq& req) throw(std::runtime_error) {
	util::fillDateTime(begin, req.nDate, req.nBeginTime);
	util::fillDateTime(end,   req.nDate, req.nEndTime  );
	int dummy;
	util::fillDateTime(date, req.nDate, dummy);
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

template <typename T, typename QType>
struct TDB::MarketInfoAccessor : public Wind::accessor::AccessorBase<T, char[24]>
{
	size_t partIndex_;

	MarketInfoAccessor(field_accessor field, size_t partIndex)
	: AccessorBase<struct_type, field_type>(q::type_traits<QType>::TYPE_NUM, field), partIndex_(partIndex) {}

	virtual void setElement(K out, size_t index, struct_type const& data) const override {
		std::vector<std::string> tokens = ::util::split(data.*(this->FIELD), '-');
		if (tokens.size() < partIndex_ + 1) {
			q::type_traits<QType>::index(out)[index] = q::type_traits<QType>::NULL_VAL;
		}
		else {
			long const value = std::strtol(tokens[partIndex_].c_str(), NULL, 10);
			q::type_traits<QType>::index(out)[index] = static_cast<QType>(value);
		}
	}
};

template <typename T>
struct TDB::MarketInfoAccessor<T, S> : public Wind::accessor::AccessorBase<T, char[24]>
{
	MarketInfoAccessor(field_accessor field)
	: AccessorBase<struct_type, field_type>(KS, field) {}

	virtual void setElement(K out, size_t index, struct_type const& data) const override {
		std::vector<std::string> tokens = ::util::split(data.*(this->FIELD), '-');
		if (tokens.empty()) {
			q::type_traits<S>::index(out)[index] = q::type_traits<S>::NULL_VAL;
		}
		else {
			q::type_traits<S>::index(out)[index] = ss(const_cast<S>(tokens[0].c_str()));
		}
	}
};