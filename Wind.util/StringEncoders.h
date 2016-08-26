#ifndef __STRINGENCODERS_H__
#define __STRINGENCODERS_H__
#pragma comment(lib, "Wind.util.lib")

#include <string>

namespace Wind {
	namespace encoder {

		struct Passthrough {
			static std::string encode(char const* str) throw();
		};

		struct GB18030_UTF8 {
			static std::string encode(char const* str) throw();
		};

		struct Stringize {
			template <typename T>
			static std::string encode(T const& data) throw();
		};

	}//namespace Wind::encoder
}//namespace Wind

#include "StringEncoders.inl"

#endif//__STRINGENCODERS_H__