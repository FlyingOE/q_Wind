#include "stdafx.h"
#include "StringEncoders.h"

#include "kdb+.util/multilang.h"

#include <cassert>

std::string Wind::encoder::Passthrough::encode(char const* str) {
	assert(str != NULL);
	return str;
}

std::string Wind::encoder::GB18030_UTF8::encode(char const* str) {
	return ml::convert(ml::CP_GB18030, CP_UTF8, str);
}