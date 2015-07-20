#ifndef __TDF_API_HANDLER_H__
#define __TDF_API_HANDLER_H__

#include "win32.util/Singleton.h"

namespace TDF {

	class TickHandler {
	public:
		static TickHandler* getInstance();

		static void DataMsgCallback(::THANDLE tdf, ::TDF_MSG* msg);
		static void SystemMsgCallback(::THANDLE tdf, ::TDF_MSG* msg);

		template <typename Data>
		static void processMsg(::THANDLE const tdf, ::TDF_MSG const& msg,
			void(TickHandler::*processor)(::THANDLE const, int, Data const&));

		static void processMsg(::THANDLE const tdf, ::TDF_MSG const& msg,
			void(TickHandler::*processor)(::THANDLE const, int));

		void onDataIndex(::THANDLE const tdf, int connectId, ::TDF_INDEX_DATA const& data);
		void onDataMarket(::THANDLE const tdf, int connectId, ::TDF_MARKET_DATA const& data);
		void onDataFuture(::THANDLE const tdf, int connectId, ::TDF_FUTURE_DATA const& data);
		void onDataTransaction(::THANDLE const tdf, int connectId, ::TDF_TRANSACTION const& data);
		void onDataOrderQueue(::THANDLE const tdf, int connectId, ::TDF_ORDER_QUEUE const& data);
		void onDataOrder(::THANDLE const tdf, int connectId, ::TDF_ORDER const& data);
		void onDataBbqTransaction(::THANDLE const tdf, int connectId, ::TDF_BBQTRANSACTION_DATA const& data);
		void onDataBbqBid(::THANDLE const tdf, int connectId, ::TDF_BBQBID_DATA const& data);

		void onSysDisconnect(::THANDLE const tdf, int connectId);
		void onSysConnect(::THANDLE const tdf, int connectId, ::TDF_CONNECT_RESULT const& data);
		void onSysLogin(::THANDLE const tdf, int connectId, ::TDF_LOGIN_RESULT const& data);
		void onSysCodeTable(::THANDLE const tdf, int connectId, ::TDF_CODE_RESULT const& data);
		void onSysQuoteDateChange(::THANDLE const tdf, int connectId, ::TDF_QUOTATIONDATE_CHANGE const& data);
		void onSysMarketClose(::THANDLE const tdf, int connectId, ::TDF_MARKET_CLOSE const& data);
		void onSysHeartbeat(::THANDLE const tdf, int connectId);

	private:
		TickHandler();
		~TickHandler();

		typedef Singleton<TickHandler> singleton_type;
		friend singleton_type;
		friend std::unique_ptr<TickHandler>::deleter_type;
	};

}//namespace TDF

#endif//__TDF_API_HANDLER_H__