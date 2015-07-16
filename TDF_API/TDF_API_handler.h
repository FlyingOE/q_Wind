#ifndef __TDF_API_HANDLER_H__
#define __TDF_API_HANDLER_H__

namespace TDF {

	void ResetMsgHandlers();

	void onDataMsg(::THANDLE tdf, ::TDF_MSG* msg);

	void onSystemMsg(::THANDLE tdf, ::TDF_MSG* msg);

	void onDataIndex(::THANDLE const tdf, ::TDF_MSG const& msg);
	void onDataMarket(::THANDLE const tdf, ::TDF_MSG const& msg);
	void onDataFuture(::THANDLE const tdf, ::TDF_MSG const& msg);
	void onDataTransaction(::THANDLE const tdf, ::TDF_MSG const& msg);
	void onDataOrderQueue(::THANDLE const tdf, ::TDF_MSG const& msg);
	void onDataOrder(::THANDLE const tdf, ::TDF_MSG const& msg);
	void onDataBbqTransaction(::THANDLE const tdf, ::TDF_MSG const& msg);
	void onDataBbqBid(::THANDLE const tdf, ::TDF_MSG const& msg);

	void onSysDisconnect(::THANDLE const tdf, ::TDF_MSG const& msg);
	void onSysConnect(::THANDLE const tdf, ::TDF_MSG const& msg);
	void onSysLogin(::THANDLE const tdf, ::TDF_MSG const& msg);
	void onSysCodeTable(::THANDLE const tdf, ::TDF_MSG const& msg);
	void onSysQuoteDateChange(::THANDLE const tdf, ::TDF_MSG const& msg);
	void onSysMarketClose(::THANDLE const tdf, ::TDF_MSG const& msg);
	void onSysHeartbeat(::THANDLE const tdf, ::TDF_MSG const& msg);

}//namespace TDF

#endif//__TDF_API_HANDLER_H__