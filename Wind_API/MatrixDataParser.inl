#ifndef __MATRIXDATAPARSER_H__
static_assert(0, "Include Wind_API/MatrixDataParser.h instead!");
#endif

#include "util.h"

#include "kdb+.util/util.h"
#include "kdb+.util/type_convert.h"
#include "kdb+.util/type_traits.h"
#include "win32.util/CodeConvert.h"
#include "win32.util/hexDump.h"
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
		return q::Variant2q(x);
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