#include "stdafx.h"
#include "util.h"

#include "kdb+.util/type_convert.h"

std::string TDF::getError(::TDF_ERR errorCode) {
	switch (errorCode) {
	case TDF_ERR_SUCCESS:				// 成功
		return "OK";
	case TDF_ERR_UNKOWN:				// 未知错误
		return "unknown error";
	case TDF_ERR_INITIALIZE_FAILURE:	// 初始化socket环境失败
		return "initialization failure";
	case TDF_ERR_NETWORK_ERROR:			// 网络连接出现问题
		return "network error";
	case TDF_ERR_INVALID_PARAMS:		// 输入参数无效
		return "invalid params";
	case TDF_ERR_VERIFY_FAILURE:		// 登陆验证失败：原因为用户名或者密码错误；超出登陆数量
		return "login failure";
	case TDF_ERR_NO_AUTHORIZED_MARKET:	// 所有请求的市场都没有授权
		return "market not authorized";
	case TDF_ERR_NO_CODE_TABLE:			// 所有请求的市场该天都没有代码表
		return "no code table";
	default:
		;
	};
	std::ostringstream buffer;
	buffer << "unknown errorCode=" << errorCode;
	return buffer.str();
}

unsigned int TDF::util::q2time(K time) throw(std::string) {
	if (time == K_NIL) throw std::string("nil time");

	char hh, mm, ss;
	short millis;
	switch (time->t) {
	case -KT:
		q::Cookbook::tsms(time->i, &hh, &mm, &ss, &millis);
		return hh * 10000 + mm * 100 + ss;
	default:
		throw std::string("not a time");
	}
}

std::string TDF::util::formatDate(int yyyymmdd) {
	I const d = q::date2q(yyyymmdd);
	q::tm_ext tm;
	std::memset(&tm, 0, sizeof(tm));
	q::Cookbook::lt_r(d, &tm);
	std::vector<char> buffer((4 + 1 + 2 + 1 + 2) + 1, '\0');
	q::Cookbook::fdt(&tm, &buffer[0]);
	return &buffer[0];
}

std::string TDF::util::formatTime(int hhmmssfff) {
	I const ts = q::time2q(hhmmssfff);
	std::vector<char> buffer((2 + 1 + 2 + 1 + 2 + 1 + 3) + 1, '\0');
	q::Cookbook::ftsms(ts, &buffer[0]);
	assert(*buffer.rbegin() == '\0');
	return &buffer[0];
}

void TDF::DataTypeFlag::registerAll() {
	registerEnumString(DATA_TYPE_TRANSACTION, "Transaction");
	registerEnumString(DATA_TYPE_ORDER, "Order");
	registerEnumString(DATA_TYPE_ORDERQUEUE, "OrderQueue");
}