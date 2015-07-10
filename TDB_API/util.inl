#ifndef __TDB_API__UTIL_H__
static_assert(0, "Include TDB_API/util.h instead!");
#endif

#include "kdb+.util/type_traits.h"

#include <cassert>


template <typename T>
struct TDB::Deleter {
	void operator()(T*& p) const {
		::TDB_Free(p);
		p = NULL;
	}
};


template <typename TDBDefine_T>
K TDB::FieldAccessor<TDBDefine_T>::extract(TDBDefine_T const* dataArray, std::size_t arrayLen) const {
	assert(NULL != dataArray);
	assert(arrayLen >= 0);
	q::K_ptr result(ktn(getTypeNum(), arrayLen));
	for (std::size_t i = 0; i < arrayLen; ++i) {
		setElement(result.get(), dataArray, i);
	}
	return result.release();
}


template <typename TDBDefine_T>
void TDB::CharAccessor<TDBDefine_T>::setElement(K out, TDBDefine_T const* dataArray, std::size_t index) const {
	q::type_traits<C>::index(out)[index] = dataArray[index].*field_;
}

template <typename TDBDefine_T>
void TDB::DateAccessor<TDBDefine_T>::setElement(K out, TDBDefine_T const* dataArray, std::size_t index) const {
	q::type_traits<I>::index(out)[index] = q::date2q(dataArray[index].*field_);
}

template <typename TDBDefine_T>
void TDB::TimeAccessor<TDBDefine_T>::setElement(K out, TDBDefine_T const* dataArray, std::size_t index) const {
	q::type_traits<I>::index(out)[index] = util::time2q(dataArray[index].*field_);
}

template <typename TDBDefine_T, typename QType>
void TDB::IntAccessor<TDBDefine_T, QType>::setElement(K out, TDBDefine_T const* dataArray, std::size_t index) const {
	q::type_traits<QType>::index(out)[index] = dataArray[index].*field_;
}

template <typename TDBDefine_T, typename Str, typename Encoder>
void TDB::StringAccessor<TDBDefine_T, Str, Encoder>::setElement(K out, TDBDefine_T const* dataArray, std::size_t index) const {
	q::type_traits<void>::index(out)[index] = kp(encode_(dataArray[index].*field_).c_str());
}

template <typename TDBDefine_T, typename Str, typename Encoder>
void TDB::SymbolAccessor<TDBDefine_T, Str, Encoder>::setElement(K out, TDBDefine_T const* dataArray, std::size_t index) const {
	q::type_traits<S>::index(out)[index] = ss(const_cast<S>(encode_(dataArray[index].*field_).c_str()));
}

template <typename TDBDefine_T, typename Val>
void TDB::FloatAccessor<TDBDefine_T, Val>::setElement(K out, TDBDefine_T const* dataArray, std::size_t index) const {
	q::type_traits<F>::index(out)[index] = scalar_ * dataArray[index].*field_;
}

template <typename TDBDefine_T, typename Vals>
void TDB::FloatsAccessor<TDBDefine_T, Vals>::setElement(K out, TDBDefine_T const* dataArray, std::size_t index) const {
	std::size_t const size = std::extent<Vals, 0>::value;
	K dst = q::type_traits<void>::index(out)[index] = ktn(KF, size);
	Vals const& src = dataArray[index].*field_;
	for (std::size_t j = 0; j < size; ++j) {
		kF(dst)[j] = scalar_ * src[j];
	}
}
