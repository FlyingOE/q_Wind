#include "stdafx.h"
#include "Wind_API.h"

#include "util.h"
#include "WindEvent.h"

#include "kdb+.util/util.h"
#include "kdb+.util/K_ptr.h"
#include "kdb+.util/type_convert.h"
#if KX_USE_OKX
#include "win32.util/util.h"
#endif
#include "win32.util/SocketPair.h"
#include <iostream>
#include <mutex>	//C++11
#include <map>

namespace Wind {
	namespace pubsub {

		bool prepare() {
			return SockPair::prepare();
		}

		bool finalize() {
			return SockPair::finalize();
		}

		// This is essentially a concurrent map...
		//NOTE: All std::map<> interface are _not_ protected! Use only the member functions wrapped below.
		class CallbackRegistry : private std::map<::WQID, std::string> {
		public:
			std::string& operator[](::WQID qid) {
				lock_guard guard(lock_);
				return parent::operator[](qid);
			}
			size_type erase(::WQID qid) {
				lock_guard guard(lock_);
				return parent::erase(qid);
			}
		private:
			typedef std::map<::WQID, std::string> parent;
			typedef std::lock_guard<std::mutex> lock_guard;
			mutable std::mutex lock_;
		};

		static CallbackRegistry REGISTRY;
		size_t const CLIENT = 0;
		size_t const SERVER = 1;

		// Data processor (executed within q's main thread)
		K invokeCallback(I socket) {
#		if _WIN64
			::SOCKET sock = static_cast<::SOCKET>(socket);
#		else
			static_assert(sizeof(::SOCKET) == sizeof(I), "SOCKET vs I: type mismatch!");
			::SOCKET sock = socket;
#		endif
			assert(sock != INVALID_SOCKET);

			// Receive (WQID, len, serialized_K) from subscription thread
#			define RECV_CHECK(expectedSize, errorMsg)	\
			if (recvd != (expectedSize)) {	\
				std::cerr << "<recv> " << (errorMsg) << ": " << recvd << " < " << (expectedSize) << std::endl;	\
				return K_NIL;	\
			}
			::WQID qid = 0;
			int recvd = ::recv(sock, reinterpret_cast<char*>(&qid), sizeof(::WQID), MSG_WAITALL);
			RECV_CHECK(sizeof(::WQID), "WQID incomplete");
			size_t len = 0;
			recvd = ::recv(sock, reinterpret_cast<char*>(&len), sizeof(len), MSG_WAITALL);
			RECV_CHECK(sizeof(len), "size incomplete");
			if (len > static_cast<size_t>(std::numeric_limits<int>::max())) {
				std::cerr << "<recv> serialized data (" << len << ") > 2G" << std::endl;
				return K_NIL;
			}
			q::K_ptr serialized(ktn(KB, len));
			std::memset(kG(serialized.get()), 0, len);
			recvd = ::recv(sock, reinterpret_cast<char*>(kG(serialized.get())), static_cast<int>(len), MSG_WAITALL);
			RECV_CHECK(len, "data incomplete");
#			undef RECV_CHECK

			// Deserialize K object
#			if KX_USE_OKX
			//@ref https://groups.google.com/forum/#!topic/personal-kdbplus/pjsugT7590A
			if (!okx(serialized.get())) {
				std::cerr << "<recv> bad data: ["
					<< ::util::hexBytes(&kG(serialized.get())[0], static_cast<size_t>(serialized->n))
					<< ']' << std::endl;
				return K_NIL;
			}
#			endif
			q::K_ptr result(d9(serialized.get()));

			// Identify the origial query and callback
			std::string const callback = REGISTRY[qid];
			if (callback.empty()) {
				std::cerr << "unknown WQID: " << qid << std::endl;
				return K_NIL;
			}

			static_assert(std::is_same<::WQID, J>::value, "WQID data type mismatch");
			q::K_ptr output(k(0, const_cast<S>(callback.c_str()), kj(qid), result.release(), K_NIL));
			if (output->t == -128) {
				std::cerr << "<q> '" << output->s << std::endl;
			}
			return output.release();
		}

		// Result publisher (executed within subscription thread)
		bool publishResult(Event const& event, SockPair::SOCKET_ptr const* socks) {
			// Convert Wind event into K object
			std::string error;
			q::K_ptr data;
			if (event.ErrCode != WQERR_OK) {
				std::cerr << "<WQ> subscription error: " << util::error2Text(event.ErrCode) << std::endl;
				return false;
			}
			try {
				data.reset(event.parse());
			}
			catch (std::runtime_error const& ex) {
				std::cerr << "<WQ> response format error: " << ex.what() << std::endl;
				return false;
			}

			// Serialize K object into bytes
			static_assert(std::is_same<::WQID, J>::value, "WQID data type mismatch");
#			if KX_USE_OKX
			q::K_ptr serialized(b9(1, data.get()));
#			else
			q::K_ptr serialized(b9(-1, data.get()));
#			endif
			assert((serialized->t == KG) && (serialized->n > 0));

			// Send tuple (WQID, len, serialized_K) over to the main thread
#			define SEND_CHECK(expectedSize, errorMsg)	\
			if (sent != (expectedSize)) {	\
				std::cerr << "<send> " << (errorMsg) << ": " << sent << " < " << (expectedSize) << std::endl;	\
				return false;	\
			}
			int sent = ::send(*socks[SERVER], reinterpret_cast<char const*>(&event.RequestID), sizeof(::WQID), 0);
			SEND_CHECK(sizeof(::WQID), "WQID incomplete");
			size_t const len = static_cast<size_t>(serialized->n);
			if (len > static_cast<size_t>(std::numeric_limits<int>::max())) {
				std::cerr << "<send> serialized data (" << len << ") > 2G" << std::endl;
				return false;
			}
			sent = ::send(*socks[SERVER], reinterpret_cast<char const*>(&len), sizeof(len), 0);
			SEND_CHECK(sizeof(len), "size incomplete");
			sent = ::send(*socks[SERVER], reinterpret_cast<char const*>(kG(serialized.get())), static_cast<int>(len), 0);
			SEND_CHECK(serialized->n, "data incomplete");
#			undef SEND_CHECK

			return true;
		}

		int WINAPI subscribe(::WQEvent* pEvent, LPVOID lpUserParam) {
			SockPair::SOCKET_ptr* socks = static_cast<SockPair::SOCKET_ptr*>(lpUserParam);
			assert(socks != NULL);
			
			assert(pEvent != NULL);
			switch (pEvent->EventType) {
			case eWQPartialResponse:
				return publishResult(static_cast<Event&>(*pEvent), socks);
			case eWQErrorReport:
				std::cerr << "<WQ> error report: " << util::error2Text(pEvent->ErrCode) << std::endl;
				return false;
			case eWQOthers:
				// Detect unsubscribe and free up socket pair
				if (pEvent->ErrCode == WQERR_USER_CANCEL) {
					assert(*socks[CLIENT] <= std::numeric_limits<I>::max());
					sd0(static_cast<I>(*socks[CLIENT]));
					REGISTRY.erase(pEvent->RequestID);
					delete[] socks;
					return true;
				}
				else {
					// fall through
				}
			default:
				std::cerr << "<WQ> unsupported subscription response: " << *pEvent << std::endl;
				return false;
			}
		}

	}//namespace Wind::pubsub
}//namespace Wind

K Wind_subscribe(::WQID(WINAPI *func)(LPCWSTR, LPCWSTR, LPCWSTR, ::IEventHandler, LPVOID),
	K windCodes, K indicators, K params, K callback)
{
	assert(NULL != func);
	std::wstring codes, indis, paras;
	std::string cb;
	try {
		codes = Wind::util::qList2WStringJoin(windCodes, L',');
		indis = Wind::util::qList2WStringJoin(indicators, L',');
		paras = Wind::util::qDict2WStringMapJoin(params, L';', L'=');
		cb = q::q2String(callback);
	}
	catch (std::runtime_error const& error) {
		return q::error2q(error.what());
	}

	// Create async socket pair
	std::unique_ptr<SockPair::SOCKET_ptr[]> socks(new ::SockPair::SOCKET_ptr[2]);
	::SOCKET server, client;
	int result = SockPair::make_socket_pair(server, client);
	if (result != 0) {
		std::ostringstream buffer;
		buffer << "<SockPair> " << SockPair::getError(result);
		return q::error2q(buffer.str());
	}
#	if _WIN64
	if (client > std::numeric_limits<I>::max()) {
		return q::error2q("SOCKET address out of range for q callback invocation!");
	}
#	else
	static_assert(sizeof(::SOCKET) == sizeof(I), "SOCKET vs I: type mismatch!");
#	endif
	using Wind::pubsub::SERVER;
	using Wind::pubsub::CLIENT;
	socks[SERVER].reset(server);
	socks[CLIENT].reset(client);

	// Setup subscription
	::WQID const qid = func(codes.c_str(), indis.c_str(), paras.c_str(),
		&Wind::pubsub::subscribe, socks.get());
	if (qid <= 0) {
		std::ostringstream buffer;
		buffer << "<WQ> subscription error: " << Wind::util::error2Text(static_cast<::WQErr>(qid));
		return q::error2q(buffer.str());
	}
	else {
		assert(*socks[CLIENT] <= std::numeric_limits<I>::max());
		sd1(static_cast<I>(*socks[CLIENT]), &Wind::pubsub::invokeCallback);
		Wind::pubsub::REGISTRY[qid] = cb;
		socks.release();
		static_assert(std::is_same<::WQID, J>::value, "WQID data type mismatch");
		return kj(qid);
	}
}

WIND_API K K_DECL Wind_wsq(K windCodes, K indicators, K params, K callback) {
	if ((callback != K_NIL) && (callback->t == 101) && (callback->g == 0)) {
		// (::) => strike
		return Wind_wsq_strike(windCodes, indicators, params);
	}
	else {
		// (`*) => subscribe
		return Wind_subscribe(&::WSQ, windCodes, indicators, params, callback);
	}
}

WIND_API K K_DECL Wind_tdq(K windCodes, K indicators, K params, K callback) {
	//*/
	return q::error2q("<WQ> nyi");
	/*/
	return Wind_subscribe(&::TDQ, windCodes, indicators, params, callback);
	//*/
}

WIND_API K K_DECL Wind_cr(K qid) {
	::WQID id = -1;
	static_assert(std::is_same<::WQID, J>::value, "WQID data type mismatch");
	try {
		id = q::q2Dec(qid);
	}
	catch (std::runtime_error const& error) {
		return q::error2q(error.what());
	}

	::WQErr const error = ::CancelRequest(id);
	if (error != WQERR_OK) {
		std::ostringstream buffer;
		buffer << "<WQ> cancellation error: " << Wind::util::error2Text(error);
		return q::error2q(buffer.str());
	}
	else {
		return K_NIL;
	}
}

WIND_API K K_DECL Wind_car(K _) {
	::WQErr const error = ::CancelAllRequest();
	if (error != WQERR_OK) {
		std::ostringstream buffer;
		buffer << "<WQ> cancellation error: " << Wind::util::error2Text(error);
		return q::error2q(buffer.str());
	}
	else {
		return K_NIL;
	}
}
