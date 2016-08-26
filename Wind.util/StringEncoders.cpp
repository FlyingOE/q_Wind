#include "stdafx.h"
#include "StringEncoders.h"

#include "kdb+.util/multilang.h"

#include <cassert>
#include <sstream>

std::string Wind::encoder::Passthrough::encode(char const* str) throw() {
	assert(str != NULL);
	return str;
}

std::string Wind::encoder::GB18030_UTF8::encode(char const* str) throw() {
	try {
		return ml::convert(ml::CP_GB18030, CP_UTF8, str);
	}
	catch (std::runtime_error const& error) {
		std::ostringstream buffer;
		buffer << "'" << error.what();
		return buffer.str();
	}
}