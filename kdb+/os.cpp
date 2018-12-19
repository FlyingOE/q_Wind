#include "stdafx.h"
#include "kdb+.h"

#include "kdb+.util/util.h"
#include "kdb+.util/types.h"
#include "win32.util/CodeConvert.h"
#include <cassert>
#include <vector>

namespace {

	K getStringResult(DWORD result, std::vector<wchar_t>& buffer) {
		if (FAILED(result)) {
			result = ::GetLastError();
			if (FAILED(result)) {
				std::swprintf(&buffer[0], buffer.size(), L"unknown error");
			}
			else {
				std::swprintf(&buffer[0], buffer.size(), L"0x%8X", result);
			}
		}
		assert(*buffer.rbegin() == L'\0');	// buffer must be properly terminated
		try {
			std::string const path = ml::convert(q::DEFAULT_CP, &buffer[0]);
			return kp(const_cast<S>(path.c_str()));
		}
		catch(std::runtime_error const& error) {
			return q::error2q(error.what());
		}
	}

}//namespace <anonymous>

KDB_API K K_DECL cwd(K _) {
	std::vector<wchar_t> buffer(MAX_PATH, L'\0');
	assert(MAX_PATH <= std::numeric_limits<DWORD>::max());
	DWORD result = ::GetCurrentDirectoryW(static_cast<DWORD>(buffer.size()), &buffer[0]);
	return getStringResult(result, buffer);
}

KDB_API K K_DECL exepath(K _) {
	std::vector<wchar_t> buffer(MAX_PATH, L'\0');
	assert(MAX_PATH <= std::numeric_limits<DWORD>::max());
	DWORD result = ::GetModuleFileNameW(NULL, &buffer[0], static_cast<DWORD>(buffer.size()));
	return getStringResult(result, buffer);
}