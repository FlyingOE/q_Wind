#include "stdafx.h"
#include "Wind_API.h"

#include <string>

WIND_API K K_DECL version(K _) {
	std::string const id("$Id$");
	return kp(const_cast<S>((id + " @ " __DATE__ " " __TIME__).c_str()));
}