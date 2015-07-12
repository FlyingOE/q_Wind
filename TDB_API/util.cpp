#include "stdafx.h"
#include "util.h"

#include "kdb+.util/multilang.h"

#include <sstream>

I TDB::util::time2q(int hhmmssfff) {
	int const hh = hhmmssfff / 10000000;
	int const mm = (hhmmssfff % 10000000) / 100000;
	int const ssfff = hhmmssfff % 100000;
	return (hh * 60 + mm) * 60000 + ssfff;
}

void TDB::util::tm2DateTime(q::tm_ext const& tm, int &date, int &time) {
	date = (1900 + tm.tm_year) * 10000 + (1 + tm.tm_mon) * 100 + tm.tm_mday;
	time = tm.tm_hour * 10000000 + tm.tm_min * 100000 + tm.tm_sec * 1000 + tm.tm_millis;
}

std::string TDB::util::GB18030Encoder::operator()(char const* str) const {
	return ml::convert(ml::CP_GB18030, CP_UTF8, str);
}

std::string TDB::getError(::TDB_ERROR errorCode) {
	switch (errorCode) {
	case TDB_SUCCESS:
		return "OK";
	case TDB_NETWORK_ERROR:		//网络错误
		return "network error";
	case TDB_NETWORK_TIMEOUT:	//网络超时
		return "network timeout";
	case TDB_NO_DATA:			//没有数据
		return "no data";
	case TDB_OUT_OF_MEMORY:		//内存耗尽
		return "out of memory";
	case TDB_LOGIN_FAILED:		//登陆失败
		return "login failed";
	case TDB_INVALID_PARAMS:	//无效的参数(hTdb无效，pReq为空，pCount为空)
		return "invalid params";
	case TDB_INVALID_CODE_TYPE:	//无效的代码类型，比如向TDB_GetFuture传入非期货类型代码，返回之。
		return "invalid code type";
	case TDB_WRONG_FORMULA:		//指标公式错误
		return "invalid_formula";
	default:
		;
	}
	std::ostringstream buffer;
	buffer << "unknown errorCode=" << errorCode;
	return buffer.str();
}