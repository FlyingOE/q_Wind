#include "stdafx.h"
#include "init.h"

//@see http://pages.cs.wisc.edu/~tlabonne/memleak.html
#include "CrtDbg.h"
void activateMemoryLeakChecks() {
#	ifndef NDEBUG
	int flags = ::_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flags |= _CRTDBG_LEAK_CHECK_DF;
	::_CrtSetDbgFlag(flags);
#	endif
}

void q::onAttachProcess() {
#if 0		//only required in standalone C programs
	khp("", -1);	//force initialize kdb+ internal memory system
#endif
	setm(1);		//allow symbols to be created in threads other than the main thread
	activateMemoryLeakChecks();
}

void q::onDetachProcess() {
}

void q::onAttachThread() {
}

void q::onDetachThread() {
	m9();			//free up memory allocated for this thread's pool
}