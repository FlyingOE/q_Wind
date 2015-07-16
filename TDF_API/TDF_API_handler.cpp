#include "stdafx.h"
#include "TDF_API_handler.h"

#include "util.h"

#include "win32.util/util.h"

#include <iostream>

namespace TDF {
	namespace util {
		template <typename Char, typename Traits>
		std::basic_ostream<Char, Traits>& logHeader(std::basic_ostream<Char, Traits>& os,
			::THANDLE const tdf, ::TDF_MSG const& msg)
		{
			return os << "<TDF> #" << msg.nConnectId << " 0x" << ::util::hexBytes(tdf);
		}
	}//namespace TDF::util
}//namespace TDF

void TDF::ResetMsgHandlers() {
}

void TDF::onDataMsg(::THANDLE tdf, ::TDF_MSG* msg) {
	assert(tdf != NULL);
	assert(msg != NULL);
	assert(msg->pData != NULL);
	std::size_t const count = msg->pAppHead->nItemCount;
	std::size_t const size = msg->pAppHead->nItemSize;
	switch (msg->nDataType) {
	case MSG_DATA_INDEX:
		onDataIndex(tdf, *msg);
		break;
	case MSG_DATA_MARKET:
		onDataMarket(tdf, *msg);
		break;
	case MSG_DATA_FUTURE:
		onDataFuture(tdf, *msg);
		break;
	case MSG_DATA_TRANSACTION:
		onDataTransaction(tdf, *msg);
		break;
	case MSG_DATA_ORDERQUEUE:
		onDataOrderQueue(tdf, *msg);
		break;
	case MSG_DATA_ORDER:
		onDataOrder(tdf, *msg);
		break;
	case MSG_DATA_BBQTRANSACTION:
		onDataBbqTransaction(tdf, *msg);
		break;
	case MSG_DATA_BBQBID:
		onDataBbqBid(tdf, *msg);
		break;
	default:
		util::logHeader(std::cerr, tdf, *msg) << "received unknown data message 0x" << ::util::hexBytes(msg) << ", "
			<< "type=" << msg->nDataType << " (0x" << ::util::hexBytes(msg->pData) << " +" << msg->nDataLen << ')'
			<< std::endl;
	}
}

void TDF::onSystemMsg(::THANDLE tdf, ::TDF_MSG* msg) {
	if (tdf == NULL) return;
	if (msg == NULL) return;
	std::size_t const count = msg->pAppHead->nItemCount;
	std::size_t const size = msg->pAppHead->nItemSize;
	switch (msg->nDataType) {
	case MSG_SYS_DISCONNECT_NETWORK:
		onSysDisconnect(tdf, *msg);
		break;
	case MSG_SYS_CONNECT_RESULT:
		onSysConnect(tdf, *msg);
		break;
	case MSG_SYS_LOGIN_RESULT:
		onSysLogin(tdf, *msg);
		break;
	case MSG_SYS_CODETABLE_RESULT:
		onSysCodeTable(tdf, *msg);
		break;
	case MSG_SYS_QUOTATIONDATE_CHANGE:
		onSysQuoteDateChange(tdf, *msg);
		break;
	case MSG_SYS_MARKET_CLOSE:
		onSysMarketClose(tdf, *msg);
		break;
	case MSG_SYS_HEART_BEAT:
		onSysHeartbeat(tdf, *msg);
		break;
	default:
		util::logHeader(std::cerr, tdf, *msg) << " received unknown system message 0x" << ::util::hexBytes(msg) << ", "
			<< "type=" << msg->nDataType << " (0x" << ::util::hexBytes(msg->pData) << " +" << msg->nDataLen << ')'
			<< std::endl;
	}
}

// 指数数据
void TDF::onDataIndex(::THANDLE const tdf, ::TDF_MSG const& msg) {
	assert(msg.pAppHead->nItemSize == sizeof(::TDF_INDEX_DATA));
	auto result = static_cast<::TDF_INDEX_DATA const*>(msg.pData);
	assert(result != NULL);
	util::logHeader(std::cout, tdf, msg) << " received TDF_INDEX_DATA 0x" << ::util::hexBytes(result) << std::endl;
}

// 行情数据
void TDF::onDataMarket(::THANDLE const tdf, ::TDF_MSG const& msg) {
	assert(msg.pAppHead->nItemSize == sizeof(::TDF_MARKET_DATA));
	auto result = static_cast<::TDF_MARKET_DATA const*>(msg.pData);
	assert(result != NULL);
	util::logHeader(std::cout, tdf, msg) << " received TDF_MARKET_DATA 0x" << ::util::hexBytes(result) << std::endl;
}

// 期货行情
void TDF::onDataFuture(::THANDLE const tdf, ::TDF_MSG const& msg) {
	assert(msg.pAppHead->nItemSize == sizeof(::TDF_FUTURE_DATA));
	auto result = static_cast<::TDF_FUTURE_DATA const*>(msg.pData);
	assert(result != NULL);
	util::logHeader(std::cout, tdf, msg) << " received TDF_FUTURE_DATA 0x" << ::util::hexBytes(result) << std::endl;
}

// 逐笔成交
void TDF::onDataTransaction(::THANDLE const tdf, ::TDF_MSG const& msg) {
	assert(msg.pAppHead->nItemSize == sizeof(::TDF_TRANSACTION));
	auto result = static_cast<::TDF_TRANSACTION const*>(msg.pData);
	assert(result != NULL);
	util::logHeader(std::cout, tdf, msg) << " received TDF_TRANSACTION 0x" << ::util::hexBytes(result) << std::endl;
}

// 委托队列
void TDF::onDataOrderQueue(::THANDLE const tdf, ::TDF_MSG const& msg) {
	assert(msg.pAppHead->nItemSize == sizeof(::TDF_ORDER_QUEUE));
	auto result = static_cast<::TDF_ORDER_QUEUE const*>(msg.pData);
	assert(result != NULL);
	util::logHeader(std::cout, tdf, msg) << " received TDF_ORDER_QUEUE 0x" << ::util::hexBytes(result) << std::endl;
}

// 逐笔委托
void TDF::onDataOrder(::THANDLE const tdf, ::TDF_MSG const& msg) {
	assert(msg.pAppHead->nItemSize == sizeof(::TDF_ORDER));
	auto result = static_cast<::TDF_ORDER const*>(msg.pData);
	assert(result != NULL);
	util::logHeader(std::cout, tdf, msg) << " received TDF_ORDER 0x" << ::util::hexBytes(result) << std::endl;
}

// BBQ现券成交数据
void TDF::onDataBbqTransaction(::THANDLE const tdf, ::TDF_MSG const& msg) {
	assert(msg.pAppHead->nItemSize == sizeof(::TDF_BBQTRANSACTION_DATA));
	auto result = static_cast<::TDF_BBQTRANSACTION_DATA const*>(msg.pData);
	assert(result != NULL);
	util::logHeader(std::cout, tdf, msg) << " received TDF_BBQTRANSACTION_DATA 0x" << ::util::hexBytes(result) << std::endl;
}

// BBQ现券报价数据
void TDF::onDataBbqBid(::THANDLE const tdf, ::TDF_MSG const& msg) {
	assert(msg.pAppHead->nItemSize == sizeof(::TDF_BBQBID_DATA));
	auto result = static_cast<::TDF_BBQBID_DATA const*>(msg.pData);
	assert(result != NULL);
	util::logHeader(std::cout, tdf, msg) << " received TDF_BBQBID_DATA 0x" << ::util::hexBytes(result) << std::endl;
}

// 网络断开事件, 相应的结构体为NULL
void TDF::onSysDisconnect(::THANDLE const tdf, ::TDF_MSG const& msg) {
	//assert(msg->pData == NULL);
	util::logHeader(std::cerr, tdf, msg) << " network disconnected!" << std::endl;
}

// 主动发起连接的结果
void TDF::onSysConnect(::THANDLE const tdf, ::TDF_MSG const& msg) {
	assert(msg.pAppHead->nItemSize == sizeof(::TDF_CONNECT_RESULT));
	auto result = static_cast<::TDF_CONNECT_RESULT const*>(msg.pData);
	assert(result != NULL);
	if (result->nConnResult) {
		util::logHeader(std::cout, tdf, msg) << " connected to "
			<< result->szIp << ':' << result->szPort << " as " << result->szUser
			<< '.' << std::endl;
	}
	else {
		util::logHeader(std::cerr, tdf, msg) << " connection to "
			<< result->szIp << ':' << result->szPort << " as " << result->szUser
			<< " failed!" << std::endl;
	}
}

//登陆应答
void TDF::onSysLogin(::THANDLE const tdf, ::TDF_MSG const& msg) {
	assert(msg.pAppHead->nItemSize == sizeof(::TDF_LOGIN_RESULT));
	auto result = static_cast<::TDF_LOGIN_RESULT const*>(msg.pData);
	assert(result != NULL);
	if (result->nLoginResult) {
		util::logHeader(std::cout, tdf, msg) << " login successful: " << result->szInfo;
		assert((0 <= result->nMarkets) && (result->nMarkets <= _countof(result->szMarket)));
		for (int i = 0; i < result->nMarkets; ++i) {
			std::cout << "\n<TDF> >>\t" << result->szMarket[i] << "  " << util::formatDate(result->nDynDate[i]);
		}
		std::cout << std::endl;
	}
	else {
		util::logHeader(std::cerr, tdf, msg) << " login failed: " << result->szInfo << std::endl;
	}
}

// 获取代码表结果
void TDF::onSysCodeTable(::THANDLE const tdf, ::TDF_MSG const& msg) {
	assert(msg.pAppHead->nItemSize == sizeof(::TDF_CODE_RESULT));
	auto result = static_cast<::TDF_CODE_RESULT const*>(msg.pData);
	assert(result != NULL);
	util::logHeader(std::cout, tdf, msg) << " got code table: " << result->szInfo;
	assert((0 <= result->nMarkets) && (result->nMarkets <= _countof(result->szMarket)));
	for (int i = 0; i < result->nMarkets; ++i) {
		std::cout << "\n<TDF> >>\t" << result->szMarket[i] << "  "
			<< util::formatDate(result->nCodeDate[i]) << "  " << result->nCodeCount[i];
	}
	std::cout << std::endl;
}

// 行情日期变更通知
void TDF::onSysQuoteDateChange(::THANDLE const tdf, ::TDF_MSG const& msg) {
	assert(msg.pAppHead->nItemSize == sizeof(::TDF_QUOTATIONDATE_CHANGE));
	auto result = static_cast<::TDF_QUOTATIONDATE_CHANGE const*>(msg.pData);
	assert(result != NULL);
	util::logHeader(std::cout, tdf, msg) << " quotation date changed: " << result->szMarket << "  "
		<< util::formatDate(result->nOldDate) << " => " << util::formatDate(result->nNewDate) << std::endl;
}

// 闭市
void TDF::onSysMarketClose(::THANDLE const tdf, ::TDF_MSG const& msg) {
	assert(msg.pAppHead->nItemSize == sizeof(::TDF_MARKET_CLOSE));
	auto result = static_cast<::TDF_MARKET_CLOSE const*>(msg.pData);
	assert(result != NULL);
	util::logHeader(std::cout, tdf, msg) << " market closed: " << result->chInfo
		<< "\n<TDF> >>\t" << result->szMarket << " (" << util::formatTime(result->nTime)
		<< std::endl;
}

// 服务器心跳消息, 相应的结构体为NULL
void TDF::onSysHeartbeat(::THANDLE const tdf, ::TDF_MSG const& msg) {
	//assert(msg->pData == NULL);	//Supposed to be NULL, but...
	util::logHeader(std::cout, tdf, msg) << " heartbeat." << std::endl;
}