#ifndef __TDF_API__UTIL_H__
#define __TDF_API__UTIL_H__

#include "TDB_API/util.h"

#include "win32.util/EnumUtil.h"

enum TDF_ERR;
namespace TDF {

	std::string getError(::TDF_ERR errorCode);

	// TDF result array smart pointer
	using TDB::Ptr;

	namespace util {

		unsigned int q2time(K time) throw(std::string);

		template <typename Delim>
		std::string join(Delim const& delim, std::vector<std::string> const& list);

		template <typename It, typename Delim>
		std::string join(Delim const& delim, It begin, It end);

	}//namespace TDF::util

	struct DataTypeFlag : public enum_util::EnumBase<DataTypeFlag, ::DATA_TYPE_FLAG> {
		static void registerAll();
	};

}//namespace TDF

#include "util.inl"

#endif//__TDF_API__UTIL_H__