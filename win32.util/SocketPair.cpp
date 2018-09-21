#include "stdafx.h"
#include "SocketPair.h"

#include <cassert>
#include <iostream>
#include <sstream>

SockPair::SOCKET_ptr::SOCKET_ptr(::SOCKET socket) : socket_(socket) {
}

SockPair::SOCKET_ptr::~SOCKET_ptr() {
	reset();
}

::SOCKET SockPair::SOCKET_ptr::operator*() const {
	return socket_;
}

void SockPair::SOCKET_ptr::reset(::SOCKET socket) {
	if (isValid()) {
		::closesocket(socket_);
		socket_ = INVALID_SOCKET;
	}
	socket_ = socket;
}

::SOCKET SockPair::SOCKET_ptr::release() {
	::SOCKET socket = socket_;
	socket_ = INVALID_SOCKET;
	return socket;
}

bool SockPair::SOCKET_ptr::isValid() const {
	return socket_ != INVALID_SOCKET;
}

char const* SockPair::getError(int error) {
	switch (error) {
	case WSASYSNOTREADY:
		return "network subsystem not ready";
		return false;
	case WSAVERNOTSUPPORTED:
		return "version not supported";
	case WSAEINPROGRESS:
		return "blocking operation in progress";
	case WSAEPROCLIM:
		return "process limits exceeded";
	case WSAEFAULT:
		return "<BUG> invalid pointer";
	case WSANOTINITIALISED:
		return "WSAStartup not called";
	case WSAENETDOWN:
		return "network subsystem down";
	case WSAEAFNOSUPPORT:
		return "address family not supported";
	case WSAEMFILE:
		return "no more socket descriptors";
	case WSAEINVAL:
		return "AF_UNSPEC & missing type/protocol";
	case WSAEINVALIDPROVIDER:
		return "invalid service provider";
	case WSAENOBUFS:
		return "no more buffer space";
	case WSAEPROTONOSUPPORT:
		return "protocol not supported";
	case WSAEPROTOTYPE:
		return "wrong protocol type";
	case WSAEPROVIDERFAILEDINIT:
		return "failed to initialize service provider";
	case WSAESOCKTNOSUPPORT:
		return "socket type not supported by address family";
	case WSA_NOT_ENOUGH_MEMORY:
		return "not enough memory";
	case WSAHOST_NOT_FOUND:
		return "host not found";
	case WSANO_DATA:
		return "no data";
	case WSANO_RECOVERY:
		return "non-recoverable error";
	case WSATRY_AGAIN:
		return "temporary error";
	case WSATYPE_NOT_FOUND:
		return "specified class not found";
	case WSAEACCES:
		return "access denied";
	case WSAEADDRINUSE:
		return "address in use";
	case WSAEADDRNOTAVAIL:
		return "address not available";
	case WSAENOTSOCK:
		return "not a socket";
	case WSAENETRESET:
		return "timed out when SO_KEEPALIVE is set";
	case WSAENOPROTOOPT:
		return "option not supported";
	case WSAENOTCONN:
		return "reset when SO_KEEPALIVE is set";
	default:
		;// fall through
	}
	static std::string ERROR_MSG;
	std::ostringstream buffer;
	buffer << "unknown error " << error;
	ERROR_MSG = buffer.str();
	return ERROR_MSG.c_str();
}

bool SockPair::prepare() {
	::WSADATA wsaData;
	int result = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		std::cerr << "<WSAStartup> init failure: " << getError(result) << std::endl;
		return false;
	}
	return true;
}

bool SockPair::finalize() {
	::WSACleanup();
	return true;
}

//@see https://github.com/ncm/selectable-socketpair/
int SockPair::make_socket_pair(::SOCKET& svr, ::SOCKET& clt) {
	svr = clt = INVALID_SOCKET;

	SOCKET_ptr listener(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
	if (!listener.isValid()) {
		return ::WSAGetLastError();
	}

	// Setting SO_REUSEADDR to avoid potential Windows resource leaks
	int const reuse = 1;
	if (0 != ::setsockopt(*listener,
		SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char const*>(&reuse), sizeof(reuse))) {
		return ::WSAGetLastError();
	}
		
	union {
		::sockaddr_in addr_in;
		::sockaddr addr;
	} addr;
	::ZeroMemory(&addr, sizeof(&addr));
	addr.addr_in.sin_family = AF_INET;
	addr.addr_in.sin_addr.s_addr = ::htonl(INADDR_LOOPBACK);
	addr.addr_in.sin_port = 0;
	if (0 != ::bind(*listener, &addr.addr, sizeof(addr.addr))) {
		return ::WSAGetLastError();
	}

	::ZeroMemory(&addr, sizeof(&addr));
	::socklen_t addrLen = sizeof(addr.addr_in);
	if (0 != ::getsockname(*listener, &addr.addr, &addrLen)) {
		return ::WSAGetLastError();
	}
	else {
		addr.addr_in.sin_addr.s_addr = ::htonl(INADDR_LOOPBACK);
		addr.addr_in.sin_family = AF_INET;
	}

	if (0 != ::listen(*listener, 1)) {
		return ::WSAGetLastError();
	}

	SOCKET_ptr client(::socket(AF_INET, SOCK_STREAM, 0));
	if (!client.isValid()) {
		return ::WSAGetLastError();
	}
	if (0 != ::connect(*client, &addr.addr, sizeof(addr.addr_in))) {
		return ::WSAGetLastError();
	}

	SOCKET_ptr server(::accept(*listener, NULL, NULL));
	if (!server.isValid()) {
		return ::WSAGetLastError();
	}

	clt = client.release();
	svr = server.release();
	return 0;
}