#ifndef __TDB_API__UTIL_H__
#define __TDB_API__UTIL_H__

#include "kdb+.util/type_convert.h"

#include <memory>

namespace TDB {
	
	namespace util {

		I time2q(int hhmmssfff);

		void tm2DateTime(q::tm_ext const& tm, int &date, int &time);

		// String encoding converters
		struct PassthruEncoder {
			std::string operator()(char const* str) const { return str;  }
		};
		struct GB18030Encoder {
			std::string operator()(char const* str) const;
		};

	}//namespace Wind::util

	std::string getError(int errorCode);


	// TDB result array smart pointer
	template <typename T> struct Deleter;
	template <typename T> using Ptr = std::unique_ptr<T, Deleter<T> >;


#	pragma region
	// Data field accessors for TDBDefine_* structs, handling data type conversions if necessary
	template <typename TDBDefine_T>
	struct FieldAccessor {
		K extract(TDBDefine_T const* dataArray, std::size_t arrayLen) const;
	protected:
		virtual H getTypeNum() const = 0;
		virtual void setElement(K out, TDBDefine_T const* dataArray, std::size_t index) const = 0;
	};

	template <typename TDBDefine_T, typename T>
	struct AccessorBase : public FieldAccessor<TDBDefine_T> {
		typedef T field_type;
		typedef field_type(TDBDefine_T::*field_accessor);
		AccessorBase(field_accessor field, H typeNum) : field_(field), typeNum_(typeNum) {}
	protected:
		field_accessor const field_;
		H const typeNum_;
		virtual H getTypeNum() const override { return typeNum_; }
	};

#	define DEFINE_ACCESSOR(Accessor, FieldType, TypeNum)	\
	template <typename TDBDefine_T>	\
	struct Accessor : public AccessorBase<TDBDefine_T, FieldType> {	\
		Accessor(field_accessor field) : AccessorBase<TDBDefine_T, FieldType>(field, (TypeNum)) {}	\
	protected:	\
		virtual void setElement(K out, TDBDefine_T const* dataArray, std::size_t index) const override;	\
	}
	DEFINE_ACCESSOR(CharAccessor, char, KC);
	DEFINE_ACCESSOR(DateAccessor, int, KD);
	DEFINE_ACCESSOR(TimeAccessor, int, KT);
	//DEFINE_ACCESSOR(IntAccessor, ...)
	template <typename TDBDefine_T, typename QType>
	struct IntAccessor : public AccessorBase<TDBDefine_T, int> {
		IntAccessor(field_accessor field)
			: AccessorBase<TDBDefine_T, int>(field, q::type_traits<QType>::TYPE_NUM) {}
	protected:
		virtual void setElement(K out, TDBDefine_T const* dataArray, std::size_t index) const override;
	};
	//DEFINE_ACCESSOR(StringAccessor, ...)
	template <typename TDBDefine_T, typename Str, typename Encoder = util::PassthruEncoder>
	struct StringAccessor : public AccessorBase<TDBDefine_T, Str> {
		StringAccessor(field_accessor field)
			: AccessorBase<TDBDefine_T, Str>(field, 0) {}
	protected:
		virtual void setElement(K out, TDBDefine_T const* dataArray, std::size_t index) const override;
	private:
		Encoder encode_;
	};
	//DEFINE_ACCESSOR(SymbolAccessor, ...)
	template <typename TDBDefine_T, typename Str, typename Encoder = util::PassthruEncoder>
	struct SymbolAccessor : public AccessorBase<TDBDefine_T, Str> {
		SymbolAccessor(field_accessor field)
			: AccessorBase<TDBDefine_T, Str>(field, KS) {}
	protected:
		virtual void setElement(K out, TDBDefine_T const* dataArray, std::size_t index) const override;
	private:
		Encoder encode_;
	};
	//DEFINE_ACCESSOR(FloatAccessor, ...)
	template <typename TDBDefine_T, typename Val>
	struct FloatAccessor : public AccessorBase<TDBDefine_T, Val> {
		FloatAccessor(field_accessor field, double scalar = 1.)
			: AccessorBase<TDBDefine_T, Val>(field, KF), scalar_(scalar) {}
	private:
		double const scalar_;
		virtual void setElement(K out, TDBDefine_T const* dataArray, std::size_t index) const override;
	};
	//DEFINE_ACCESSOR(FloatsAccessor, ...)
	template <typename TDBDefine_T, typename Vals>
	struct FloatsAccessor : public AccessorBase<TDBDefine_T, Vals> {
		FloatsAccessor(field_accessor field, double scalar = 1.)
			: AccessorBase<TDBDefine_T, Vals>(field, 0), scalar_(scalar) {}
	private:
		double const scalar_;
		virtual void setElement(K out, TDBDefine_T const* dataArray, std::size_t index) const override;
	};
#	undef DEFINE_ACCESSOR
#	pragma endregion

}//namespace TDB

#include "util.inl"

#endif//__TDB_API__UTIL_H__