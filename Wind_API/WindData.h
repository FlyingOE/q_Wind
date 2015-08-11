#ifndef __WINDDATA_H__
#define __WINDDATA_H__
#pragma comment(lib, "WindQuantData.lib")

#include "WindQuantAPI.h"

namespace Wind {

	struct Data : ::WQData {
		explicit Data(::WQData const& data);
		explicit Data(::WQData& data);	//NOTE: Takes over data ownership!
		~Data() throw();
		void clear();
		::WQData release();
		K parse() const throw();
	private:
		void copyData(::VARIANT& dst, ::VARIANT const& src);
		void moveData(::VARIANT& dst, ::VARIANT& src);
	};
	static_assert(sizeof(Data) == sizeof(::WQData),
		"Invalid WQData wrapper! Check Wind API for changes.");

}//namespace Wind

#endif//__WQEVENTDATA_H__