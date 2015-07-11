#ifndef __TDB_API_HELPER_H__
#define __TDB_API_HELPER_H__

#include "kdb+.util/type_traits.h"

#include <string>
#include <vector>

#ifdef _MSC_VER
//@ref https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(C4290);k(VS.ErrorList)&rd=true
#pragma warning(disable: 4290)
#endif

namespace TDB {

#	pragma region
	// Data field accessors for TDBDefine_* structs, handling data type conversions if necessary
	template <typename TdbT>
	struct FieldAccessor {
		K extract(TdbT const* dataArray, std::size_t arrayLen) const;
	protected:
		virtual q::TypeNum getTypeNum() const = 0;
		virtual void setElement(K out, TdbT const* dataArray, std::size_t index) const = 0;
	};

	template <typename TdbT, typename FieldT>
	struct AccessorBase : public FieldAccessor<TdbT> {
		typedef FieldT field_type;
		typedef field_type(TdbT::*field_accessor);
		AccessorBase(field_accessor field, q::TypeNum typeNum) : field_(field), typeNum_(typeNum) {}
	protected:
		field_accessor const field_;
		q::TypeNum const typeNum_;
		virtual q::TypeNum getTypeNum() const override { return typeNum_; }
	};

#	define DEFINE_ACCESSOR(Accessor, FieldType, TypeNum)	\
	template <typename TdbT>	\
	struct Accessor : public AccessorBase<TdbT, FieldType> {	\
		Accessor(field_accessor field) : AccessorBase<TdbT, FieldType>(field, (TypeNum)) {}	\
	protected:	\
		virtual void setElement(K out, TdbT const* dataArray, std::size_t index) const override;	\
		}
	DEFINE_ACCESSOR(CharAccessor, char, KC);
	DEFINE_ACCESSOR(DateAccessor, int, KD);
	DEFINE_ACCESSOR(TimeAccessor, int, KT);
	//DEFINE_ACCESSOR(IntAccessor, ...)
	template <typename TdbT, typename QType>
	struct IntAccessor : public AccessorBase<TdbT, int> {
		IntAccessor(field_accessor field)
			: AccessorBase<TdbT, int>(field, q::type_traits<QType>::TYPE_NUM) {}
	protected:
		virtual void setElement(K out, TdbT const* dataArray, std::size_t index) const override;
	};
	//DEFINE_ACCESSOR(StringAccessor, ...)
	template <typename TdbT, typename Str, typename Encoder = util::PassthruEncoder>
	struct StringAccessor : public AccessorBase<TdbT, Str> {
		StringAccessor(field_accessor field)
			: AccessorBase<TdbT, Str>(field, 0) {}
	protected:
		virtual void setElement(K out, TdbT const* dataArray, std::size_t index) const override;
	private:
		Encoder const encode_;
	};
	//DEFINE_ACCESSOR(SymbolAccessor, ...)
	template <typename TdbT, typename Str, typename Encoder = util::PassthruEncoder>
	struct SymbolAccessor : public AccessorBase<TdbT, Str> {
		SymbolAccessor(field_accessor field)
			: AccessorBase<TdbT, Str>(field, KS) {}
	protected:
		virtual void setElement(K out, TdbT const* dataArray, std::size_t index) const override;
	private:
		Encoder const encode_;
	};
	//DEFINE_ACCESSOR(FloatAccessor, ...)
	template <typename TdbT, typename Val>
	struct FloatAccessor : public AccessorBase<TdbT, Val> {
		FloatAccessor(field_accessor field, double scalar = 1.)
			: AccessorBase<TdbT, Val>(field, KF), scalar_(scalar) {}
	protected:
		virtual void setElement(K out, TdbT const* dataArray, std::size_t index) const override;
	private:
		double const scalar_;
	};
	//DEFINE_ACCESSOR(FloatsAccessor, ...)
	template <typename TdbT, typename Vals>
	struct FloatsAccessor : public AccessorBase<TdbT , Vals> {
		FloatsAccessor(field_accessor field, double scalar = 1.)
			: AccessorBase<TdbT, Vals>(field, 0), scalar_(scalar) {}
	protected:
		virtual void setElement(K out, TdbT const* dataArray, std::size_t index) const override;
	private:
		double const scalar_;
	};
#	undef DEFINE_ACCESSOR
#	pragma endregion

	void parseTdbHandle(K h, ::THANDLE& tdb) throw(std::string);

	template <typename Traits>
	void parseIndicators(K indicators,
		std::vector<typename Traits::field_accessor_type const*>& indis) throw(std::string);

	template <typename TdbReq>
	void parseTdbReq(K windCode, K begin, K end, TdbReq& req) throw(std::string);

	template <typename Traits>
	K getFields();

	template <typename Traits, typename TdbReq>
	K runQuery(::THANDLE tdb, TdbReq const& req,
		std::vector<typename Traits::field_accessor_type const*>& indis,
		int(*tdbCall)(::THANDLE, TdbReq const*, typename Traits::tdb_result_type**, int*));

}//namespace TDB

#include "TDB_API_helper.inl"

#endif//__TDB_API_HELPER_H__