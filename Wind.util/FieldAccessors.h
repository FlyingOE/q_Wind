#ifndef __FIELDACCESSORS_H__
#define __FIELDACCESSORS_H__

#include "StringEncoders.h"

#include "kdb+.util/type_traits.h"

namespace Wind {
	namespace accessor {

		// Data field accessors for TDBDefine_*/TDF_* structs, handling data type conversions if necessary
		template <typename T>
		struct FieldAccessor {
			K extract(T const* dataArray, std::size_t arrayLen) const;
		protected:
			virtual q::TypeNum getTypeNum() const = 0;
			virtual void setElement(K out, T const* dataArray, std::size_t index) const = 0;
		};

		template <typename T, typename F>
		struct AccessorBase : public FieldAccessor<T> {
			typedef F field_type;
			typedef field_type(T::*field_accessor);
			AccessorBase(field_accessor field, q::TypeNum typeNum) : field_(field), typeNum_(typeNum) {}
		protected:
			field_accessor const field_;
			q::TypeNum const typeNum_;
			virtual q::TypeNum getTypeNum() const override { return typeNum_; }
		};

#	define DECL_ACCESSOR_INTERF	\
		protected:	\
			virtual void setElement(K out, T const* dataArray, std::size_t index) const override
#	define DECL_ACCESSOR(Accessor, FieldType, TypeNum)	\
		template <typename T>	\
		struct Accessor : public AccessorBase<T, FieldType> {	\
			Accessor(field_accessor field) : AccessorBase<T, FieldType>(field, (TypeNum)) {}	\
			DECL_ACCESSOR_INTERF;	\
		}
		DECL_ACCESSOR(CharAccessor, char, KC);
		DECL_ACCESSOR(DateAccessor, int, KD);
		DECL_ACCESSOR(TimeAccessor, int, KT);
#	undef DECL_ACCESSOR

		template <typename T, typename QType>
		struct IntAccessor : public AccessorBase<T, int> {
			IntAccessor(field_accessor field) : AccessorBase<T, int>(field, q::type_traits<QType>::TYPE_NUM) {}
			DECL_ACCESSOR_INTERF;
		};

		template <typename T, typename Str, typename Encoder = encoder::PassthruEncoder>
		struct StringAccessor : public AccessorBase<T, Str> {
			StringAccessor(field_accessor field) : AccessorBase<T, Str>(field, 0) {}
			DECL_ACCESSOR_INTERF;
		private:
			Encoder const encode_;
		};

		template <typename T, typename Str, typename Encoder = encoder::PassthruEncoder>
		struct SymbolAccessor : public AccessorBase<T, Str> {
			SymbolAccessor(field_accessor field) : AccessorBase<T, Str>(field, KS) {}
			DECL_ACCESSOR_INTERF;
		private:
			Encoder const encode_;
		};

		template <typename T, typename Val>
		struct FloatAccessor : public AccessorBase<T, Val> {
			FloatAccessor(field_accessor field, double scalar = 1.) : AccessorBase<T, Val>(field, KF), scalar_(scalar) {}
			DECL_ACCESSOR_INTERF;
		private:
			double const scalar_;
		};

		template <typename T, typename Vals>
		struct FloatsAccessor : public AccessorBase<T, Vals> {
			FloatsAccessor(field_accessor field, double scalar = 1.) : AccessorBase<T, Vals>(field, 0), scalar_(scalar) {}
			DECL_ACCESSOR_INTERF;
		private:
			double const scalar_;
		};

#	undef DECL_ACCESSOR_INTERF

	}//namespace Wind::accessor
}//namespace Wind

#include "FieldAccessors.inl"

#endif//__FIELDACCESSORS_H__