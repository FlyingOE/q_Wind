#include "stdafx.h"
#include "Wind_API.h"

#include "util.h"
#include "WindEvent.h"

#include "kdb+.util/util.h"
#include "kdb+.util/type_convert.h"
#include <memory>	//C++11: std::shared_ptr<>
#include <future>	//C++11
#include <sstream>

namespace Wind {
	namespace callback {

		// A manager to take care of memory management for Wind's asynchronous callbacks
		class Result {
		public:
			typedef std::shared_ptr<std::promise<::WQEvent*> > pointer;

			Result() : result_(new pointer(new pointer::element_type)) {}

			pointer* dup() const {
				return new pointer(*result_);
			}
			
			K waitFor(::WQID qid, std::chrono::milliseconds const& timeout = Wind::ASYNC_TIMEOUT) {
				if (qid == 0) {
					return q::error2q("unknown request ID");
				}
				else if (qid < 0) {
					std::ostringstream buffer;
					buffer << "<WQ> strike result error: " << util::error2Text(static_cast<::WQErr>(qid));
					return q::error2q(buffer.str());
				}

				assert(result_.get() != NULL);
				std::future<::WQEvent*> outcome = (*result_)->get_future();
				switch (outcome.wait_for(timeout)) {
				case std::future_status::ready:
					try {
						std::auto_ptr<Event> event(static_cast<Event*>(outcome.get()));
						assert(event.get() != NULL);
						return event->parse();
					}
					catch (std::string const& error) {
						return q::error2q(error);
					}
				case std::future_status::timeout:
					return q::error2q("request timed out");
				default:
					return q::error2q("TODO request differed or ...?!");
				}
			}

		private:
			std::auto_ptr<pointer> result_;
		};

		int WINAPI strike(::WQEvent* pEvent, LPVOID lpUserParam) {
			std::auto_ptr<Result::pointer> result(static_cast<Result::pointer*>(lpUserParam));
			assert(result.get() != NULL);

			assert(pEvent != NULL);
			switch (pEvent->EventType) {
			case eWQResponse:
			case eWQErrorReport:
				(*result)->set_value(new Wind::Event(*pEvent));
				return true;
			default: {
					std::ostringstream buffer;
					buffer << "<WQ> unsupported strike response: " << *pEvent;
					(*result)->set_exception(std::make_exception_ptr(buffer.str()));
					return false;
				}
			}
		}

	}//namespace Wind::callback
}//namespace Wind

WIND_API K K_DECL Wind_wsd(K windCode, K indicators, K beginDate, K endDate, K params) {
	std::wstring code, indis, begin, end, paras;
	try {
		code = q::q2WString(windCode);
		indis = Wind::util::qList2WStringJoin(indicators, L',');
		begin = Wind::util::q2DateStr(beginDate);
		end   = Wind::util::q2DateStr(endDate);
		paras = Wind::util::qDict2WStringMapJoin(params, L';', L'=');
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	Wind::callback::Result result;
	::WQID const qid = ::WSD(code.c_str(), indis.c_str(), begin.c_str(), end.c_str(), paras.c_str(),
		&Wind::callback::strike, result.dup());
	return result.waitFor(qid);
}

WIND_API K K_DECL Wind_wss(K windCodes, K indicators, K params) {
	std::wstring codes, indis, paras;
	try {
		codes = Wind::util::qList2WStringJoin(windCodes, L',');
		indis = Wind::util::qList2WStringJoin(indicators, L',');
		paras = Wind::util::qDict2WStringMapJoin(params, L';', L'=');
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	Wind::callback::Result result;
	::WQID const qid = ::WSS(codes.c_str(), indis.c_str(), paras.c_str(),
		&Wind::callback::strike, result.dup());
	return result.waitFor(qid);
}

WIND_API K K_DECL Wind_wsi(K windCode, K indicators, K beginTime, K endTime, K params) {
	std::wstring code, indis, begin, end, paras;
	try {
		code = q::q2WString(windCode);
		indis = Wind::util::qList2WStringJoin(indicators, L',');
		begin = Wind::util::q2DateTimeStr(beginTime);
		end = Wind::util::q2DateTimeStr(endTime);
		paras = Wind::util::qDict2WStringMapJoin(params, L';', L'=');
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	Wind::callback::Result result;
	::WQID const qid = ::WSI(code.c_str(), indis.c_str(), begin.c_str(), end.c_str(), paras.c_str(),
		&Wind::callback::strike, result.dup());
	return result.waitFor(qid);
}

WIND_API K K_DECL Wind_wst(K windCode, K indicators, K beginTime, K endTime, K params) {
	std::wstring code, indis, begin, end, paras;
	try {
		code = q::q2WString(windCode);
		indis = Wind::util::qList2WStringJoin(indicators, L',');
		begin = Wind::util::q2DateTimeStr(beginTime);
		end = Wind::util::q2DateTimeStr(endTime);
		paras = Wind::util::qDict2WStringMapJoin(params, L';', L'=');
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	Wind::callback::Result result;
	::WQID const qid = ::WST(code.c_str(), indis.c_str(), begin.c_str(), end.c_str(), paras.c_str(),
		&Wind::callback::strike, result.dup());
	return result.waitFor(qid);
}

WIND_API K K_DECL Wind_wset(K reportName, K params) {
	std::wstring report, paras;
	try {
		report = q::q2WString(reportName);
		paras = Wind::util::qDict2WStringMapJoin(params, L';', L'=');
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	Wind::callback::Result result;
	::WQID const qid = ::WSET(report.c_str(), paras.c_str(),
		&Wind::callback::strike, result.dup());
	return result.waitFor(qid);
}

WIND_API K K_DECL Wind_tdays(K beginDate, K endDate, K params) {
	std::wstring begin, end, paras;
	try {
		begin = Wind::util::q2DateStr(beginDate);
		end   = Wind::util::q2DateStr(endDate);
		paras = Wind::util::qDict2WStringMapJoin(params, L';', L'=');
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	Wind::callback::Result result;
	::WQID const qid = ::TDays(begin.c_str(), end.c_str(), paras.c_str(),
		&Wind::callback::strike, result.dup());
	return result.waitFor(qid);
}

WIND_API K K_DECL Wind_tdaysoff(K beginDate, K offset, K params) {
	std::wstring begin, offs, paras;
	try {
		begin = Wind::util::q2DateStr(beginDate);
		std::wostringstream buffer;
		buffer << q::q2Dec(offset);
		offs = buffer.str();
		paras = Wind::util::qDict2WStringMapJoin(params, L';', L'=');
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	Wind::callback::Result result;
	::WQID const qid = ::TDaysOffset(begin.c_str(), offs.c_str(), paras.c_str(),
		&Wind::callback::strike, result.dup());
	return result.waitFor(qid);
}

WIND_API K K_DECL Wind_tdayscnt(K beginDate, K endDate, K params) {
	std::wstring begin, end, paras;
	try {
		begin = Wind::util::q2DateStr(beginDate);
		end   = Wind::util::q2DateStr(endDate);
		paras = Wind::util::qDict2WStringMapJoin(params, L';', L'=');
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	Wind::callback::Result result;
	::WQID const qid = ::TDaysCount(begin.c_str(), end.c_str(), paras.c_str(),
		&Wind::callback::strike, result.dup());
	return result.waitFor(qid);
}

WIND_API K K_DECL Wind_wupf(K portfolioName, K tradeDates, K windCodes, K quantities, K costPrices, K params) {
	std::wstring portf, dates, codes, qtys, costs, paras;
	try {
		portf = q::q2WString(portfolioName);
		dates = Wind::util::qList2DateStrJoin(tradeDates, L',');
		codes = Wind::util::qList2WStringJoin(windCodes, L',');
		qtys = Wind::util::qList2DecStrJoin(quantities, L',');
		costs = Wind::util::qList2FpStrJoin(costPrices, L',');
		paras = Wind::util::qDict2WStringMapJoin(params, L';', L'=');
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	Wind::callback::Result result;
	::WQID const qid = ::WUPF(portf.c_str(), dates.c_str(), codes.c_str(), qtys.c_str(), costs.c_str(), paras.c_str(),
		&Wind::callback::strike, result.dup());
	return result.waitFor(qid);
}