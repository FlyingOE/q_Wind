#ifndef __TDB_API__UTIL_H__
static_assert(0, "Include TDB_API/util.h instead!");
#endif

#include <sstream>

template <typename T>
struct TDB::Deleter {
	void operator()(T*& p) const {
		::TDB_Free(p);
		p = NULL;
	}
};

template <typename T>
std::string TDB::util::StringizeEncoder::operator()(T const& data) const {
	std::ostringstream buffer;
	buffer << data;
	return buffer.str();
}