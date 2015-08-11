#include "stdafx.h"
#include "WindCodeArray.h"

#include "util.h"

#include "kdb+.util/type_traits.h"
#include <cassert>

Wind::WindCodeArray::WindCodeArray(::WQWindCodeArray const& array) {
	this->arrLen = 0;
	this->codeArray = NULL;
	util::dupSimpleArray<::WQWindCodeArray>(*this, array, &::WQWindCodeArray::codeArray);
}

Wind::WindCodeArray::~WindCodeArray() throw() {
	clear();
}

void Wind::WindCodeArray::clear() {
	delete[] codeArray;
	reset();
}

::WQWindCodeArray Wind::WindCodeArray::release() {
	::WQWindCodeArray dup(*this);	// ownership transfer
	reset();
	return dup;
}

void Wind::WindCodeArray::reset() {
	this->arrLen = 0;
	this->codeArray = NULL;
}

struct Wind::WindCodeArray::qTypeTraits : q::type_traits<void>{
	static K convert(::WQWindCode const code) { return kp(const_cast<S>(code)); }
};

K Wind::WindCodeArray::parse() const throw() {
	return util::parseSimpleArray<qTypeTraits, ::WQWindCodeArray>(*this, &::WQWindCodeArray::codeArray);
}