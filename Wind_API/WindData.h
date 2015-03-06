#ifndef __WINDDATA_H__
#define __WINDDATA_H__
#pragma comment(lib, "WindQuantData.lib")

#include "WindQuantAPI.h"

namespace Wind {

	struct Data : ::WQData {
		explicit Data(::WQData const& data);
		~Data() throw();
		void clear();
		::WQData release();
		K parse() const throw();
	};
	static_assert(sizeof(Data) == sizeof(::WQData),
		"Invalid WQData wrapper! Check Wind API for changes.");

}//namespace Wind

#endif//__WQEVENTDATA_H__