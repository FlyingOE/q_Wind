#include "stdafx.h"
#include "TDF_API.h"

#include "TDF_API_handler.h"

#include "kdb+.util/type_traits.h"
#include "kdb+.util/type_convert.h"

#include <cassert>
#include <string>

::TDF_OPEN_SETTING_EXT TDF::SETTINGS = {
	{ 0 }, 0,
	&TDF::onDataMsg, &TDF::onSystemMsg,
	"BBQ", "", 0,
	0xFFFFFFFF,
	__COUNTER__
};

namespace TDF {
	namespace util {

		// Helper for ::setTimeout(...)
		void setEnv(::TDF_ENVIRON_SETTING env, K data) throw(std::string) {
			if (data == K_NIL) return;

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
				return;
			}
			else if ((value < std::numeric_limits<unsigned int>::min()) || (std::numeric_limits<unsigned int>::max() < value)) {
				throw std::string("out of bounds");
			}
			else {
				::TDF_SetEnv(env, static_cast<unsigned int>(value));
			}
		}

	}//namespace TDF::util
}//namespace TDF


TDF_API K K_DECL setTimeout(K hbInterval, K hbMissing, K openTimeout) {
	try {
		TDF::util::setEnv(::TDF_ENVIRON_HEART_BEAT_INTERVAL, hbInterval);
		TDF::util::setEnv(::TDF_ENVIRON_MISSED_BEART_COUNT, hbMissing);
		TDF::util::setEnv(::TDF_ENVIRON_OPEN_TIME_OUT, openTimeout);
	}
	catch (std::string const &error) {
		return q::error2q(error);
	}
	return K_NIL;
}