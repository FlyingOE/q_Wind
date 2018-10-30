#include "stdafx.h"
#include "TDB_API.h"

#include "kdb+.util/util.h"

TDB_API K K_DECL version(K _) {
	return q::version("$Id$");
}