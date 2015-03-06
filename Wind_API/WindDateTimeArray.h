#ifndef __WINDDATETIMEARRAY_H__
#define __WINDDATETIMEARRAY_H__
#pragma comment(lib, "WindQuantData.lib")

#include "WindQuantAPI.h"

namespace Wind {

	struct DateTimeArray : ::WQDateTimeArray {
		explicit DateTimeArray(::WQDateTimeArray const& array);
		~DateTimeArray() throw();
		void clear();
		::WQDateTimeArray release();
		K parse() const throw();

	private:
		struct qTypeTraits;
	};
	static_assert(sizeof(DateTimeArray) == sizeof(::WQDateTimeArray),
		"Invalid WQDateTimeArray wrapper! Check Wind API for changes.");

}//namespace Wind

#endif//__WINDDATETIMEARRAY_H__