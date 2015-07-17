#ifndef __STRINGENCODERS_H__
static_assert(0, "Include Wind.util/StringEncoders.h instead!");
#endif

#include <sstream>

template <typename T>
std::string Wind::encoder::Stringize::encode(T const& data) {
	std::ostringstream buffer;
	buffer << data;
	return buffer.str();
}