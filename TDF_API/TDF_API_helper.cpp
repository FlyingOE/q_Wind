#include "stdafx.h"
#include "TDF_API_helper.h"

#include "kdb+.util/type_convert.h"

#include <cassert>

void TDF::parseTdfHandle(K h, ::THANDLE& tdf) throw(std::string) {
	assert(tdf == NULL);
	tdf = reinterpret_cast<::THANDLE>(q::q2Dec(h));
	if (!tdf) throw std::string("null THANDLE");
}