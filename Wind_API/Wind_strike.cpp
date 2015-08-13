#include "stdafx.h"
#include "Wind_strike.h"

#include "util.h"

#include "kdb+.util/util.h"
#include "kdb+.util/type_convert.h"
#include <sstream>

Wind::callback::Result::Result() : result_(new Wind::callback::Result::promise_ptr::element_type) {}

Wind::callback::Result::promise_ptr* Wind::callback::Result::dup() const {
	return new promise_ptr(result_);
}
			
K Wind::callback::Result::waitFor(::WQID qid, std::chrono::milliseconds const& timeout) {
	if (qid == 0) {
		return q::error2q("unknown request ID");
	}
	else if (qid < 0) {
		std::ostringstream buffer;
		buffer << "<WQ> strike result error: " << util::error2Text(static_cast<::WQErr>(qid));
		return q::error2q(buffer.str());
	}

	assert(result_);
	std::future<Event*> outcome = result_->get_future();
	switch (outcome.wait_for(timeout)) {
	case std::future_status::ready:
		try {
			std::unique_ptr<Event> event(outcome.get());
			assert(event);
			return event->parse();
		}
		catch (std::runtime_error& error) {	// Error from Wind::Callback::strike(...)
			return q::error2q(error.what());
		}
		catch (std::string const& error) {	// Error from Wind::Event::parse()
			return q::error2q(error);
		}
	case std::future_status::timeout:
		return q::error2q((::CancelRequest(qid) == WQERR_OK)
			? "request timed out, cancelled"
			: "request timed out");
	default:
		return q::error2q("TODO request differed or ...?!");
	}
}

int WINAPI Wind::callback::strike(::WQEvent* pEvent, LPVOID lpUserParam) {
	std::unique_ptr<Result::promise_ptr> pResult(static_cast<Result::promise_ptr*>(lpUserParam));
	assert(pResult);
	Result::promise_ptr& result(*pResult);
	assert(result);

	assert(pEvent != NULL);
	switch (pEvent->EventType) {
	case eWQResponse:
	case eWQErrorReport:
		result->set_value(new Event(*pEvent));
		return true;
	default: {
			std::ostringstream buffer;
			buffer << "<WQ> unsupported strike response: " << *pEvent;
			result->set_exception(std::make_exception_ptr(std::runtime_error(buffer.str())));
			return false;
		}
	}
}


WIND_API K K_DECL Wind_wsd(K windCodes, K indicators, K beginDate, K endDate, K params) {
	std::wstring codes, indis, begin, end, paras;
	try {
		codes = Wind::util::qList2WStringJoin(windCodes, L',');
		indis = Wind::util::qList2WStringJoin(indicators, L',');
		begin = Wind::util::q2DateStr(beginDate);
		end   = Wind::util::q2DateStr(endDate);
		paras = Wind::util::qDict2WStringMapJoin(params, L';', L'=');
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	Wind::callback::Result result;
	::WQID const qid = ::WSD(codes.c_str(), indis.c_str(), begin.c_str(), end.c_str(), paras.c_str(),
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

WIND_API K K_DECL Wind_edb(K windCode, K beginTime, K endTime, K params) {
	std::wstring code, begin, end, paras;
	try {
		code = q::q2WString(windCode);
		begin = Wind::util::q2DateTimeStr(beginTime);
		end = Wind::util::q2DateTimeStr(endTime);
		paras = Wind::util::qDict2WStringMapJoin(params, L';', L'=');
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	Wind::callback::Result result;
	::WQID const qid = ::EDB(code.c_str(), begin.c_str(), end.c_str(), paras.c_str(),
		&Wind::callback::strike, result.dup());
	return result.waitFor(qid);
}