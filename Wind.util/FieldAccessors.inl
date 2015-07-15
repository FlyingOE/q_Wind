#ifndef __FIELDACCESSORS_H__
static_assert(0, "Include Wind.util/FieldAccessors.h instead!");
#endif

#include "util.h"

template <typename T>
K Wind::accessor::FieldAccessor<T>::extract(T const* dataArray, std::size_t arrayLen) const {
	assert(NULL != dataArray);
	assert(arrayLen >= 0);
	q::K_ptr result(ktn(getTypeNum(), arrayLen));
	for (std::size_t i = 0; i < arrayLen; ++i) {
		setElement(result.get(), dataArray, i);
	}
	return result.release();
}

template <typename T>
void Wind::accessor::CharAccessor<T>::setElement(K out, T const* dataArray, std::size_t index) const {
	q::type_traits<C>::index(out)[index] = dataArray[index].*field_;
}

template <typename T>
void Wind::accessor::DateAccessor<T>::setElement(K out, T const* dataArray, std::size_t index) const {
	q::type_traits<I>::index(out)[index] = q::date2q(dataArray[index].*field_);
}

template <typename T>
void Wind::accessor::TimeAccessor<T>::setElement(K out, T const* dataArray, std::size_t index) const {
	q::type_traits<I>::index(out)[index] = q::time2q(dataArray[index].*field_);
}

template <typename T, typename QType>
void Wind::accessor::IntAccessor<T, QType>::setElement(K out, T const* dataArray, std::size_t index) const {
	q::type_traits<QType>::index(out)[index] = dataArray[index].*field_;
}

template <typename T, typename Str, typename Encoder>
void Wind::accessor::StringAccessor<T, Str, Encoder>::setElement(K out, T const* dataArray, std::size_t index) const {
	q::type_traits<void>::index(out)[index] = kp(const_cast<S>(encode_(dataArray[index].*field_).c_str()));
}

template <typename T, typename Str, typename Encoder>
void Wind::accessor::SymbolAccessor<T, Str, Encoder>::setElement(K out, T const* dataArray, std::size_t index) const {
	q::type_traits<S>::index(out)[index] = ss(const_cast<S>(encode_(dataArray[index].*field_).c_str()));
}

template <typename T, typename Val>
void Wind::accessor::FloatAccessor<T, Val>::setElement(K out, T const* dataArray, std::size_t index) const {
	q::type_traits<F>::index(out)[index] = scalar_ * dataArray[index].*field_;
}

template <typename T, typename Vals>
void Wind::accessor::FloatsAccessor<T, Vals>::setElement(K out, T const* dataArray, std::size_t index) const {
	std::size_t const size = std::extent<Vals, 0>::value;
	K dst = q::type_traits<void>::index(out)[index] = ktn(KF, size);
	Vals const& src = dataArray[index].*field_;
	for (std::size_t j = 0; j < size; ++j) {
		kF(dst)[j] = scalar_ * src[j];
	}
}