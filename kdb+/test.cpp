#include "stdafx.h"

#ifndef NDEBUG
/////////////////////////////////////////////////////////////////////////////
#include "kdb+.h"

#include <iostream>
#include <iomanip>

KDB_API K K_DECL testSerial(K a) {
	K k = kpn("000001.SZ", 9);

	//K s = b9(-1, k);
	K s = b9(1, k);

	for (G* i = kG(s); i < kG(s) + s->n; ++i) {
		std::cout << std::setiosflags(std::ios::uppercase) << std::setfill('0') << std::setw(2)
			<< std::hex << static_cast<int>(*i) << ',';
	}
	std::cout << std::endl;

#	if KX_HAS_OKX
	std::cout << "okx = " << okx(s) << std::endl;
#	endif

	if (a->g) {
		r0(s);
		return k;
	}
	else {
		r0(k);
		return s;
	}
}

/////////////////////////////////////////////////////////////////////////////
#endif//!NDEBUG