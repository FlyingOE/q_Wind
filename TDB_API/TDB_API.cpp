#include "stdafx.h"
#include "TDB_API.h"

#include "TDB_API_helper.h"

#include "kdb+.util/util.h"
#include "kdb+.util/types.h"
#include "kdb+.util/type_convert.h"

#include <cassert>
#include <string>
#include <sstream>

::OPEN_SETTINGS TDB::SETTINGS = {
	"localhost",	//host
	"10222",		//port
	"user",			//userName
	"password",		//password
	30,				//timeout (sec)
	2,				//retries
	0				//retry delay
};

std::map<::THANDLE, short> TDB::LEVELS;

short TDB::DATA_SRC = 0;

namespace TDB {
	namespace util {

		// Helper for ::setTimeout(...)
		template <typename T>
		T& setSetting(K data, T& setting) throw(std::runtime_error) {
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
				throw std::runtime_error("not an integral value");
			}

			long long value = q::q2Dec(data);
			if (value == nullValue) {
				return setting;
			}
			else if ((value < std::numeric_limits<T>::min()) || (std::numeric_limits<T>::max() < value)) {
				throw std::runtime_error("out of bounds");
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
	catch (std::runtime_error const &error) {
		return q::error2q(error.what());
	}
	return getTimeout(K_NIL);
}


TDB_API K K_DECL getDataSource(K _) {
	return kh(TDB::DATA_SRC);
}

TDB_API K K_DECL setDataSource(K dataSrc) {
	long long src = 0;
	try {
		src = q::q2Dec(dataSrc);
	}
	catch (std::runtime_error const &error) {
		return q::error2q(error.what());
	}

	if ((src < 0) || (std::numeric_limits<short>::max() < src)) {
		return q::error2q("invalid data source ID");
	}
	else {
		TDB::DATA_SRC = static_cast<short>(src);
	}

	return getDataSource(K_NIL);
}