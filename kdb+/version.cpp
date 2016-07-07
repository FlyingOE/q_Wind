#include "stdafx.h"
#include "kdb+.h"

#include <string>

KDB_API K K_DECL version(K _) {
	std::string const ver("$Id$");
	return kp(const_cast<S>(ver.c_str()));
}