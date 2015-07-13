#ifndef __TDF_API__UTIL_H__
static_assert(0, "Include TDF_API/util.h instead!");
#endif

#include <sstream>

template <typename Delim>
std::string TDF::util::join(Delim const& delim, std::vector<std::string> const& list) {
	return join(delim, list.begin(), list.end());
}

template <typename It, typename Delim>
std::string TDF::util::join(Delim const& delim, It begin, It end) {
	std::ostringstream buffer;
	It p = begin;
	if (p != end) {
		buffer << *p++;
	}
	for (; p != end; ++p) {
		buffer << delim << *p;
	}
	return buffer.str();
}