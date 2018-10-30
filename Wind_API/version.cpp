#include "stdafx.h"
#include "Wind_API.h"

#include "kdb+.util/util.h"

WIND_API K K_DECL version(K _) {
	return q::version("$Id$");
}