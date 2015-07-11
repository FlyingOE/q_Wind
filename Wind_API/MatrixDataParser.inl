#ifndef __MATRIXDATAPARSER_H__
static_assert(0, "Include Wind_API/MatrixDataParser.h instead!");
#endif

#include "util.h"

#include "kdb+.util/type_convert.h"
#include "kdb+.util/type_traits.h"
#include "kdb+.util/multilang.h"
#include "win32.util/util.h"
#include <cassert>
#include <sstream>


template <typename T>
class Wind::MatrixDataParser::SafeArrayAccessor {
public:
	SafeArrayAccessor(::SAFEARRAY& array) : array_(array), p_(NULL) {
		::HRESULT result = ::SafeArrayAccessData(&array_, reinterpret_cast<void HUGEP**>(&p_));
		assert(!(FAILED(result) || (p_ == NULL)));
	}
	
	~SafeArrayAccessor() {
		::SafeArrayUnaccessData(&array_);
		p_ = NULL;
	}

	T const& operator[](LONG idx) const { return p_[idx]; }
	T      & operator[](LONG idx)       { return p_[idx]; }

private:
	::SAFEARRAY& array_;
	T HUGEP* p_;
};


namespace Wind {
	namespace util {
		
		template <typename T>
		struct PrimitiveTypeTraits {
			static T convert(T x) { return x; }
			static K checkError(T) { return K_NIL; }
		};

		struct qDateTypeTraits : q::type_traits <I> {
			static q::TypeNum const TYPE_NUM = KD;
			static I convert(DATE date) { return static_cast<I>(q::DATE2q(date)); }
		};

	}//namespace Wind::util
}//namespace Wind


template <typename T>
struct Wind::MatrixDataParser::qTypeTraits : q::type_traits<T>, Wind::util::PrimitiveTypeTraits<T> {};

template <>
struct Wind::MatrixDataParser::qTypeTraits<::BSTR> : q::type_traits<void> {
	static K convert(::BSTR x) {
		return kp(const_cast<S>(ml::convert(q::DEFAULT_CP, x).c_str()));
	}

	static K checkError(K& k) {
		K error = K_NIL;
		if ((k != K_NIL) && (k->t == -128)) {
			error = k;
			k = K_NIL;
		}
		return error;
	}
};

//@ref Wind::MatrixDataParser::parse()
template <>
struct Wind::MatrixDataParser::qTypeTraits<::VARIANT> : q::type_traits<void> {
	static K convert(::VARIANT const& x) {
		switch (x.vt) {
		case ::VT_EMPTY:
		case ::VT_NULL:
			return ktn(0, 0);	// Not ideal, but since we don't know the expected data type here...
		case ::VT_I2:
			static_assert(std::is_same<H, SHORT>::value, "Mismatched data types: H vs SHORT");
			return kh(x.iVal);
		case ::VT_I4:
			//static_assert(std::is_same<I, LONG>::value, "Mismatched data types: I vs LONG");
			//TODO: cannot use std::is_same<>... when will this break?!
			static_assert(sizeof(I) == sizeof(LONG), "Mismatched data types: I vs LONG");
			return ki(x.lVal);
		case ::VT_I8:
			static_assert(std::is_same<J, LONGLONG>::value, "Mismatched data types: J vs LONGLONG");
			return kj(x.llVal);
		case ::VT_UI2:
			static_assert(sizeof(I) > sizeof(USHORT), "No suitable data types: I vs USHORT");
			return ki(x.uiVal);
		case ::VT_UI4:
			static_assert(sizeof(J) > sizeof(ULONG), "No suitable data types: J vs ULONG");
			return kj(x.ulVal);
		case ::VT_UI8:
			static_assert(sizeof(J) >= sizeof(ULONGLONG), "No suitable data types: J vs ULONGLONG");
			return (x.ullVal <= static_cast<ULONGLONG>(std::numeric_limits<J>::max()))
				? kj(x.ullVal)
				: q::error2q("ULONGLONG out of bounds of J");
		case ::VT_R4:
			static_assert(std::is_same<E, FLOAT>::value, "Mismatched data types: E vs FLOAT");
			return ke(x.fltVal);
		case ::VT_R8:
			static_assert(std::is_same<F, DOUBLE>::value, "Mismatched data types: F vs DOUBLE");
			return kf(x.dblVal);
		case ::VT_DATE:
			return kd(static_cast<I>(q::DATE2q(x.date)));
		case ::VT_BSTR:
			return kp(const_cast<S>(ml::convert(q::DEFAULT_CP, x.bstrVal).c_str()));
		default: {
				std::ostringstream buffer;
				buffer << "unsupported VARIANT type: 0x" << ::util::hexBytes(x.vt);
				return q::error2q(buffer.str());
			}
		}
	}

	static K checkError(K& k) {
		K error = K_NIL;
		if ((k != K_NIL) && (k->t == -128)) {
			error = k;
			k = K_NIL;
		}
		return error;
	}
};


template <typename T, typename Traits>
K Wind::MatrixDataParser::parseSafeArray(::SAFEARRAY& array) throw() {
	UINT const dims = ::SafeArrayGetDim(&array);
	if (dims != 3) {
		std::ostringstream buffer;
		buffer << "unsupported VARIANT SAFEARRAY dimensions: " << dims;
		return q::error2q(buffer.str());
	}

	LONG lb[3], ub[3];
	for (UINT i = 0; i < dims; ++i) {
		::SafeArrayGetLBound(&array, 1 + i, &lb[i]);
		::SafeArrayGetUBound(&array, 1 + i, &ub[i]);
		assert(lb[i] <= ub[i]);
	}

#	define Xd(bounds)	(bounds[0])
#	define Yd(bounds)	(bounds[1])
#	define Zd(bounds)	(bounds[2])
#	define Dn(XYZ)		(XYZ(ub) - XYZ(lb) + 1)
#	define Di(XYZ, idx)	(idx - XYZ(lb))
	SafeArrayAccessor<T> data(array);
	q::K_ptr result(ktn(0, Dn(Zd)));
	for (LONG idx = 0, z = Zd(lb); z <= Zd(ub); ++z) {
		K& resultZ = kK(result.get())[Di(Zd, z)];
		resultZ = ktn(0, Dn(Yd));
		for (LONG y = Yd(lb); y <= Yd(ub); ++y) {
			K& resultY = kK(resultZ)[Di(Yd, y)];
			resultY = ktn(Traits::TYPE_NUM, Dn(Xd));
			for (LONG x = Xd(lb); x <= Xd(ub); ++x) {
				typename Traits::value_type& k = Traits::index(resultY)[Di(Xd, x)];
				k = Traits::convert(data[idx++]);
				K const error = Traits::checkError(k);	// detect potential conversion errors
				if (error != K_NIL) return error;
			}
		}
	}
#	undef Xd
#	undef Yd
#	undef Zd
#	undef Dn
#	undef Di

	return result.release();
}