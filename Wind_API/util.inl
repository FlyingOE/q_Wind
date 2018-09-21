#ifndef __WIND_API__UTIL_H__
static_assert(0, "Include Wind_API/util.h instead!");
#endif

#include "kdb+.util/K_ptr.h"
#include <cassert>
#include <algorithm>
#ifdef _MSC_VER
//@see https://msdn.microsoft.com/en-us/library/dn607301.aspx
#include <iterator>	//MSVC: stdext::make_unchecked_array_iterator
#endif
#include <memory>	//C++11: std::unique_ptr<>
#include <vector>
#include <sstream>

template <typename It, typename Delim>
std::wstring Wind::util::join(Delim const& delim, It begin, It end) {
	std::wostringstream buffer;
	It p = begin;
	if (p != end) {
		buffer << *p++;
	}
	for (; p != end; ++p) {
		buffer << delim << *p;
	}
	return buffer.str();
}

template <typename Delim>
std::wstring Wind::util::qList2DecStrJoin(K data, Delim const& delim) throw(std::runtime_error) {
	std::vector<long long> const decs = q::qList2Dec(data);
	return join(delim, decs.begin(), decs.end());
}

template <typename Delim>
std::wstring Wind::util::qList2FpStrJoin(K data, Delim const& delim) throw(std::runtime_error) {
	std::vector<double> const fps = q::qList2Fp(data);
	return join(delim, fps.begin(), fps.end());
}

template <typename Delim>
std::wstring Wind::util::qList2WStringJoin(K data, Delim const& delim) throw(std::runtime_error) {
	std::vector<std::wstring> const strings = q::qList2WString(data);
	return join(delim, strings.begin(), strings.end());
}

template <typename ItemDelim, typename PairDelim>
std::wstring Wind::util::qDict2WStringMapJoin(K data, ItemDelim const& itemDelim, PairDelim const& pairDelim)
	throw(std::runtime_error)
{
	std::map<std::wstring, std::wstring> const map = q::qDict2WStringMap(data);
	std::vector<std::wstring> list;
	list.reserve(map.size());
	for (auto i = map.begin(); i != map.end(); ++i) {
		std::wostringstream buffer;
		buffer << i->first << pairDelim << i->second;
		list.push_back(buffer.str());
	}
	return join(itemDelim, list.begin(), list.end());
}

template <typename Delim>
std::wstring Wind::util::qList2DateStrJoin(K data, Delim const& delim) throw(std::runtime_error) {
	std::vector<std::wstring> const strs = qList2DateStr(data);
	return join(delim, strs.begin(), strs.end());
}

template <typename Delim>
std::wstring Wind::util::qList2DateTimeStrJoin(K data, Delim const& delim) throw(std::runtime_error) {
	std::vector<std::wstring> const strs = qList2DateTimeStr(data);
	return join(delim, strs.begin(), strs.end());
}

template <typename WQArray, typename ElemT>
void Wind::util::dupSimpleArray(WQArray& dst, WQArray const& src, ElemT* WQArray::*arrPtr) {
	assert(src.arrLen >= 0);
	std::unique_ptr<ElemT[]> data(new ElemT[src.arrLen]);
	std::memcpy(data.get(), src.*arrPtr, sizeof(ElemT) * src.arrLen);

	dst.arrLen = src.arrLen;
	dst.*arrPtr = data.release();
}

template <typename qTraits, typename WQArray, typename ElemT>
K Wind::util::parseSimpleArray(WQArray const& array, ElemT* WQArray::*arrPtr) throw() {
	assert(array.arrLen >= 0);
	q::K_ptr result(ktn(qTraits::TYPE_NUM, array.arrLen));

	typename qTraits::value_type* const dst = qTraits::index(result.get());
	assert(dst != NULL);
	ElemT const* const src = array.*arrPtr;
	assert(src != NULL);
#	ifdef _MSC_VER
	std::transform(src, src + array.arrLen, stdext::make_unchecked_array_iterator(dst), &qTraits::convert);
#	else
	std::transform(src, src + array.arrLen, dst, &qTraits::convert);
#	endif
	return result.release();
}

template <typename qSrcTraits, typename qDstTraits>
K Wind::util::qConvertArray3D(K array) throw() {
	if (array == K_NIL) {
		return K_NIL;
	}

	assert((array->t == 0) && (array->n >= 0));
	size_t const len = static_cast<size_t>(array->n);
	q::K_ptr result(ktn(0, len));
	for (size_t z = 0; z < len; ++z) {
		K const arrayZ = kK(array)[z];
		assert(arrayZ != K_NIL);
		K& resultZ = kK(result.get())[z];
		assert((arrayZ->t == 0) && (arrayZ->n >= 0));
		size_t const lenZ = static_cast<size_t>(arrayZ->n);
		resultZ = ktn(0, lenZ);
		for (size_t y = 0; y < lenZ; ++y) {
			K const arrayY = kK(arrayZ)[y];
			assert(arrayY != K_NIL);
			K& resultY = kK(resultZ)[y];
			assert((arrayY->t == qSrcTraits::TYPE_NUM) && (arrayY->n >= 0));
			size_t const lenY = static_cast<size_t>(arrayY->n);
			resultY = ktn(qDstTraits::TYPE_NUM, lenY);
			for (size_t x = 0; x < lenY; ++x) {
				qDstTraits::index(resultY)[x] = qDstTraits::convert(qSrcTraits::index(arrayY)[x]);
			}
		}
	}

	return result.release();
}