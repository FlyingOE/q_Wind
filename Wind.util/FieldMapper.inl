#ifndef __FIELDMAPPER_H__
static_assert(0, "Include Wind.util/FieldMapper.h instead!");
#endif

#include "kdb+.util/K_ptr.h"
#include "win32.util/hexDump.h"

#include <set>
#include <sstream>

template <typename T>
K Wind::mapper::Fields<T>::getFields() const {
	return getFields(NULL);
}

template <typename T>
K Wind::mapper::Fields<T>::getFields(char const* category) const {
	std::vector<std::string> list;
	getFields(category, list);
	q::K_ptr result(ktn(KS, list.size()));
	for (size_t i = 0; i < list.size(); ++i) {
		kS(result.get())[i] = ss(const_cast<S>(list[i].c_str()));
	}
	return result.release();
}

template <typename T>
K Wind::mapper::Fields<T>::getFields(std::string const& category) const {
	return getFields(category.c_str());
}

template <typename T>
void Wind::mapper::Fields<T>::getFields(std::vector<std::string>& list) const {
	return getFields(NULL, list);
}

template <typename T>
void Wind::mapper::Fields<T>::getFields(char const* category, std::vector<std::string>& list) const {
	if (category == NULL) {
		category = "";
	}
	list.empty();
	list.reserve(catalog_.size());
	for (auto c = catalog_.cbegin(); c != catalog_.cend(); ++c) {
		if (c->first == category) {
			list.push_back(c->second);
		}
	}
}

template <typename T>
void Wind::mapper::Fields<T>::getFields(std::string const& category, std::vector<std::string>& list) const {
	return getFields(category.c_str(), list);
}

template <typename T>
void Wind::mapper::Fields<T>::addField(
	char const* fieldName, typename Wind::mapper::Fields<T>::field_accessor* accessor)
	throw(std::runtime_error)
{
	addField("", fieldName, accessor);
}

template <typename T>
void Wind::mapper::Fields<T>::addField(
	std::string const& fieldName, typename Wind::mapper::Fields<T>::field_accessor* accessor)
	throw(std::runtime_error)
{
	addField("", fieldName.c_str(), accessor);
}

template <typename T>
void Wind::mapper::Fields<T>::addField(char const* category,
	char const* fieldName, typename Wind::mapper::Fields<T>::field_accessor* accessor)
	throw(std::runtime_error)
{
	auto const result = fields_.emplace(fieldName, typename map_type::mapped_type(accessor));
	if (!result.second) {
		std::ostringstream buffer;
		buffer << fieldName << " -> 0x" << util::hexBytes(accessor);
		throw std::runtime_error(buffer.str());
	}
	catalog_.emplace(category, fieldName);
}

template <typename T>
void Wind::mapper::Fields<T>::addField(std::string const& category,
	std::string const& fieldName, typename Wind::mapper::Fields<T>::field_accessor* accessor)
	throw(std::runtime_error)
{
	addField(category.c_str(), fieldName.c_str(), accessor);
}

template <typename T>
void Wind::mapper::Fields<T>::getCategories(std::vector<std::string>& list) const {
	list.empty();
	std::set<std::string> cats;
	for (auto c = catalog_.cbegin(); c != catalog_.cend(); ++c) {
		cats.emplace(c->first);
	}
	list.assign(cats.cbegin(), cats.cend());
}

template <typename T>
bool Wind::mapper::Fields<T>::hasCategory(char const* category) const {
	return catalog_.count(category) > 0;
}

template <typename T>
bool Wind::mapper::Fields<T>::hasCategory(std::string const& category) const {
	return hasCategory(category.c_str());
}

template <typename T>
typename Wind::mapper::Fields<T>::field_accessor const* Wind::mapper::Fields<T>::operator[](char const* fieldName)
throw(std::runtime_error)
{
	auto const f = fields_.find(fieldName);
	if (f == fields_.cend()) {
		throw std::runtime_error(fieldName);
	}
	else {
		return f->second.get();
	}
}

template <typename T>
typename Wind::mapper::Fields<T>::field_accessor const* Wind::mapper::Fields<T>::operator[](std::string const& fieldName)
throw(std::runtime_error)
{
	return operator[](fieldName.c_str());
}