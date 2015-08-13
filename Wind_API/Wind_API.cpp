#include "stdafx.h"
#include "Wind_API.h"

#include "kdb+.util/types.h"
#include "kdb+.util/util.h"
#include "kdb+.util/type_convert.h"

std::chrono::milliseconds Wind::ASYNC_TIMEOUT(30 * 1000L);	//30 seconds

BOOL Wind::prepare() {
	bool status = true;
	status = status && Wind::pubsub::prepare();
	return status;
}

BOOL Wind::finalize() {
	bool status = true;
	status = status && Wind::pubsub::finalize();
	return status;
}

WIND_API K K_DECL getTimeout(K _) {
	return kj(Wind::ASYNC_TIMEOUT.count());
}

WIND_API K K_DECL setTimeout(K timeout) {
	if (timeout == K_NIL) {
		return q::error2q("nil timeout");
	}
	try {
		Wind::ASYNC_TIMEOUT = std::chrono::milliseconds(1) * q::q2Dec(timeout);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}
	return getTimeout(K_NIL);
}
