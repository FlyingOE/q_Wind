#include "stdafx.h"
#include "util.h"

#include "kdb+.util/util.h"
#include "kdb+.util/type_convert.h"
#include "win32.util/CodeConvert.h"
#include <cassert>
#ifdef _MSC_VER
//@ref https://msdn.microsoft.com/en-us/library/dn607301.aspx
#include <iterator>	//MSVC: stdext::make_unchecked_array_iterator
#endif
#include <sstream>

std::string Wind::util::error2Text(::WQErr error) {
	TCHAR const* msg = ::WErr(error, eENG);
	static_assert(std::is_same<TCHAR, wchar_t>::value, "UNICODE/_UNICODE not defined");
	std::ostringstream buffer;
	buffer << '(' << error << ") ";
	try {
		buffer << ml::convert(q::DEFAULT_CP, msg);
	}
	catch (std::runtime_error const& ex) {
		buffer << "'" << ex.what();
	}
	return buffer.str();
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

std::wstring Wind::util::q2tmStr(K data, size_t maxLen, wchar_t const* fmt) throw(std::runtime_error) {
	// Special treatment of millisecond format
	std::vector<wchar_t> f(fmt, fmt + std::wcslen(fmt) + 1);
	while (wchar_t* p = std::wcsstr(&f[0], L"%##")) {
		*p = L'#';
	}

	q::tm_ext const tm = q::q2tm(data);
	std::vector<wchar_t> buffer(maxLen + 1, L'\0');
	assert(fmt != NULL);
	size_t const count = std::wcsftime(&buffer[0], buffer.size(), &f[0], &tm);
	assert(count + 1 == buffer.size());
	assert(*buffer.rbegin() == L'\0');

	// Patch millisecond format
	wchar_t ms[4] = { L'\0' };
	auto const pms = stdext::make_unchecked_array_iterator(ms);
	std::swprintf(ms, _countof(ms), L"%03d", tm.tm_millis);
	while (wchar_t* p = std::wcsstr(&buffer[0], L"###")) {
		std::copy(pms, pms + 3, stdext::make_unchecked_array_iterator(p));
	}
	
	return &buffer[0];
}

std::vector<std::wstring> Wind::util::qList2tmStr(K data, size_t maxLen, wchar_t const* fmt) throw(std::runtime_error) {
	// Special treatment of millisecond format
	std::vector<wchar_t> f(fmt, fmt + std::wcslen(fmt) + 1);
	while (wchar_t* p = std::wcsstr(&f[0], L"%##")) {
		*p = L'#';
	}

	std::vector<q::tm_ext> const tms = q::qList2tm(data);
	std::vector<std::wstring> result;
	result.reserve(tms.size());
	std::vector<wchar_t> buffer(maxLen + 1, L'\0');
	wchar_t ms[4] = { L'\0' };
	auto const pms = stdext::make_unchecked_array_iterator(ms);
	assert(fmt != NULL);
	for (size_t i = 0; i < tms.size(); ++i) {
		size_t const count = std::wcsftime(&buffer[0], buffer.size(), &f[0], &tms[i]);
		assert(count + 1 == buffer.size());
		assert(*buffer.rbegin() == L'\0');

		// Patch millisecond format
		std::swprintf(ms, _countof(ms), L"%03d", tms[i].tm_millis);
		while (wchar_t* p = std::wcsstr(&buffer[0], L"###")) {
			std::copy(pms, pms + 2, stdext::make_unchecked_array_iterator(p));
		}

		result.push_back(&buffer[0]);
	}
	return result;
}

std::wstring Wind::util::q2DateStr(K data) throw(std::runtime_error) {
	return q2tmStr(data, 10, L"%Y-%m-%d");
}

std::vector<std::wstring> Wind::util::qList2DateStr(K data) throw(std::runtime_error) {
	return qList2tmStr(data, 10, L"%Y-%m-%d");
}

std::wstring Wind::util::q2DateTimeStr(K data) throw(std::runtime_error) {
	return q2tmStr(data, 10 + 1 + 8 + 1 + 3, L"%Y-%m-%d %H:%M:%S.%##");
}

std::vector<std::wstring> Wind::util::qList2DateTimeStr(K data) throw(std::runtime_error) {
	return qList2tmStr(data, 10 + 1 + 8 + 1 + 3, L"%Y-%m-%d %H:%M:%S.%##");
}

std::wstring Wind::util::q2StrOrX(K data, std::wstring(*alt)(K)) throw(std::runtime_error) {
	if (data == K_NIL) {
		throw std::runtime_error("nil symbol or date or time or datetime");
	}
	switch (data->t) {
	case -KS:
	case KC:
		return q::q2WString(data);
	default:
		return alt(data);
	}
}