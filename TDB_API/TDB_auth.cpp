#include "stdafx.h"
#include "TDB_API.h"

#include "win32.util/hexDump.h"
#include "kdb+.util/util.h"
#include "kdb+.util/type_convert.h"
#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>

TDB_API K K_DECL TDB_login(K level, K host, K port, K username, K password) {
	std::string h, uid, pwd;
	long long L, p;
	try {
		L = q::q2Dec(level);
		h = q::q2String(host);
		p = q::q2Dec(port);
		uid = q::q2String(username);
		pwd = q::q2String(password);
	}
	catch (std::runtime_error const& error) {
		return q::error2q(error.what());
	}

	if ((L < 1) || (2 < L)) {
		return q::error2q(std::string("invalid data level"));
	}

	::OPEN_SETTINGS settings = ::TDB::SETTINGS;
	std::memset(settings.szIP, '\0', _countof(settings.szIP));
	std::memset(settings.szPort, '\0', _countof(settings.szPort));
	std::memset(settings.szUser, '\0', _countof(settings.szUser));
	std::memset(settings.szPassword, '\0', _countof(settings.szPassword));

	if (h.size() >= _countof(settings.szIP)) return q::error2q("host/IP too long");
	if (p >= std::pow(10, _countof(settings.szPort))) return q::error2q("port number too long");
	if (uid.size() >= _countof(settings.szUser)) return q::error2q("username too long");
	if (pwd.size() >= _countof(settings.szPassword)) return q::error2q("password too long");
#	ifdef _MSC_VER
	::strncpy_s(settings.szIP,       h.c_str(),   h.size());
	::strncpy_s(settings.szUser,     uid.c_str(), uid.size());
	::strncpy_s(settings.szPassword, pwd.c_str(), pwd.size());
#	else
	std::strncpy(settings.szIP,       h.c_str(),   _countof(settings.szIP));
	std::strncpy(settings.szUser,     uid.c_str(), _countof(settings.szUser));
	std::strncpy(settings.szPassword, pwd.c_str(), _countof(settings.szPassword));
#	endif
	std::snprintf(settings.szPort, _countof(settings.szPort), "%ld", p);

#	ifndef NDEBUG
	std::cerr << ">>> TDB_Open({\""
		<< settings.szIP << "\", \"" << settings.szPort << "\", \""
		<< settings.szUser << "\", \"" << settings.szPassword << "\"})"
		<< std::endl;
#	endif
	::TDBDefine_ResLogin result = { 0 };
	::THANDLE tdb = ::TDB_Open(&settings, &result);
	if (tdb) {
		std::cerr << "<TDB> logged in as " << uid << " (0x" << util::hexBytes(tdb) << ')' << std::endl;
		auto const result = TDB::LEVELS.insert(std::make_pair(tdb, static_cast<char>(L)));
		assert(result.second && "unexpected duplicated THANDLE");
		static_assert(sizeof(J) >= sizeof(::THANDLE), "J is smaller than THANDLE");
		return kj(reinterpret_cast<J>(tdb));
	}
	else {
		return q::error2q(result.szInfo);
	}
}

TDB_API K K_DECL TDB_logout(K h) {
	::THANDLE tdb = NULL;
	try {
		long long const hh = q::q2Dec(h);
		tdb = reinterpret_cast<::THANDLE>(hh);
	}
	catch (std::runtime_error const& error) {
		return q::error2q(error.what());
	}

	if (tdb) {
		int const error = ::TDB_Close(tdb);
		std::cerr << "<TDB> logged out 0x" << util::hexBytes(tdb) << std::endl;
		size_t const result = TDB::LEVELS.erase(tdb);
		assert((result == 1) && "unregistered THANDLE");
		return ki(error);
	}
	else {
		return q::error2q("null THANDLE");
	}
}