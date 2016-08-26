#include "stdafx.h"
#include "multilang.h"

#include "types.h"
#include "util.h"
#include "type_convert.h"
#include <cassert>
#include <vector>
#include <sstream>

UINT const q::DEFAULT_CP = CP_UTF8;

UINT const ml::CP_GB18030 = 54936;

std::runtime_error getEncodeError(char const* prefix, DWORD const error) {
	std::ostringstream buffer;
	buffer << prefix << ':';
	switch (error) {
	case ERROR_INSUFFICIENT_BUFFER:
		buffer << "ERROR_INSUFFICIENT_BUFFER";
		break;
	case ERROR_INVALID_FLAGS:
		buffer << "ERROR_INVALID_FLAGS";
		break;
	case ERROR_INVALID_PARAMETER:
		buffer << "ERROR_INVALID_PARAMETER";
		break;
	case ERROR_NO_UNICODE_TRANSLATION:
		buffer << "ERROR_NO_UNICODE_TRANSLATION";
		break;
	default:
		buffer << '[' << error << ']';
	}
	return std::runtime_error(buffer.str());
}

std::wstring ml::convert(UINT frCP, char const* inStr) throw(std::runtime_error) {
	assert(inStr != NULL);
	if (std::strlen(inStr) == 0) return L"";
	size_t const len = ::MultiByteToWideChar(frCP, MB_ERR_INVALID_CHARS, inStr, -1, NULL, 0);
	if (len == 0) {
		throw getEncodeError("MultiByteToWideChar<0>", ::GetLastError());
	}
	std::vector<wchar_t> outUcs(len, L'\0');
	int const result = ::MultiByteToWideChar(frCP, MB_ERR_INVALID_CHARS, inStr, -1, &outUcs[0], len);
	if (result == 0) {
		throw getEncodeError("MultiByteToWideChar<1>", ::GetLastError());
	}
	assert(result == len);
	assert(outUcs[len - 1] == L'\0');	//outUcs must be properly terminated!
	return &outUcs[0];
}

std::string ml::convert(UINT toCP, wchar_t const* inUcs) throw(std::runtime_error) {
	assert(inUcs != NULL);
	if (std::wcslen(inUcs) == 0) return "";
	size_t const len = ::WideCharToMultiByte(toCP, WC_ERR_INVALID_CHARS, inUcs, -1, NULL, 0, NULL, NULL);
	if (len == 0) {
		throw getEncodeError("WideCharToMultiByte<0>", ::GetLastError());
	}
	std::vector<char> outStr(len, '\0');
	int const result = ::WideCharToMultiByte(toCP, WC_ERR_INVALID_CHARS, inUcs, -1, &outStr[0], len, NULL, NULL);
	if (result == 0) {
		throw getEncodeError("WideCharToMultiByte<1>", ::GetLastError());
	}
	assert(result == len);
	assert(outStr[len - 1] == '\0');	//outStr must be properly terminated!
	return &outStr[0];
}

//@ref http://blog.csdn.net/xiaohu_2012/article/details/14454299
std::string ml::convert(UINT frCP, UINT toCP, char const* inStr) throw(std::runtime_error) {
	return convert(toCP, convert(frCP, inStr).c_str());
}

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