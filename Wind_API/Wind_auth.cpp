#include "stdafx.h"
#include "Wind_API.h"

#include "util.h"

#include "kdb+.util/util.h"
#include "kdb+.util/multilang.h"
#include "kdb+.util/type_convert.h"
#include <iostream>


WIND_API K K_DECL Wind_login(K username, K password) {
	std::wstring uid, pwd;
	try {
		uid = q::q2WString(username);
		pwd = q::q2WString(password);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	::WQAUTH_INFO login = { true };
	std::wmemset(login.strUserName, L'\0', _countof(login.strUserName));
	std::wmemset(login.strPassword, L'\0', _countof(login.strPassword));

	static_assert(std::is_same<std::wstring::value_type, TCHAR>::value, "UNICODE/_UNICODE not defined");
	if (uid.size() >= _countof(login.strUserName)) return q::error2q("username too long");
	if (pwd.size() >= _countof(login.strPassword)) return q::error2q("password too long");
#	ifdef _MSC_VER
	::wcsncpy_s(login.strUserName, uid.c_str(), uid.size());
	::wcsncpy_s(login.strPassword, pwd.c_str(), pwd.size());
#	else
	std::wcsncpy(login.strUserName, uid.c_str(), _countof(login.strUserName));
	std::wcsncpy(login.strPassword, pwd.c_str(), _countof(login.strPassword));
#	endif

#	ifndef NDEBUG
	std::wcerr << L">>> WDataAuthorize({\""
		<< login.strUserName << L"\", \"" << login.strPassword
		<< L"\"})" << std::endl;
#	endif
	::WQErr const error = ::WDataAuthorize(&login);
#	ifndef NDEBUG
	std::wcerr << L"<<< WDataAuthorize = " << error << std::endl;
#	endif
	if (error == WQERR_OK) {
		std::string const u = ml::convert(q::DEFAULT_CP, uid.c_str());
		std::cerr << "<Wind> logged in as " << u << std::endl;
		return ks(const_cast<S>(u.c_str()));
	}
	else {
		return q::error2q(Wind::util::error2Text(error));
	}
}

WIND_API K K_DECL Wind_logout(K _) {
	::WQErr const error = ::WDataAuthQuit();
	if (error == WQERR_OK) {
		std::cerr << "<Wind> logged out" << std::endl;
		return K_NIL;
	}
	else {
		return q::error2q(Wind::util::error2Text(error));
	}
}