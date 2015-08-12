#include "stdafx.h"
#include "TDB_API.h"

#include "TDB_API_helper.h"

#include "kdb+.util/types.h"
#include "kdb+.util/type_convert.h"

#include <cassert>
#include <string>

::OPEN_SETTINGS TDB::SETTINGS = {
	"localhost",	//host
	"10222",		//port
	"user",			//userName
	"password",		//password
	30,				//timeout (sec)
	2,				//retries
	0				//retry delay
};

namespace TDB {
	namespace util {

		// Helper for ::setTimeout(...)
		template <typename T>
		T& setSetting(K data, T& setting) throw(std::string) {
			if (data == K_NIL) return setting;

			long long nullValue;
			switch (data->t) {
			case -KH:
				nullValue = q::type_traits<H>::NULL_VAL;
				break;
			case -KI:
				nullValue = q::type_traits<I>::NULL_VAL;
				break;
			case -KJ:
				nullValue = q::type_traits<J>::NULL_VAL;
				break;
			default:
				throw std::string("not an integral value");
			}

			long long value = q::q2Dec(data);
			if (value == nullValue) {
				return setting;
			}
			else if ((value < std::numeric_limits<T>::min()) || (std::numeric_limits<T>::max() < value)) {
				throw std::string("out of bounds");
			}
			else {
				return setting = static_cast<T>(value);
			}
		}

	}//namespace TDB::util
}//namespace TDB


TDB_API K K_DECL getTimeout(K _) {
	return knk(3, ki(TDB::SETTINGS.nTimeOutVal), ki(TDB::SETTINGS.nRetryCount), ki(TDB::SETTINGS.nRetryGap));
}

TDB_API K K_DECL setTimeout(K timeout, K retries, K delay) {
	try {
		TDB::util::setSetting(timeout, TDB::SETTINGS.nTimeOutVal);
		TDB::util::setSetting(retries, TDB::SETTINGS.nRetryCount);
		TDB::util::setSetting(delay, TDB::SETTINGS.nRetryGap);
	}
	catch (std::string const &error) {
		return q::error2q(error);
	}
	return getTimeout(K_NIL);
}