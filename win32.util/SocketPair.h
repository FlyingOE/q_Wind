#ifndef __SOCKETPAIR_H__
#define __SOCKETPAIR_H__
#pragma comment(lib, "win32.util.lib")

#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

namespace SockPair {

	// A std::auto_ptr<>-like manager for ::SOCKET
	class SOCKET_ptr {
	public:
		SOCKET_ptr(::SOCKET socket = INVALID_SOCKET);
		~SOCKET_ptr();
		::SOCKET operator*() const;
		void reset(::SOCKET socket = INVALID_SOCKET);
		::SOCKET release();
		bool isValid() const;
	private:
		::SOCKET socket_;
	};

	char const* getError(int error);

	bool prepare();
	bool finalize();

	int make_socket_pair(::SOCKET& svr, ::SOCKET& clt);

}//namespace SockPair

#endif//__SOCKETPAIR_H__