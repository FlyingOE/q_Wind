#ifndef __TDB_API_HELPER_H__
static_assert(0, "Include TDB_API/TDB_API_helper.h instead!");
#endif

#include "util.h"

#include "kdb+.util/type_convert.h"
#include <cassert>

#pragma region

template <typename TdbT>
K TDB::FieldAccessor<TdbT>::extract(TdbT const* dataArray, std::size_t arrayLen) const {
	assert(NULL != dataArray);
	assert(arrayLen >= 0);
	q::K_ptr result(ktn(getTypeNum(), arrayLen));
	for (std::size_t i = 0; i < arrayLen; ++i) {
		setElement(result.get(), dataArray, i);
	}
	return result.release();
}

template <typename TdbT>
void TDB::CharAccessor<TdbT>::setElement(K out, TdbT const* dataArray, std::size_t index) const {
	q::type_traits<C>::index(out)[index] = dataArray[index].*field_;
}

template <typename TdbT>
void TDB::DateAccessor<TdbT>::setElement(K out, TdbT const* dataArray, std::size_t index) const {
	q::type_traits<I>::index(out)[index] = q::date2q(dataArray[index].*field_);
}

template <typename TdbT>
void TDB::TimeAccessor<TdbT>::setElement(K out, TdbT const* dataArray, std::size_t index) const {
	q::type_traits<I>::index(out)[index] = util::time2q(dataArray[index].*field_);
}

template <typename TdbT, typename QType>
void TDB::IntAccessor<TdbT, QType>::setElement(K out, TdbT const* dataArray, std::size_t index) const {
	q::type_traits<QType>::index(out)[index] = dataArray[index].*field_;
}

template <typename TdbT, typename Str, typename Encoder>
void TDB::StringAccessor<TdbT, Str, Encoder>::setElement(K out, TdbT const* dataArray, std::size_t index) const {
	q::type_traits<void>::index(out)[index] = kp(const_cast<S>(encode_(dataArray[index].*field_).c_str()));
}

template <typename TdbT, typename Str, typename Encoder>
void TDB::SymbolAccessor<TdbT, Str, Encoder>::setElement(K out, TdbT const* dataArray, std::size_t index) const {
	q::type_traits<S>::index(out)[index] = ss(const_cast<S>(encode_(dataArray[index].*field_).c_str()));
}

template <typename TdbT, typename Val>
void TDB::FloatAccessor<TdbT, Val>::setElement(K out, TdbT const* dataArray, std::size_t index) const {
	q::type_traits<F>::index(out)[index] = scalar_ * dataArray[index].*field_;
}

template <typename TdbT, typename Vals>
void TDB::FloatsAccessor<TdbT, Vals>::setElement(K out, TdbT const* dataArray, std::size_t index) const {
	std::size_t const size = std::extent<Vals, 0>::value;
	K dst = q::type_traits<void>::index(out)[index] = ktn(KF, size);
	Vals const& src = dataArray[index].*field_;
	for (std::size_t j = 0; j < size; ++j) {
		kF(dst)[j] = scalar_ * src[j];
	}
}

#pragma endregion

template <typename Traits>
void TDB::parseIndicators(K indicators, std::vector<typename Traits::field_accessor_type const*>& indis)
	throw(std::string)
{
	assert(indis.empty());
	std::vector<std::string> const is = q::qList2String(indicators);
	indis.reserve(is.size());
	for (auto i = is.cbegin(); i != is.cend(); ++i) {
		auto const f = Traits::FieldName::fromString(*i);
		if (f == Traits::NIL) {
			throw *i;
		}
		indis.push_back(Traits::Accessors[f].get());
		assert(*indis.rbegin() != NULL);
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

template <typename Traits>
K TDB::getFields() {
	std::vector<std::string> const fieldNames = Traits::FieldName::getAllStrings();
	q::K_ptr result(ktn(KS, fieldNames.size()));
	for (std::size_t i = 0; i < fieldNames.size(); ++i) {
		kS(result.get())[i] = ss(const_cast<S>(fieldNames[i].c_str()));
	}
	return result.release();
}

template <typename Traits, typename TdbReq>
K TDB::runQuery(::THANDLE tdb, TdbReq const& req,
	std::vector<typename Traits::field_accessor_type const*>& indis,
	int(*tdbCall)(::THANDLE, TdbReq const*, typename Traits::tdb_result_type**, int*))
{
	int arrayLen = 0;
	typename Traits::tdb_result_type* dataArray = NULL;
	::TDB_ERROR const result = static_cast<::TDB_ERROR>(tdbCall(tdb, &req, &dataArray, &arrayLen));
	TDB::Ptr<typename Traits::tdb_result_type> data(dataArray);
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