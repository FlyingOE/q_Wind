#define KXVER 3
#include <k.h>

#include "hack.h"

#ifdef NO_Q_LIB
static_assert(0, "kdb+ hacks must be compiled without NO_Q_LIB!");
#endif

K q::krr(S const error) {
	return ::krr(error);
}

K q::orr(S const error) {
	return ::orr(error);
}