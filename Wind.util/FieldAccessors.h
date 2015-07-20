#ifndef __FIELDACCESSORS_H__
#define __FIELDACCESSORS_H__

#include "StringEncoders.h"

#include "kdb+.util/type_traits.h"

namespace Wind {
	namespace accessor {

		// Data field accessors for TDBDefine_*/TDF_* structs, handling data type conversions if necessary
		template <typename T>
		class FieldAccessor
		{
		public:
			typedef T struct_type;
		public:
			q::TypeNum const TYPE_NUM;
		public:
			FieldAccessor(q::TypeNum typeNum) : TYPE_NUM(typeNum) {}
			K extract(struct_type const* dataArray, size_t arrayLen) const;
			virtual void setElement(K out, size_t index, struct_type const& data) const = 0;
		};

		template <typename T, typename FieldT>
		class AccessorBase : public FieldAccessor<T>
		{
		public:
			typedef FieldT field_type;
			typedef field_type(struct_type::*field_accessor);
		protected:
			field_accessor const FIELD;
		public:
			AccessorBase(q::TypeNum typeNum, field_accessor field)
				: FieldAccessor<struct_type>(typeNum), FIELD(field) {}
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

		template <typename T, typename... NestedTs> struct NestedAccessor;
		template <typename T> struct NestedAccessor<T>;		//terminal condition
		template <typename T, typename NestedT0, typename... NestedTn> struct NestedAccessor<T, NestedT0, NestedTn...>;
		template <typename T, typename NestedT0, typename... NestedTn> struct NestedAccessor<T, NestedT0*, NestedTn...>;

	}//namespace Wind::accessor
}//namespace Wind

#include "FieldAccessors.inl"

#endif//__FIELDACCESSORS_H__