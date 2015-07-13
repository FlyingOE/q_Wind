#include "stdafx.h"
#include "util.h"

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

void TDF::DataTypeFlag::registerAll() {
	registerEnumString(DATA_TYPE_TRANSACTION, "Transaction");
	registerEnumString(DATA_TYPE_ORDER, "Order");
	registerEnumString(DATA_TYPE_ORDERQUEUE, "OrderQueue");
}