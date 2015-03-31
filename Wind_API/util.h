#ifndef __WIND_API__UTIL_H__
#define __WIND_API__UTIL_H__
#pragma comment(lib, "WindQuantData.lib")

#include <iosfwd>
#include <string>
#include <vector>

#ifdef _MSC_VER
//@ref https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(C4290);k(VS.ErrorList)&rd=true
#pragma warning(disable: 4290)
#endif

namespace Wind {
	namespace util {

		std::string error2Text(::WQErr error);
		char const* eventType2Text(::WQEventType eventType);

		// Convert a q date or datetime into a C++ string
		std::wstring q2tmStr(K data, std::size_t maxLen, wchar_t const* fmt) throw(std::string);
		std::wstring q2DateStr(K data) throw(std::string);
		std::wstring q2DateTimeStr(K data) throw(std::string);

		std::vector<std::wstring> qList2tmStr(K data, std::size_t maxLen, wchar_t const* fmt) throw(std::string);
		std::vector<std::wstring> qList2DateStr(K data) throw(std::string);
		std::vector<std::wstring> qList2DateTimeStr(K data) throw(std::string);

		template <typename It, typename Delim>
		std::wstring join(Delim const& delim, It begin, It end);

		template <typename Delim>
		std::wstring qList2DecStrJoin(K data, Delim const& delim) throw(std::string);
		template <typename Delim>
		std::wstring qList2FpStrJoin(K data, Delim const& delim) throw(std::string);

		template <typename Delim>
		std::wstring qList2WStringJoin(K data, Delim const& delim) throw(std::string);
		template <typename ItemDelim, typename PairDelim>
		std::wstring qDict2WStringMapJoin(K data, ItemDelim const& itemDelim, PairDelim const& pairDelim) throw(std::string);

		template <typename Delim>
		std::wstring qList2DateStrJoin(K data, Delim const& delim) throw(std::string);
		template <typename Delim>
		std::wstring qList2DateTimeStrJoin(K data, Delim const& delim) throw(std::string);

		// Common logic for simple arrays in Wind API
		template <typename WQArray, typename ElemT>
		void dupSimpleArray(WQArray& dst, WQArray const& src, ElemT* WQArray::*arrPtr);
		template <typename qTraits, typename WQArray, typename ElemT>
		K parseSimpleArray(WQArray const& array, ElemT* WQArray::*arrPtr) throw();

		template <typename qSrcTraits, typename qDstTraits>
		K qConvertArray3D(K array) throw();

	}//namespace Wind::util
}//namespace Wind

#include "util.inl"

#endif//__WIND_API__UTIL_H__