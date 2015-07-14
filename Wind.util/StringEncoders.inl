#ifndef __STRINGENCODERS_H__
static_assert(0, "Include Wind.util/StringEncoders.h instead!");
#endif

#include <sstream>

template <typename T>
std::string Wind::encoder::StringizeEncoder::operator()(T const& data) const {
	std::ostringstream buffer;
	buffer << data;
	return buffer.str();
}