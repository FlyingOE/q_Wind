#include "stdafx.h"
#include "multilang.h"

#include "win32.util/CodeConvert.h"
#include "types.h"
#include "util.h"
#include "type_convert.h"
#include <cassert>
#include <vector>
#include <sstream>

UINT const q::DEFAULT_CP = CP_UTF8;

std::string q::convert(UINT frCP, UINT toCP, char const* str) throw(std::runtime_error) {
	assert(str != NULL);
	return (frCP == toCP) ? str : ml::convert(frCP, toCP, str);
}

std::string q::convert(UINT frCP, UINT toCP, std::string const& str) throw(std::runtime_error) {
	return convert(frCP, toCP, str.c_str());
}

K q::convert(UINT frCP, UINT toCP, K str) throw() {
	if (str == K_NIL) {
		return error2q("unexpected nil");
	}
	else {
		K(*factory)(S) = NULL;
		switch (str->t) {
		case KC:
			factory = kp;
			break;
		case -KS:
			factory = ks;
			break;
		default:
			return error2q("not convertible to C++ string");
		}
		try {
			std::string const result = convert(frCP, toCP, q2String(str));
			return factory(const_cast<S>(result.c_str()));
		}
		catch (std::runtime_error const& error) {
			return error2q(error.what());
		}
	}
}