#include "stdafx.h"
#include "WindFieldsArray.h"

#include "util.h"

#include "kdb+.util/type_traits.h"
#include <cassert>

Wind::WindFieldsArray::WindFieldsArray(::WQWindFieldsArray const& array) {
	this->arrLen = 0;
	this->fieldsArray = NULL;
	util::dupSimpleArray<::WQWindFieldsArray>(*this, array, &::WQWindFieldsArray::fieldsArray);
}

Wind::WindFieldsArray::~WindFieldsArray() throw() {
	clear();
}

void Wind::WindFieldsArray::clear() {
	delete[] fieldsArray;
	reset();
}

::WQWindFieldsArray Wind::WindFieldsArray::release() {
	::WQWindFieldsArray dup(*this);	// ownership transfer
	reset();
	return dup;
}

void Wind::WindFieldsArray::reset() {
	this->arrLen = 0;
	this->fieldsArray = NULL;
}

struct Wind::WindFieldsArray::qTypeTraits : q::type_traits<S>{
	static S convert(::WQWindField const code) { return ss(const_cast<S>(code)); }
};

K Wind::WindFieldsArray::parse() const throw() {
	return util::parseSimpleArray<qTypeTraits, ::WQWindFieldsArray>(*this, &::WQWindFieldsArray::fieldsArray);
}