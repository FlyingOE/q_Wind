#include "stdafx.h"
#include "TDF_API.h"

#include "util.h"

#include "TDB_API/util.h"
#include "win32.util/util.h"
#include "kdb+.util/type_convert.h"

#include <cassert>
#include <string>
#include <vector>
#include <iostream>

namespace TDF {
	namespace util {

		template <typename T, typename Id>
		void setServerInfo(std::string const& data, T& field, char const* fieldName, Id const& fieldId)
			throw(std::string)
		{
			if (data.size() >= _countof(field)) {
				std::ostringstream buffer;
				buffer << fieldName << " [" << fieldId << "] too long";
				throw buffer.str();
			}
			else {
				std::memset(field, '\0', _countof(field));
#				ifdef _MSC_VER
				::strncpy_s(field, data.c_str(), data.size());
#				else
				std::strncpy(field, data.c_str(), _countof(field));
#				endif
			}
		}

		std::size_t setServerInfos(K const servers, ::TDF_SERVER_INFO out[MAXSERVER])
			throw(std::string)
		{
			if (servers == K_NIL) throw std::string("null servers");
			if (servers->t != XT) throw std::string("not a table");

			std::vector<std::string> const cols = q::qList2String(kK(servers->k)[0]);
			if (cols.size() != 4) throw std::string("invalid server info table");

			K const table = kK(servers->k)[1];
			assert(table->t == 0);
			assert(table->n == 4);

			assert(kK(table)[0]->t >= 0);
			assert(kK(table)[0]->n >= 0);
			std::size_t const count = static_cast<std::size_t>(kK(table)[0]->n);
			if (count > MAXSERVER) {
				throw std::string("too many servers");
			}

			std::vector<std::string> hosts, uids, pwds;
			std::vector<long long> ports;
			for (std::size_t c = 0; c < table->n; ++c) {
				if (cols[c] == "host") {
					hosts = q::qList2String(kK(table)[c]);
				}
				else if (cols[c] == "port") {
					ports = q::qList2Dec(kK(table)[c]);
				}
				else if (cols[c] == "username") {
					uids = q::qList2String(kK(table)[c]);
				}
				else if (cols[c] == "password") {
					pwds = q::qList2String(kK(table)[c]);
				}
				else {
					throw cols[c];
				}
			}

			for (std::size_t i = 0; i < count; ++i) {
				setServerInfo(hosts[i], out[i].szIp, "host/IP", i);
				setServerInfo(uids[i], out[i].szUser, "username", i);
				setServerInfo(pwds[i], out[i].szPwd, "password", i);
				if (ports[i] >= std::pow(10, _countof(out[i].szPort))) {
					std::ostringstream buffer;
					buffer << "port number [" << i << "] too long";
					throw buffer.str();
				}
				else {
					std::memset(out[i].szPort, '\0', _countof(out[i].szPort));
					std::snprintf(out[i].szPort, _countof(out[i].szPort), "%ld", ports[i]);
				}
			}
			return count;
		}

	}//namespace TDF::util
}//namespace TDF


TDF_API K K_DECL TDF_login(K servers, K markets, K windCodes, K msgTypes, K startTime) {
	::TDF_OPEN_SETTING_EXT settings = TDF::SETTINGS;
	std::vector<char> mkts, tiks;
	try {
		settings.nServerNum = TDF::util::setServerInfos(servers, settings.siServer);
		
		std::string const mktList = TDF::util::join(';', q::qList2String(markets));
		mkts.assign(mktList.begin(), mktList.end());
		mkts.push_back('\0');
		settings.szMarkets = &mkts[0];

		std::string const codeList = TDF::util::join(';', q::qList2String(windCodes));
		tiks.assign(codeList.begin(), codeList.end());
		tiks.push_back('\0');
		settings.szSubScriptions = &tiks[0];
		
		std::vector<std::string> const typeList = q::qList2String(msgTypes);
		settings.nTypeFlags = 0;
		for (auto t = typeList.cbegin(); t != typeList.cend(); ++t) {
			if (!t->empty()) {
				::DATA_TYPE_FLAG const flag = TDF::DataTypeFlag::fromString(*t);
				if (flag == 0) throw *t;
				settings.nTypeFlags |= flag;
			}
		}

		settings.nTime = TDF::util::q2time(startTime);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

#	ifndef NDEBUG
	std::cerr << ">>> TDF_OpenExt({[";
	for (std::size_t i = 0; i < settings.nServerNum; ++i) {
		std::cerr << "{\""
			<< settings.siServer[i].szIp << "\", \"" << settings.siServer[i].szPort << "\", \""
			<< settings.siServer[i].szUser << "\", \"" << settings.siServer[i].szPwd << "\"}";
		if (i + 1 < settings.nServerNum) std::cerr << ", ";
	}
	std::cerr << "], \"" << settings.szMarkets << "\", \"" << settings.szSubScriptions << "\", "
		<< settings.nTime << ", 0x" << util::hexBytes(settings.nTypeFlags) << "})" << std::endl;
#	endif
	::TDF_ERR result = TDF_ERR_UNKOWN;
	::THANDLE tdf = ::TDF_OpenExt(&settings, &result);
	if (result == TDF_ERR_SUCCESS) {
		std::cerr << "<TDF> logged in as [";
		for (std::size_t i = 0; i < settings.nServerNum; ++i) {
			std::cerr << settings.siServer[i].szUser;
			if (i + 1 < settings.nServerNum) std::wcerr << ';';
		}
		std::cerr << "] (0x" << util::hexBytes(tdf) << ')' << std::endl;
		static_assert(sizeof(J) >= sizeof(::THANDLE), "J is smaller than THANDLE");
		return kj(reinterpret_cast<J>(tdf));
	}
	else {
		return q::error2q(TDF::getError(result));
	}
}

TDF_API K K_DECL TDF_logout(K h) {
	::THANDLE tdf = NULL;
	try {
		long long const hh = q::q2Dec(h);
		tdf = reinterpret_cast<::THANDLE>(hh);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	if (tdf) {
		int const error = ::TDF_Close(tdf);
		std::cerr << "<TDF> logged out 0x" << util::hexBytes(tdf) << std::endl;
		return ki(error);
	}
	else {
		return q::error2q("null THANDLE");
	}
}