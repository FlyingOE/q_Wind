#ifndef __KDB_HACK__HACK_H__
#define __KDB_HACK__HACK_H__

namespace q {
	K krr(S const error);
	K orr(S const error);
}

#ifdef NO_Q_LIB
	K q::krr(S const error) { return static_cast<K>(0); }
	K q::orr(S const error) { return static_cast<K>(0); }
#else
#   ifdef NDEBUG
#       pragma comment(lib, "kdb+.hack.lib")
#   else
#       pragma comment(lib, "kdb+.hackd.lib")
#   endif
#endif

#endif//__KDB_HACK__HACK_H__