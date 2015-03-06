#ifndef __WINDCODEARRAY_H__
#define __WINDCODEARRAY_H__
#pragma comment(lib, "WindQuantData.lib")

#include "WindQuantAPI.h"

namespace Wind {

	struct WindCodeArray : ::WQWindCodeArray {
		explicit WindCodeArray(::WQWindCodeArray const& array);
		~WindCodeArray() throw();
		void clear();
		::WQWindCodeArray release();
		K parse() const throw();

	private:
		struct qTypeTraits;
	};
	static_assert(sizeof(WindCodeArray) == sizeof(::WQWindCodeArray),
		"Invalid WQWindCodeArray wrapper! Check Wind API for changes.");

}//namespace Wind

#endif//__WINDCODEARRAY_H__