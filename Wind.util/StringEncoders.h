#ifndef __STRINGENCODERS_H__
#define __STRINGENCODERS_H__
#pragma comment(lib, "Wind.util.lib")

#include <string>

namespace Wind {
	namespace encoder {

		struct PassthruEncoder {
			std::string operator()(char const* str) const { return str; }
		};

		struct GB18030Encoder {
			std::string operator()(char const* str) const;
		};

		struct StringizeEncoder {
			template <typename T>
			std::string operator()(T const& data) const;
		};

	}//namespace Wind::encoder
}//namespace Wind

#include "StringEncoders.inl"

#endif//__STRINGENCODERS_H__