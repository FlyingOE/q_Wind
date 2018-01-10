#ifndef __TDB_API__UTIL_H__
static_assert(0, "Include TDB_API/util.h instead!");
#endif

template <typename T>
struct TDB::Deleter {

	void operator()(T*& p) const {
		operator()(const_cast<T* const&>(p));
		p = NULL;
	}

	void operator()(T* const& p) const {
		::TDB_Free(p);
	}
};