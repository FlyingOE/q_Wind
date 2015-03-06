#include "stdafx.h"
#include "WindData.h"

#include "WindDateTimeArray.h"
#include "WindCodeArray.h"
#include "WindFieldsArray.h"
#include "MatrixDataParser.h"

#include "kdb+.util/K_ptr.h"
#include <cassert>

Wind::Data::Data(::WQData const& data) {
	DateTimeArray dateTimes(data.ArrDateTime);
	WindCodeArray windCodes(data.ArrWindCode);
	WindFieldsArray windFields(data.ArrWindFields);

	::VariantInit(&MatrixData);
	::HRESULT const result = ::VariantCopy(&this->MatrixData, &data.MatrixData);
	assert(!FAILED(result));
	this->ArrDateTime = dateTimes.release();
	this->ArrWindCode = windCodes.release();
	this->ArrWindFields = windFields.release();
}

Wind::Data::~Data() throw() {
	clear();
}

void Wind::Data::clear() {
	static_cast<DateTimeArray&>(this->ArrDateTime).clear();
	static_cast<WindCodeArray&>(this->ArrWindCode).clear();
	static_cast<WindFieldsArray&>(this->ArrWindFields).clear();
	::HRESULT const result = ::VariantClear(&this->MatrixData);
	assert(!FAILED(result));
}

::WQData Wind::Data::release() {
	::WQData dup(*this);	// ownership transfer
	static_cast<DateTimeArray&>(this->ArrDateTime).release();
	static_cast<WindCodeArray&>(this->ArrWindCode).release();
	static_cast<WindFieldsArray&>(this->ArrWindFields).release();
	::VariantInit(&this->MatrixData);
	return dup;
}

K Wind::Data::parse() const throw() {
#	define PARSE_AND_VERIFY(var, expr)	\
		q::K_ptr (var)((expr).parse());	\
		assert((var).get() != K_NIL);	\
		if ((var)->t == -128) return (var).release()
	PARSE_AND_VERIFY(dateTimes, static_cast<DateTimeArray const&>(this->ArrDateTime));
	PARSE_AND_VERIFY(windCodes, static_cast<WindCodeArray const&>(this->ArrWindCode));
	PARSE_AND_VERIFY(windFields, static_cast<WindFieldsArray const&>(this->ArrWindFields));
	PARSE_AND_VERIFY(data, MatrixDataParser(this->MatrixData));
#	undef PARSE_AND_VERIFY

	//NOTE: result is _not_ a q table! Further massaging is required in q driver code.
	return knk(4, dateTimes.release(), windCodes.release(), windFields.release(), data.release());
}