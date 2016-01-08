#include "stdafx.h"
#include "TDB_API.h"

#include "TDB_API_helper.h"

#include <sstream>

void TDB::parseTdbHandle(K h, ::THANDLE& tdb) throw(std::string) {
	assert(tdb == NULL);
	tdb = reinterpret_cast<::THANDLE>(q::q2Dec(h));
	if (!tdb) throw std::string("null THANDLE");
}

std::string TDB::getMarketId(::THANDLE tdb, char const* market) throw(std::string) {
	auto const level = TDB::LEVELS.find(tdb);
	if (level == TDB::LEVELS.cend()) throw std::string("unknown THANDLE");
	std::ostringstream buffer;
	buffer << market << '-' << static_cast<int>(level->second) << '-' << static_cast<int>(TDB::DATA_SRC);
	return buffer.str();
}

std::string TDB::getMarketId(::THANDLE tdb, std::string const& market) throw(std::string) {
	return getMarketId(tdb, market.c_str());
}