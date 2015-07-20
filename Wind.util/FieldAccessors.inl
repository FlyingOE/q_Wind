#ifndef __FIELDACCESSORS_H__
static_assert(0, "Include Wind.util/FieldAccessors.h instead!");
#endif

#include "kdb+.util/type_convert.h"
#include "kdb+.util/type_traits.h"

#include <memory>

template <typename T>
K Wind::accessor::FieldAccessor<T>::extract(
	typename Wind::accessor::FieldAccessor<T>::struct_type const* dataArray, size_t arrayLen) const
{
	assert(NULL != dataArray);
	q::K_ptr result(ktn(this->TYPE_NUM, arrayLen));
	for (size_t i = 0; i < arrayLen; ++i) {
		setElement(result.get(), i, dataArray[i]);
	}
	return result.release();
}

template <typename T>
struct Wind::accessor::CharAccessor : public Wind::accessor::AccessorBase<T, char>
{
	CharAccessor(field_accessor field) : AccessorBase<struct_type, field_type>(KC, field) {}

	virtual void setElement(K out, size_t index, struct_type const& data) const override {
		q::type_traits<C>::index(out)[index] = data.*(this->FIELD);
	}
};

template <typename T, typename QType>
struct Wind::accessor::IntAccessor : public Wind::accessor::AccessorBase<T, int>
{
	IntAccessor(field_accessor field)
		: AccessorBase<struct_type, field_type>(q::type_traits<QType>::TYPE_NUM, field) {}

	virtual void setElement(K out, size_t index, struct_type const& data) const override {
		q::type_traits<QType>::index(out)[index] = static_cast<QType>(data.*(this->FIELD));
	}
};

template <typename T, typename FieldT>
struct Wind::accessor::FloatAccessor : public Wind::accessor::AccessorBase<T, FieldT>
{
	FloatAccessor(field_accessor field, double scalar = 1.)
		: AccessorBase<struct_type, field_type>(KF, field), scalar_(scalar) {}

	virtual void setElement(K out, size_t index, struct_type const& data) const override {
		q::type_traits<F>::index(out)[index] = data.*(this->FIELD) * scalar_;
	}
private:
	double const scalar_;
};

template <typename T, typename FieldT>
struct Wind::accessor::FloatsAccessor : public Wind::accessor::AccessorBase<T, FieldT>
{
	FloatsAccessor(field_accessor field, double scalar = 1.)
		: AccessorBase<struct_type, field_type>(0, field), scalar_(scalar) {}

	virtual void setElement(K out, size_t index, struct_type const& data) const override {
		size_t const size = getCount(data);
		assert(size <= (std::extent<field_type, 0>::value));
		K dst = q::type_traits<void>::index(out)[index] = ktn(KF, size);
		field_type const& src = data.*(this->FIELD);
		for (size_t j = 0; j < size; ++j) {
			kF(dst)[j] = src[j] * scalar_;
		}
	}

	virtual size_t getCount(struct_type const& _) const {
		return std::extent<field_type, 0>::value;
	}
private:
	double const scalar_;
};

template <typename T, typename FieldT>
struct Wind::accessor::VarFloatsAccessor : public Wind::accessor::FloatsAccessor<T, FieldT>
{
	typedef typename int(struct_type::*counter_accessor);
	VarFloatsAccessor(field_accessor field, counter_accessor counter, double scalar = 1.)
		: FloatsAccessor<struct_type, field_type>(field, scalar), counter_(counter) {}

	virtual size_t getCount(struct_type const& data) const override {
		return data.*counter_;
	}
private:
	counter_accessor const counter_;
};

template <typename T>
struct Wind::accessor::DateAccessor : public Wind::accessor::AccessorBase<T, int>
{
	DateAccessor(field_accessor field)
		: AccessorBase<struct_type, field_type>(KD, field) {}

	virtual void setElement(K out, size_t index, struct_type const& data) const override {
		q::type_traits<I>::index(out)[index] = q::date2q(data.*(this->FIELD));
	}
};

template <typename T>
struct Wind::accessor::TimeAccessor : public Wind::accessor::AccessorBase <T, int>
{
	TimeAccessor(field_accessor field)
		: AccessorBase<struct_type, field_type>(KT, field) {}

	virtual void setElement(K out, size_t index, struct_type const& data) const override {
		q::type_traits<I>::index(out)[index] = q::time2q(data.*(this->FIELD));
	}
};

template <typename T, typename FieldT, typename Encoder>
struct Wind::accessor::SymbolAccessor : public Wind::accessor::AccessorBase<T, FieldT>
{
	SymbolAccessor(field_accessor field)
		: AccessorBase<struct_type, field_type>(KS, field) {}

	virtual void setElement(K out, size_t index, struct_type const& data) const override {
		std::string const sym = Encoder::encode(data.*(this->FIELD));
		q::type_traits<S>::index(out)[index] = ss(const_cast<S>(sym.c_str()));
	}
};

template <typename T, typename FieldT, typename Encoder>
struct Wind::accessor::StringAccessor : public Wind::accessor::AccessorBase<T, FieldT>
{
	StringAccessor(field_accessor field)
	: AccessorBase<struct_type, field_type>(field, 0) {}

	virtual void setElement(K out, size_t index, struct_type const& data) const override {
		std::string const str = Encoder::encode(data.*(this->FIELD));
		q::type_traits<void>::index(out)[index] = kp(const_cast<S>(str.c_str()));
	}
};

template <typename T>
struct Wind::accessor::NestedAccessor<T> : public Wind::accessor::FieldAccessor<T>
{
	typedef FieldAccessor<struct_type> field_accessor_type;
protected:
	std::unique_ptr<field_accessor_type> FIELD;
public:
	NestedAccessor(field_accessor_type* field)
		: FieldAccessor(field->TYPE_NUM), FIELD(field) {}

	virtual void setElement(K out, size_t index, struct_type const& data) const override {
		FIELD->setElement(out, index, data);
	}
};

template <typename T, typename NestedT0, typename... NestedTn>
struct Wind::accessor::NestedAccessor<T, NestedT0, NestedTn...> : public Wind::accessor::FieldAccessor<T>
{
	typedef NestedT0 nested_type;
	typedef nested_type const(struct_type::*outer_accessor);
	typedef NestedAccessor<nested_type, NestedTn...> nested_accessor;
	typedef typename nested_accessor::field_accessor_type field_accessor_type;
protected:
	nested_accessor nested_;
	outer_accessor const OUTER;
public:
	template <typename Field, typename... Args>
	NestedAccessor(field_accessor_type* field, outer_accessor outer, Args&&... args)
		: FieldAccessor<struct_type>(field->TYPE_NUM), nested_(field, std::forward(args)...), OUTER(outer) {}

	virtual void setElement(K out, size_t index, struct_type const& data) const override {
		nested_.setElement(out, index, data.*(this->OUTER));
	}
};

template <typename T, typename NestedT0, typename... NestedTn>
struct Wind::accessor::NestedAccessor<T, NestedT0*, NestedTn...> : public Wind::accessor::FieldAccessor<T>
{
	typedef NestedT0 nested_type;
	typedef nested_type const*(struct_type::*outer_accessor);
	typedef NestedAccessor<nested_type, NestedTn...> nested_accessor;
	typedef typename nested_accessor::field_accessor_type field_accessor_type;
protected:
	nested_accessor nested_;
	outer_accessor const OUTER;
public:
	template <typename... Args>
	NestedAccessor(field_accessor_type* field, outer_accessor outer, Args&&... args)
		: FieldAccessor<struct_type>(field->TYPE_NUM), nested_(field, std::forward(args)...), OUTER(outer) {}

	virtual void setElement(K out, size_t index, struct_type const& data) const override {
		nested_.setElement(out, index, *(data.*(this->OUTER)));
	}
};