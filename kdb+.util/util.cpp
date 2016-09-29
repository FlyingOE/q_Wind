#include "stdafx.h"
#include "util.h"

#include "kdb+.hack/hack.h"
#include "win32.util/CodeConvert.h"

#include <cassert>
#include <algorithm>

namespace q {
	namespace impl {
		static std::string ERROR_STR;	//TODO how to get rid of this static buffer?
	}
}

K q::error2q(char const* error, bool isSystemError) throw() {
	impl::ERROR_STR = error;
	K(*const reporter)(S const) = isSystemError ? q::orr : q::krr;
	return reporter(const_cast<S>(impl::ERROR_STR.c_str()));
}

K q::error2q(std::string const& error, bool isSystemError) throw() {
	return error2q(error.c_str(), isSystemError);
}

K q::error2q(wchar_t const* error, bool isSystemError, UINT toCP) throw() {
	return error2q(ml::convert(toCP, error), isSystemError);
}

K q::error2q(std::wstring const& error, bool isSystemError, UINT toCP) throw() {
	return error2q(error.c_str(), isSystemError, toCP);
}