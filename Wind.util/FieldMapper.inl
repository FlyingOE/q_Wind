#ifndef __FIELDMAPPER_H__
static_assert(0, "Include Wind.util/FieldMapper.h instead!");
#endif

#include "kdb+.util/K_ptr.h"
#include "win32.util/hexDump.h"

#include <sstream>

template <typename T>
K Wind::mapper::Fields<T>::getFields() const {
	std::vector<std::string> list;
	getFields(list);
	q::K_ptr result(ktn(KS, list.size()));
	for (size_t i = 0; i < list.size(); ++i) {
		kS(result.get())[i] = ss(const_cast<S>(list[i].c_str()));
	}
	return result.release();
}

template <typename T>
void Wind::mapper::Fields<T>::getFields(std::vector<std::string>& list) const {
	list.reserve(fields_.size());
	for (auto f = fields_.cbegin(); f != fields_.cend(); ++f) {
		list.push_back(f->first);
	}
}

template <typename T>
void Wind::mapper::Fields<T>::addField(
	char const* fieldName, typename Wind::mapper::Fields<T>::field_accessor* accessor)
	throw(std::string)
{
	auto const result = fields_.emplace(fieldName, typename map_type::mapped_type(accessor));
	if (!result.second) {
		std::ostringstream buffer;
		buffer << fieldName << " -> 0x" << util::hexBytes(accessor);
		throw std::string(buffer.str());
	}
}

template <typename T>
void Wind::mapper::Fields<T>::addField(
	std::string const& fieldName, typename Wind::mapper::Fields<T>::field_accessor* accessor)
	throw(std::string)
{
	addField(fieldName.c_str(), accessor);
}

template <typename T>
typename Wind::mapper::Fields<T>::field_accessor const* Wind::mapper::Fields<T>::operator[](char const* fieldName)
	throw(std::string)
{
	auto const f = fields_.find(fieldName);
	if (f == fields_.cend()) {
		throw std::string(fieldName);
	}
	else {
		return f->second.get();
	}
}

template <typename T>
typename Wind::mapper::Fields<T>::field_accessor const* Wind::mapper::Fields<T>::operator[](std::string const& fieldName)
	throw(std::string)
{
	return operator[](fieldName.c_str());
}