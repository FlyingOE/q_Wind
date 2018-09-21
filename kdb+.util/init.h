#ifndef __INIT_H__
#define __INIT_H__
#pragma comment(lib, "kdb+.util.lib")

/**
 * @see http://code.kx.com/q//interfaces/c-client-for-q/
 */
namespace q {

	void onAttachProcess();
	void onDetachProcess();

	void onAttachThread();
	void onDetachThread();

}//namespace q

#endif//__INIT_H__