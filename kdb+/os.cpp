#include "stdafx.h"
#include "kdb+.h"

#include "kdb+.util/util.h"
#include "kdb+.util/types.h"
#include "kdb+.util/multilang.h"
#include <cassert>
#include <vector>

KDB_API K K_DECL cwd(K _) {
	std::vector<wchar_t> buffer(MAX_PATH, L'\0');
	DWORD result = ::GetCurrentDirectoryW(buffer.size(), &buffer[0]);
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