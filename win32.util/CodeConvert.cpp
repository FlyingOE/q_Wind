#include "stdafx.h"
#include "CodeConvert.h"

#include <cassert>
#include <vector>
#include <sstream>

UINT const ml::CP_GB18030 = 54936;

std::ostringstream getEncodeErrorInfo(char const* prefix, DWORD const error) {
	std::ostringstream buffer;
	buffer << prefix << ':';
	switch (error) {
	case ERROR_INSUFFICIENT_BUFFER:
		buffer << "INSUFFICIENT_BUFFER";
		break;
	case ERROR_INVALID_FLAGS:
		buffer << "INVALID_FLAGS";
		break;
	case ERROR_INVALID_PARAMETER:
		buffer << "INVALID_PARAMETER";
		break;
	case ERROR_NO_UNICODE_TRANSLATION:
		buffer << "NO_UNICODE_TRANSLATION";
		break;
	default:
		buffer << '[' << error << ']';
	}
	return buffer;
}

std::runtime_error getEncodeError(char const* prefix, DWORD const error, char const* str) {
	std::ostringstream buffer = getEncodeErrorInfo(prefix, error);
	if (NULL != str) buffer << " \"" << str << '"';
	return std::runtime_error(buffer.str());
}

std::runtime_error getEncodeError(char const* prefix, DWORD const error, wchar_t const* str) {
	std::ostringstream buffer = getEncodeErrorInfo(prefix, error);
	if (NULL != str) {
		size_t const len = std::wcslen(str) * sizeof(*str);
		std::vector<char> strBuf(len + 1, '\0');
		std::copy(reinterpret_cast<char const*>(str), reinterpret_cast<char const*>(str)+len, strBuf.begin());
		buffer << " \"" << &strBuf[0] << '"';
	}
	return std::runtime_error(buffer.str());
}

std::wstring ml::convert(UINT frCP, char const* inStr) throw(std::runtime_error) {
	assert(inStr != NULL);
	if (std::strlen(inStr) == 0) return L"";
	size_t const len = ::MultiByteToWideChar(frCP, MB_ERR_INVALID_CHARS, inStr, -1, NULL, 0);
	if (len == 0) {
		throw getEncodeError("MultiByteToWideChar<<", ::GetLastError(), inStr);
	}
	else if (len >= static_cast<size_t>(std::numeric_limits<int>::max())) {
		throw std::runtime_error("string too long for MultiByteToWideChar!");
	}
	std::vector<wchar_t> outUcs(len, L'\0');
	int const result = ::MultiByteToWideChar(frCP, MB_ERR_INVALID_CHARS, inStr, -1, &outUcs[0], static_cast<int>(len));
	if (result == 0) {
		throw getEncodeError("MultiByteToWideChar>>", ::GetLastError(), inStr);
	}
	assert(result == len);
	assert(outUcs[len - 1] == L'\0');	//outUcs must be properly terminated!
	return &outUcs[0];
}

std::string ml::convert(UINT toCP, wchar_t const* inUcs) throw(std::runtime_error) {
	assert(inUcs != NULL);
	if (std::wcslen(inUcs) == 0) return "";
	DWORD const flags = (CP_UTF8 == toCP) ? WC_ERR_INVALID_CHARS : 0;
	size_t const len = ::WideCharToMultiByte(toCP, flags, inUcs, -1, NULL, 0, NULL, NULL);
	if (len == 0) {
		throw getEncodeError("WideCharToMultiByte<<", ::GetLastError(), inUcs);
	}
	else if (len >= static_cast<size_t>(std::numeric_limits<int>::max())) {
		throw std::runtime_error("string too long for WideCharToMultiByte!");
	}
	std::vector<char> outStr(len, '\0');
	int const result = ::WideCharToMultiByte(toCP, flags, inUcs, -1, &outStr[0], static_cast<int>(len), NULL, NULL);
	if (result == 0) {
		throw getEncodeError("WideCharToMultiByte>>", ::GetLastError(), inUcs);
	}
	assert(result == len);
	assert(outStr[len - 1] == '\0');	//outStr must be properly terminated!
	return &outStr[0];
}

//@ref http://blog.csdn.net/xiaohu_2012/article/details/14454299
std::string ml::convert(UINT frCP, UINT toCP, char const* inStr) throw(std::runtime_error) {
	return convert(toCP, convert(frCP, inStr).c_str());
}
