#ifndef __ENUMUTIL_H__
static_assert(0, "Include win32.util/EnumUtil.h instead!");
#endif

#include <cassert>
#include <sstream>

template <typename C, typename E>
std::vector<std::string> enum_util::EnumBase<C, E>::getAllStrings() {
	std::vector<std::string> result;
	result.reserve(getMap().size());
	for (auto i = getMap().cbegin(); i != getMap().cend(); ++i) {
		result.push_back(i->first);
	}
	return result;
}

template <typename C, typename E>
std::string const& enum_util::EnumBase<C, E>::toString(E const e) {
	for (auto const i = getMap().cbegin(); i != getMap.cend(); ++i) {
		if (i->second == e) {
			return i->first;
		}
	}
	// Not found
	std::ostringstream buffer;
	buffer << "<unknown enum=" << e << ">";
	return buffer.str();
}

template <typename C, typename E>
E enum_util::EnumBase<C, E>::fromString(char const* str) {
	assert(NULL != str);
	auto const i = getMap().find(str);
	if (i == getMap().cend()) {
		return E();
	}
	else {
		return i->second;
	}
}

template <typename C, typename E>
bool enum_util::EnumBase<C, E>::isValidString(char const* str) {
	assert(NULL != str);
	return getMap().find(str) != getMap().cend();
}

template <typename C, typename E>
void enum_util::EnumBase<C, E>::registerEnumString(E const e, char const* str) {
	auto const result = getMap().insert(typename str2enum::value_type(str, e));
	assert(result.second);	//insertion must succeed!
}

template <typename C, typename E>
typename enum_util::EnumBase<C, E>::str2enum& enum_util::EnumBase<C, E>::getMap() {
	static str2enum map;
	static volatile bool init = true;
	if (init) {
		init = false;	//NOTE: prevent infinite recursion!
		C::registerAll();
		assert(!map.empty());
	}
	return map;
}