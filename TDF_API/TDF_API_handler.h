#ifndef __TDF_API_HANDLER_H__
#define __TDF_API_HANDLER_H__

namespace TDF {

	void ResetMsgHandlers();

	void OnDataMsg(::THANDLE tdf, ::TDF_MSG* msg);

	void OnSystemMsg(::THANDLE tdf, ::TDF_MSG* msg);

}//namespace TDF

#endif//__TDF_API_HANDLER_H__