#ifndef __TDF_API__UTIL_H__
static_assert(0, "Include TDF_API/util.h instead!");
#endif

#include <sstream>

template <typename T>
struct TDF::Deleter {
	void operator()(T*& p) const {
		::TDF_FreeArr(p);
		p = NULL;
	}
};

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

template <typename Char, typename Traits>
std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, ::TDF_OPEN_SETTING_EXT const& settings) {
	os << "{[";
	for (std::size_t i = 0; i < settings.nServerNum; ++i) {
		os << "{\"" << settings.siServer[i].szIp << "\", "
			<< '"' << settings.siServer[i].szPort << "\", "
			<< '"' << settings.siServer[i].szUser << "\", "
			<< '"' << settings.siServer[i].szPwd << "\"}";
		if (i + 1 < settings.nServerNum) os << ", ";
	}
	os << "], \"" << settings.szMarkets << "\", \"" << settings.szSubScriptions << "\", "
		<< settings.nTime << ", 0x" << util::hexBytes(settings.nTypeFlags) << '}';
	return os;
}