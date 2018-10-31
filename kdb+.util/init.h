#ifndef __INIT_H__
#define __INIT_H__

/**
 * @see http://code.kx.com/q//interfaces/c-client-for-q/
 */
namespace q {

	void onAttachProcess();
	void onDetachProcess();

	void onAttachThread();
	void onDetachThread();

}//namespace q

#ifdef NDEBUG
#	pragma comment(lib, "kdb+.util.lib")
#else
#	pragma comment(lib, "kdb+.utild.lib")
#endif

#endif//__INIT_H__