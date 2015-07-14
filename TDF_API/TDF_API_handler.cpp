#include "stdafx.h"
#include "TDF_API_handler.h"

#include "win32.util/util.h"

#include <cassert>
#include <iostream>

void TDF::ResetMsgHandlers() {
}

void TDF::OnDataMsg(::THANDLE tdf, ::TDF_MSG* msg) {
	assert(tdf != NULL);
	assert(msg != NULL);
	assert(msg->pData != NULL);
	std::size_t const count = msg->pAppHead->nItemCount;
	std::size_t const size = msg->pAppHead->nItemSize;
	switch (msg->nDataType) {
	case MSG_DATA_INDEX:			//指数数据
		assert(size == sizeof(::TDF_INDEX_DATA));
		std::cout << "<TDF> " << util::hexBytes(tdf) << " received TDF_INDEX_DATA x " << count << std::endl;
		break;
	case MSG_DATA_MARKET:			//行情数据
		assert(size == sizeof(::TDF_MARKET_DATA));
		std::cout << "<TDF> " << util::hexBytes(tdf) << " received TDF_MARKET_DATA x " << count << std::endl;
		break;
	case MSG_DATA_FUTURE:			//期货行情
		assert(size == sizeof(::TDF_FUTURE_DATA));
		std::cout << "<TDF> " << util::hexBytes(tdf) << " received TDF_FUTURE_DATA x " << count << std::endl;
		break;
	case MSG_DATA_TRANSACTION:		//逐笔成交
		assert(size == sizeof(::TDF_TRANSACTION));
		std::cout << "<TDF> " << util::hexBytes(tdf) << " received TDF_TRANSACTION x " << count << std::endl;
		break;
	case MSG_DATA_ORDERQUEUE:		//委托队列
		assert(size == sizeof(::TDF_ORDER_QUEUE));
		std::cout << "<TDF> " << util::hexBytes(tdf) << " received TDF_ORDER_QUEUE x " << count << std::endl;
		break;
	case MSG_DATA_ORDER:			//逐笔委托
		assert(size == sizeof(::TDF_ORDER));
		std::cout << "<TDF> " << util::hexBytes(tdf) << " received TDF_ORDER x " << count << std::endl;
		break;
	case MSG_DATA_BBQTRANSACTION:	//BBQ现券成交数据
		assert(size == sizeof(::TDF_BBQTRANSACTION_DATA));
		std::cout << "<TDF> " << util::hexBytes(tdf) << " received TDF_BBQTRANSACTION_DATA x " << count << std::endl;
		break;
	case MSG_DATA_BBQBID:			//BBQ现券报价数据
		assert(size == sizeof(::TDF_BBQBID_DATA));
		std::cout << "<TDF> " << util::hexBytes(tdf) << " received TDF_BBQBID_DATA x " << count << std::endl;
		break;
	default:
		std::cerr << "<TDF> " << util::hexBytes(tdf)
			<< "received unknown data message 0x" << util::hexBytes(msg) << ", "
			<< "type=" << msg->nDataType << " (" << util::hexBytes(msg->pData) << ')'
			<< std::endl;
	}
}

void TDF::OnSystemMsg(::THANDLE tdf, ::TDF_MSG* msg) {
	if (tdf == NULL) return;
	if (msg == NULL) return;
	std::size_t const count = msg->pAppHead->nItemCount;
	std::size_t const size = msg->pAppHead->nItemSize;
	switch (msg->nDataType) {
	case MSG_SYS_DISCONNECT_NETWORK:	//网络断开事件, 相应的结构体为NULL
		std::cerr << "<TDF> " << util::hexBytes(tdf) << " network disconnected!" << std::endl;
		assert(msg->pData == NULL);
		break;
	case MSG_SYS_CONNECT_RESULT:		//主动发起连接的结果
		assert(msg->pData != NULL);
		assert(size == sizeof(::TDF_CONNECT_RESULT));
		std::cout << "<TDF> " << util::hexBytes(tdf) << " received TDF_CONNECT_RESULT x " << count << std::endl;
		break;
	case MSG_SYS_LOGIN_RESULT:			//登陆应答
		assert(msg->pData != NULL);
		assert(size == sizeof(::TDF_LOGIN_RESULT));
		std::cout << "<TDF> " << util::hexBytes(tdf) << " received TDF_LOGIN_RESULT x " << count << std::endl;
		break;
	case MSG_SYS_CODETABLE_RESULT:		//获取代码表结果
		assert(msg->pData != NULL);
		assert(size == sizeof(::TDF_CODE_RESULT));
		std::cout << "<TDF> " << util::hexBytes(tdf) << " received TDF_CODE_RESULT x " << count << std::endl;
		break;
	case MSG_SYS_QUOTATIONDATE_CHANGE:	//行情日期变更通知
		assert(msg->pData != NULL);
		assert(size == sizeof(::TDF_QUOTATIONDATE_CHANGE));
		std::cout << "<TDF> " << util::hexBytes(tdf) << " received TDF_QUOTATIONDATE_CHANGE x " << count << std::endl;
		break;
	case MSG_SYS_MARKET_CLOSE:			//闭市
		assert(msg->pData != NULL);
		assert(size == sizeof(::TDF_MARKET_CLOSE));
		std::cout << "<TDF> " << util::hexBytes(tdf) << " received TDF_MARKET_CLOSE x " << count << std::endl;
		break;
	case MSG_SYS_HEART_BEAT:			//服务器心跳消息, 相应的结构体为NULL
		std::cout << "<TDF> " << util::hexBytes(tdf) << " heartbeat." << std::endl;
		//assert(msg->pData == NULL);	//Supposed to be NULL, but...
		break;
	default:
		std::cerr << "<TDF> " << util::hexBytes(tdf)
			<< "received unknown system message 0x" << util::hexBytes(msg) << ", "
			<< "type=" << msg->nDataType << " (" << util::hexBytes(msg->pData) << ')'
			<< std::endl;
	}
}