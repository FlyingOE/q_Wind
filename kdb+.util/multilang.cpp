#include "stdafx.h"
#include "multilang.h"

#include "types.h"
#include "util.h"
#include "type_convert.h"
#include <cassert>
#include <vector>

UINT const ml::CP_GB18030 = 54936;

std::wstring ml::convert(UINT frCP, char const* inStr) {
	assert(inStr != NULL);
	size_t const len = ::MultiByteToWideChar(frCP, 0, inStr, -1, NULL, 0);
	std::vector<wchar_t> outUcs(len, L'\0');
	::MultiByteToWideChar(frCP, 0, inStr, -1, &outUcs[0], len);
	assert(outUcs[len - 1] == L'\0');	//outUcs must be properly terminated!
	return &outUcs[0];
}

std::string ml::convert(UINT toCP, wchar_t const* inUcs) {
	assert(inUcs != NULL);
	size_t const len = ::WideCharToMultiByte(toCP, 0, inUcs, -1, NULL, 0, NULL, NULL);
	std::vector<char> outStr(len, '\0');
	::WideCharToMultiByte(toCP, 0, inUcs, -1, &outStr[0], len, NULL, NULL);
	assert(outStr[len - 1] == '\0');	//outStr must be properly terminated!
	return &outStr[0];
}

//@ref http://blog.csdn.net/xiaohu_2012/article/details/14454299
std::string ml::convert(UINT frCP, UINT toCP, char const* inStr) {
	return convert(toCP, convert(frCP, inStr).c_str());
}

std::string q::convert(UINT frCP, UINT toCP, char const* str) {
	assert(str != NULL);
	return (frCP == toCP) ? str : ml::convert(frCP, toCP, str);
}

std::string q::convert(UINT frCP, UINT toCP, std::string const& str) {
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
		std::string const result = convert(frCP, toCP, q2String(str));
		return factory(const_cast<S>(result.c_str()));
	}
}