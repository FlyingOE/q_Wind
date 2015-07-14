#include "stdafx.h"
#include "StringEncoders.h"

#include "kdb+.util/multilang.h"

std::string Wind::encoder::GB18030Encoder::operator()(char const* str) const {
	return ml::convert(ml::CP_GB18030, CP_UTF8, str);
}