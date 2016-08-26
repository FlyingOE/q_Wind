#ifndef __TDB_API__UTIL_H__
#define __TDB_API__UTIL_H__

#include "kdb+.util/type_convert.h"

#include <memory>

enum TDB_ERROR;
namespace TDB {
	
	std::string getError(::TDB_ERROR errorCode);

	// TDB result array smart pointer
	template <typename T> struct Deleter;
	template <typename T> using Ptr = std::unique_ptr<T, Deleter<T> >;

	namespace util {

		void fillDateTime(K data, int &date, int &time) throw(std::runtime_error);
		void tm2DateTime(q::tm_ext const& tm, int &date, int &time);
	
	}//namespace TDB::util

}//namespace TDB

#include "util.inl"

#endif//__TDB_API__UTIL_H__