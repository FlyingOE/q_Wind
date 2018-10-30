#include "stdafx.h"
#include "kdb+.h"

#include "kdb+.util/util.h"

KDB_API K K_DECL version(K _) {
	return q::version("$Id$");
}