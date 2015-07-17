#ifndef __FIELDACCESSORS_H__
#define __FIELDACCESSORS_H__

#include "StringEncoders.h"

#include "kdb+.util/type_traits.h"

namespace Wind {
	namespace accessor {

		// Data field accessors for TDBDefine_*/TDF_* structs, handling data type conversions if necessary
		template <typename T>
		class FieldAccessor {
		public:
			typedef T struct_type;
		public:
			q::TypeNum const TYPE_NUM;
		public:
			FieldAccessor(q::TypeNum typeNum) : TYPE_NUM(typeNum) {}
			K extract(struct_type const* dataArray, size_t arrayLen) const;
		protected:
			virtual void setElement(K out, struct_type const* dataArray, size_t index) const = 0;
		};

		template <typename T, typename FieldT>
		class AccessorBase : public FieldAccessor<T> {
		public:
			typedef FieldT field_type;
			typedef field_type(struct_type::*field_accessor);
		protected:
			field_accessor const FIELD;
		public:
			AccessorBase(q::TypeNum typeNum, field_accessor field) : FieldAccessor<struct_type>(typeNum), FIELD(field) {}
		};

		template <typename T> struct CharAccessor;
		template <typename T, typename QType> struct IntAccessor;
		template <typename T, typename FieldT> struct FloatAccessor;
		template <typename T, typename FieldT> struct FloatsAccessor;
		template <typename T, typename FieldT> struct VarFloatsAccessor;
		template <typename T> struct DateAccessor;
		template <typename T> struct TimeAccessor;
		template <typename T, typename FieldT, typename Encoder = encoder::Passthrough> struct SymbolAccessor;
		template <typename T, typename FieldT, typename Encoder = encoder::Passthrough> struct StringAccessor;

		/*

		template <typename T, typename Accessor>
		struct NestedFieldAccessor : public AccessorBase<T, typename Accessor::field_type> {
			typedef typename Accessor::struct_type(field_type::*outer_accessor);
			typedef typename Accessor::field_accessor inner_accessor;
			NestedFieldAccessor(outer_accessor outer, inner_accessor inner)
				: AccessorBase<struct_type, field_type>() {}
			DECL_ACCESSOR_INTERF;
		};

		template <typename T, typename Accessor>
		struct NestedPtrFieldAccessor : public AccessorBase<T, typename Accessor::field_type> {
			typedef typename Accessor::struct_type*(field_type::*outer_accessor);
			typedef typename Accessor::field_accessor inner_accessor;
			NestedFieldAccessor(outer_accessor outer, inner_accessor inner)
				: AccessorBase<struct_type, field_type>() {}
			DECL_ACCESSOR_INTERF;
		};
		*/

	}//namespace Wind::accessor
}//namespace Wind

#include "FieldAccessors.inl"

#endif//__FIELDACCESSORS_H__