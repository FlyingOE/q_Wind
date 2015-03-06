#ifndef __WINDFIELDSARRAY_H__
#define __WINDFIELDSARRAY_H__
#pragma comment(lib, "WindQuantData.lib")

#include "WindQuantAPI.h"

namespace Wind {

	struct WindFieldsArray : ::WQWindFieldsArray {
		explicit WindFieldsArray(::WQWindFieldsArray const& array);
		~WindFieldsArray() throw();
		void clear();
		::WQWindFieldsArray release();
		K parse() const throw();

	private:
		struct qTypeTraits;
	};
	static_assert(sizeof(WindFieldsArray) == sizeof(::WQWindFieldsArray),
		"Invalid WQWindFieldsArray wrapper! Check Wind API for changes.");

}//namespace Wind

#endif//__WINDFIELDSARRAY_H__