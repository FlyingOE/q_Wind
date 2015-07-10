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

}//namespace TDB

#include "util.inl"

#endif//__TDB_API__UTIL_H__