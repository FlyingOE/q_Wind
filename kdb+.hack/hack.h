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
#	pragma comment(lib, "kdb+.hack.lib")
#endif

#endif//__KDB_HACK__HACK_H__