#include "stdafx.h"
#include "TDB_API.h"

#include "TDB_API_helper.h"

#include "kdb+.util/type_convert.h"

void TDB::parseTdbHandle(K h, ::THANDLE& tdb) throw(std::string) {
	assert(tdb == NULL);
	tdb = reinterpret_cast<::THANDLE>(q::q2Dec(h));
	if (!tdb) throw std::string("null THANDLE");
}