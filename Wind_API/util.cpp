#include "stdafx.h"
#include "util.h"

#include "kdb+.util/util.h"
#include "kdb+.util/multilang.h"
#include "kdb+.util/type_convert.h"
#include <cassert>

std::string Wind::util::error2Text(::WQErr error) {
	TCHAR const* msg = ::WErr(error, eENG);
	static_assert(std::is_same<TCHAR, wchar_t>::value, "UNICODE/_UNICODE not defined");
	try {
		return ml::convert(q::DEFAULT_CP, msg);
	}
	catch (std::string const& ex) {
		return ex;
	}
}

char const* Wind::util::eventType2Text(::WQEventType type) {
	static char const* EVENT_TYPES[] = {
		NULL,	//@ref WindQuantAPI.h
		"eWQLogin",
		"eWQResponse",
		"eWQPartialResponse",
		"eWQErrorReport",
		"eWQOthers"
	};
	if ((0 < type) && (type < _countof(EVENT_TYPES))) {
		return EVENT_TYPES[type];
	}
	else {
		return "???";
	}
}

std::wstring Wind::util::q2tmStr(K data, std::size_t maxLen, wchar_t const* fmt) throw(std::string) {
	std::tm const tm = q::q2tm(data);
	std::vector<wchar_t> buffer(maxLen + 1, L'\0');
	assert(fmt != NULL);
	std::size_t const count = std::wcsftime(&buffer[0], buffer.size(), fmt, &tm);
	assert(count + 1 == buffer.size());
	assert(*buffer.rbegin() == L'\0');
	return &buffer[0];
}

std::vector<std::wstring> Wind::util::qList2tmStr(K data, std::size_t maxLen, wchar_t const* fmt) throw(std::string) {
	std::vector<std::tm> const tms = q::qList2tm(data);
	std::vector<std::wstring> result;
	result.reserve(tms.size());
	std::vector<wchar_t> buffer(maxLen + 1, L'\0');
	assert(fmt != NULL);
	for (std::size_t i = 0; i < tms.size(); ++i) {
		std::size_t const count = std::wcsftime(&buffer[0], buffer.size(), fmt, &tms[i]);
		assert(count + 1 == buffer.size());
		assert(*buffer.rbegin() == L'\0');
		result.push_back(&buffer[0]);
	}
	return result;
}

std::wstring Wind::util::q2DateStr(K data) throw(std::string) {
	return q2tmStr(data, 10, L"%Y-%m-%d");
}

std::vector<std::wstring> Wind::util::qList2DateStr(K data) throw(std::string) {
	return qList2tmStr(data, 10, L"%Y-%m-%d");
}

std::wstring Wind::util::q2DateTimeStr(K data) throw(std::string) {
	return q2tmStr(data, 10 + 1 + 8, L"%Y-%m-%d %H:%M:%S");
}

std::vector<std::wstring> Wind::util::qList2DateTimeStr(K data) throw(std::string) {
	return qList2tmStr(data, 10 + 1 + 8, L"%Y-%m-%d %H:%M:%S");
}