#include "stdafx.h"
#include "WindDateTimeArray.h"

#include "util.h"

#include "kdb+.util/type_traits.h"
#include "kdb+.util/type_convert.h"
#include <cassert>

Wind::DateTimeArray::DateTimeArray(::WQDateTimeArray const& array) {
	this->arrLen = 0;
	this->timeArray = NULL;
	util::dupSimpleArray<::WQDateTimeArray>(*this, array, &::WQDateTimeArray::timeArray);
}

Wind::DateTimeArray::~DateTimeArray() throw() {
	clear();
}

void Wind::DateTimeArray::clear() {
	this->arrLen = 0;
	delete[] timeArray;
	this->timeArray = NULL;
}

::WQDateTimeArray Wind::DateTimeArray::release() {
	::WQDateTimeArray dup(*this);	// ownership transfer
	this->timeArray = NULL;
	this->arrLen = 0;
	return dup;
}

struct Wind::DateTimeArray::qTypeTraits : q::type_traits<F> {
	static q::TypeNum const typeNum = KZ;
	static F convert(DATE date) { return q::DATE2q(date); }
};

K Wind::DateTimeArray::parse() const throw() {
	return util::parseSimpleArray<qTypeTraits, ::WQDateTimeArray>(*this, &::WQDateTimeArray::timeArray);
}