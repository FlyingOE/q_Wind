#include "stdafx.h"
#include "kdb+.h"

#include "kdb+.util/multilang.h"

KDB_API K K_DECL gb18030_utf8(K strGBK) {
	return q::convert(ml::CP_GB18030, CP_UTF8, strGBK);
}

KDB_API K K_DECL utf8_gb18030(K strUTF) {
	return q::convert(CP_UTF8, ml::CP_GB18030, strUTF);
}