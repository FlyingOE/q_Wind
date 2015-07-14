#ifndef __TDF_API__UTIL_H__
#define __TDF_API__UTIL_H__

#include "Wind.util/EnumUtil.h"

#include <memory>
#include <iosfwd>

#ifdef _MSC_VER
//@ref https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(C4290);k(VS.ErrorList)&rd=true
#pragma warning(disable: 4290)
#endif

enum TDF_ERR;
namespace TDF {

	std::string getError(::TDF_ERR errorCode);

	struct DataTypeFlag : public enum_util::EnumBase<DataTypeFlag, ::DATA_TYPE_FLAG> {
		static void registerAll();
	};

	// TDF result array smart pointer
	template <typename T> struct Deleter;
	template <typename T> using Ptr = std::unique_ptr<T, Deleter<T> >;

	namespace util {

		unsigned int q2time(K time) throw(std::string);

		template <typename Delim>
		std::string join(Delim const& delim, std::vector<std::string> const& list);

		template <typename It, typename Delim>
		std::string join(Delim const& delim, It begin, It end);

	}//namespace TDF::util

}//namespace TDF

template <typename Char, typename Traits>
std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, ::TDF_OPEN_SETTING_EXT const& settings);

#include "util.inl"

#endif//__TDF_API__UTIL_H__