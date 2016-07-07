#include "stdafx.h"
#include "TDB_API.h"

#include <string>

TDB_API K K_DECL version(K _) {
	std::string const id("$Id$");
	return kp(const_cast<S>((id + " @ " __DATE__ " " __TIME__).c_str()));
}