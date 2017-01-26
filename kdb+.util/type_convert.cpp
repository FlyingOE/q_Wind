#include "stdafx.h"
#include "type_convert.h"

#include "util.h"
#include "K_ptr.h"
#include "types.h"
#include "win32.util/CodeConvert.h"
#include "win32.util/hexDump.h"
#include <cassert>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <sstream>

long long q::q2Dec(K data) throw(std::runtime_error) {
	if (data == K_NIL) {
		throw std::runtime_error("nil decimal");
	}
	switch (data->t) {
	case -KB:
		return data->g ? 1 : 0;
	case -KG:
		return data->g;
	case -KH:
		return data->h;
	case -KI:
		return data->i;
	case -KJ:
		return data->j;
	default:
		throw std::runtime_error("not a decimal");
	}
}

std::vector<long long> q::qList2Dec(K data) throw(std::runtime_error) {
	if (data == K_NIL) {
		throw std::runtime_error("nil decimal list");
	}
	else if (data->t <= 0) {
		throw std::runtime_error("not a decimal list");
	}
	assert(data->n >= 0);
	size_t const len = static_cast<size_t>(data->n);
	std::vector<long long> result;
	result.reserve(len);
	switch (data->t) {
	case KB: {
		for (auto i = kG(data); i < kG(data) + len; ++i) {
			result.push_back(*i ? 1 : 0);
		}
		break;
	}
	case KG:
		result.resize(len, 0L);
		std::copy(kG(data), kG(data) + data->n, result.begin());
		break;
	case KH:
		result.resize(len, 0L);
		std::copy(kH(data), kH(data) + data->n, result.begin());
		break;
	case KI:
		result.resize(len, 0L);
		std::copy(kI(data), kI(data) + data->n, result.begin());
		break;
	case KJ:
		result.resize(len, 0L);
		std::copy(kJ(data), kJ(data) + data->n, result.begin());
		break;
	default:
		throw std::runtime_error("not a decimal list");
	}
	return result;
}

double q::q2Fp(K data) throw(std::runtime_error) {
	if (data == K_NIL) {
		throw std::runtime_error("nil floating-point");
	}
	switch (data->t) {
	case -KB:
	case -KG:
	case -KH:
	case -KI:
	case -KJ:
		return static_cast<double>(q2Dec(data));
	case -KE:
		return data->e;
	case -KF:
		return data->f;
	default:
		throw std::runtime_error("not a floating-point");
	}
}

std::vector<double> q::qList2Fp(K data) throw(std::runtime_error) {
	if (data == K_NIL) {
		throw std::runtime_error("nil floating-point list");
	}
	else if (data->t <= 0) {
		throw std::runtime_error("not a floating-point list");
	}
	assert(data->n >= 0);
	size_t const len = static_cast<size_t>(data->n);
	std::vector<double> result;
	result.reserve(len);
	switch (data->t) {
	case KB:
	case KG:
	case KH:
	case KI:
	case KJ: {
		std::vector<long long> decs = qList2Dec(data);
		for (auto i = decs.cbegin(); i != decs.cend(); ++i) {
			result.push_back(static_cast<double>(*i));
		}
		break;
	}
	case KE:
		result.resize(len, 0.);
		std::copy(kE(data), kE(data) + data->n, result.begin());
		break;
	case KF:
		result.resize(len, 0.);
		std::copy(kF(data), kF(data) + data->n, result.begin());
		break;
	default:
		throw std::runtime_error("not a floating-point list");
	}
	return result;
}

std::string q::q2String(K data) throw(std::runtime_error) {
	if (data == K_NIL) {
		throw std::runtime_error("nil string or symbol");
	}
	switch (data->t) {
	case KC:												// char list (a.k.a. string)
		assert(data->n >= 0);
		return std::string(kC(data), kC(data) + data->n);
	case -KS:												// symbol
		return std::string(data->s);
	default:
		if ((-K_ENUM_MAX <= data->t) && (data->t <= -K_ENUM_MIN)) {	// enumerated symbol
			K_ptr sym(k(0, "value", r1(data), K_NIL));
			assert(sym);
			return std::string(sym->s);
		}
		else {
			throw std::runtime_error("not a char list or symbol");
		}
	}
}

std::vector<std::string> q::qList2String(K data) throw(std::runtime_error) {
	if (data == K_NIL) {
		throw std::runtime_error("nil string or symbol list");
	}
	std::vector<std::string> result;
	size_t len = 0;
	switch (data->t) {
	case 0:													// char list list (a.k.a. string list)
		assert(data->n >= 0);
		len = static_cast<size_t>(data->n);
		result.reserve(len);
		for (size_t i = 0; i < len; ++i) {
			result.push_back(q2String(kK(data)[i]));
		}
		break;
	case KS:												// symbol list
		assert(data->n >= 0);
		len = static_cast<size_t>(data->n);
		result.reserve(len);
		for (size_t i = 0; i < len; ++i) {
			result.push_back(kS(data)[i]);
		}
		break;
	default:
		if ((K_ENUM_MIN <= data->t) && (data->t <= K_ENUM_MAX)) {	// enumerated symbol list
			K_ptr syms(k(0, "value", r1(data), K_NIL));
			assert(syms && (syms->n == data->n));
			assert(syms->n >= 0);
			len = static_cast<size_t>(data->n);
			result.reserve(len);
			for (size_t i = 0; i < len; ++i) {
				result.push_back(kS(syms)[i]);
			}
		}
		else {
			throw std::runtime_error("not a string or symbol list");
		}
	}
	assert(result.size() == len);
	return result;
}

std::wstring q::q2WString(K data, UINT frCP) throw(std::runtime_error) {
	std::string const input = q2String(data);
	return ml::convert(frCP, input.c_str());
}

std::vector<std::wstring> q::qList2WString(K data, UINT frCP) throw(std::runtime_error) {
	std::vector<std::string> const input = qList2String(data);
	std::vector<std::wstring> result;
	result.reserve(input.size());
	for (auto i = input.begin(); i != input.end(); ++i) {
		result.push_back(ml::convert(frCP, i->c_str()));
	}
	return result;
}

std::map<std::string, std::string> q::qDict2StringMap(K data) throw(std::runtime_error) {
	if (data == K_NIL) {
		throw std::runtime_error("nil dict");
	}
	std::vector<std::string> keys, vals;
	switch (data->t) {
	case 0:
		if (data->n != 0) {
			throw std::runtime_error("not a dict or empty list");
		}
		break;
	case XT:
		return qDict2StringMap(data->k);
	case XD:
		assert(data->n == 2);
		keys = qList2String(kK(data)[0]);
		vals = qList2String(kK(data)[1]);
		break;
	default:
		throw std::runtime_error("not a dict");
	}
	std::map<std::string, std::string> result;
	assert(keys.size() == vals.size());
	for (auto k = keys.cbegin(), v = vals.cbegin(); k != keys.cend(); ++k, ++v) {
		result[*k] = *v;
	}
	return result;
}

std::map<std::wstring, std::wstring> q::qDict2WStringMap(K data, UINT frCP) throw(std::runtime_error) {
	std::map<std::string, std::string> const input = qDict2StringMap(data);
	std::map<std::wstring, std::wstring> result;
	for (auto i = input.cbegin(); i != input.cend(); ++i) {
		std::string const& k = i->first;
		std::string const& v = i->second;
		result[ml::convert(frCP, k.c_str())] = ml::convert(frCP, v.c_str());
	}
	return result;
}

q::tm_ext q::q2tm(K data) throw(std::runtime_error) {
	if (data == K_NIL) {
		throw std::runtime_error("nil date or time or datetime");
	}
	tm_ext result;
	std::memset(&result, 0, sizeof(result));
	switch (data->t) {
	case -KD:
		if ((data->i == wi) || (data->i == -wi)) {
			throw std::runtime_error("+/-inf date");
		}
		else {
			Cookbook::gt_r((data->i == ni) ? 0 : data->i, &result);
		}
		break;
	case -KT:
		if ((data->i == wi) || (data->i == -wi)) {
			throw std::runtime_error("+/-inf time");
		}
		else {
			Cookbook::gt_r((data->i == ni) ? 0. : data->i / (24 * 60 * 60 * 1000.), &result);
		}
		break;
	case -KZ:
		if ((data->f == wf) || (data->f == -wf)) {
			throw std::runtime_error("+/-inf datetime");
		}
		else {
			Cookbook::gt_r(std::isnan(data->f) ? 0. : data->f, &result);
		}
		break;
	default:
		throw std::runtime_error("not a date or datetime");
	}
	return result;
}

std::vector<q::tm_ext> q::qList2tm(K data) throw(std::runtime_error) {
	if (data == K_NIL) {
		throw std::runtime_error("nil date or datetime list");
	}
	else if (data->t <= 0) {
		throw std::runtime_error("not a date or datetime list");
	}
	assert(data->n >= 0);
	size_t const len = static_cast<size_t>(data->n);
	std::vector<tm_ext> result(len);
	switch (data->t) {
	case KD:
		for (size_t i = 0; i < len; ++i) {
			I const& d = kI(data)[i];
			if ((d == wi) || (d == -wi)) {
				throw std::runtime_error("+/-inf date in list");
			}
			else {
				Cookbook::gt_r((d == ni) ? 0 : d, &result[i]);
			}
		}
		break;
	case KT:
		for (size_t i = 0; i < len; ++i) {
			I const& d = kI(data)[i];
			if ((d == wi) || (d == -wi)) {
				throw std::runtime_error("+/-inf time in list");
			}
			else {
				Cookbook::gt_r((d == ni) ? 0. : d / (24 * 60 * 60 * 1000.), &result[i]);
			}
		}
		break;
	case KZ:
		for (size_t i = 0; i < len; ++i) {
			F const& f = kF(data)[i];
			if ((f == wf) || (f == -wf)) {
				throw std::runtime_error("+/-inf datetime in list");
			}
			else {
				Cookbook::gt_r(std::isnan(f) ? 0. : f, &result[i]);
			}
		}
		break;
	default:
		throw std::runtime_error("not a date or datetime list");
	}
	return result;
}

::VARIANT q::q2Variant(K data) throw(std::runtime_error) {
	if (data == K_NIL) {
		throw std::runtime_error("nil value");
	}
	::VARIANT var;
	::VariantInit(&var);
	switch (data->t) {
	case 101:
		var.vt = VT_ERROR;
		var.scode = DISP_E_PARAMNOTFOUND;
		break;
	case -KB:
		var.vt = VT_BOOL;
		var.boolVal = (!!data->g) ? VARIANT_TRUE : VARIANT_FALSE;
		break;
	case -KG:
		var.vt = VT_UI1;
		static_assert(sizeof(G) == sizeof(BYTE), "type mismatch: G vs VT_BOOL");
		var.bVal = data->g;
		break;
	case -KH:
		var.vt = VT_I2;
		static_assert(sizeof(H) == sizeof(SHORT), "type mismatch: H vs VT_I2");
		var.iVal = data->h;
		break;
	case -KI:
		var.vt = VT_I4;
		static_assert(sizeof(I) == sizeof(LONG), "type mismatch: I vs VT_I4");
		var.lVal = data->i;
		break;
	case -KJ:
		var.vt = VT_I8;
		static_assert(sizeof(J) == sizeof(LONGLONG), "type mismatch: J vs VT_I8");
		var.llVal = data->j;
		break;
	case -KE:
		var.vt = VT_R4;
		static_assert(sizeof(E) == sizeof(FLOAT), "type mismatch: E vs VT_R4");
		if (std::isnan(data->e)) {
			var.fltVal = std::numeric_limits<FLOAT>::quiet_NaN();
		}
		else if (data->e == wf) {
			var.fltVal = std::numeric_limits<FLOAT>::infinity();
		}
		else if (data->e == -wf) {
			var.fltVal = - std::numeric_limits<FLOAT>::infinity();
		}
		else {
			var.fltVal = data->e;
		}
		break;
	case -KF:
		var.vt = VT_R8;
		static_assert(sizeof(F) == sizeof(DOUBLE), "type mismatch: F vs VT_R8");
		if (std::isnan(data->f)) {
			var.dblVal = std::numeric_limits<DOUBLE>::quiet_NaN();
		}
		else if (data->f == wf) {
			var.dblVal = std::numeric_limits<DOUBLE>::infinity();
		}
		else if (data->f == -wf) {
			var.dblVal = - std::numeric_limits<DOUBLE>::infinity();
		}
		else {
			var.dblVal = data->f;
		}
		break;
	case -KC:
		var.vt = VT_I1;
		static_assert(sizeof(C) == sizeof(CHAR), "type mismatch: C vs VT_I1");
		var.cVal = data->u;
		break;
	case -KD:
	case -KZ:
		var.vt = VT_DATE;
		var.date = q2DATE(data);
		break;
	case -KS:
	default: {			//BSTR or anything else (unsupported)
			std::wstring str;
			try {
				str = q2WString(data);
			}
			catch (std::runtime_error& ) {
				std::ostringstream buffer;
				buffer << "unsupported q type: " << static_cast<int>(data->t);
				throw std::runtime_error(buffer.str());
			}
			var.vt = VT_BSTR;
			var.bstrVal = ::SysAllocString(str.c_str());
			if (var.bstrVal == NULL) {
				throw std::runtime_error("BSTR E_OUTOFMEMORY");
			}
		}
	}
	return var;
}

K q::Variant2q(::VARIANT const& data) throw() {
	switch (data.vt) {
	case VT_ERROR: {
			std::ostringstream buffer;
			buffer << "VARIANT error 0x" << util::hexBytes(data.scode);
			return error2q(buffer.str());
		}
	case VT_EMPTY:
	case VT_NULL: {
			q::K_ptr id(ka(101));
			id->g = NULL;
			return id.release();
		}
	case VT_BOOL:
		return kb(!!data.boolVal);
	case VT_UI1:
		static_assert(sizeof(G) == sizeof(BYTE), "type mismatch: G vs VT_BOOL");
		return kg(data.bVal);
	case VT_I2:
		static_assert(sizeof(H) == sizeof(SHORT), "type mismatch: H vs VT_I2");
		return kh(data.iVal);
	case VT_I4:
		static_assert(sizeof(I) == sizeof(LONG), "type mismatch: I vs VT_I4");
		return ki(data.lVal);
	case VT_I8:
		static_assert(sizeof(J) == sizeof(LONGLONG), "type mismatch: J vs VT_I8");
		return kj(data.llVal);
	case VT_R4:
		static_assert(sizeof(E) == sizeof(FLOAT), "type mismatch: E vs VT_R4");
		if (std::isnan(data.fltVal)) {
			return ke(nf);
		}
		else if (std::isinf(data.fltVal)) {
			return ke((data.fltVal > 0) ? wf : -wf);
		}
		else {
			return ke(data.fltVal);
		}
	case VT_R8:
		static_assert(sizeof(F) == sizeof(DOUBLE), "type mismatch: F vs VT_R8");
		if (std::isnan(data.dblVal)) {
			return kf(nf);
		}
		else if (std::isinf(data.dblVal)) {
			return kf((data.dblVal > 0) ? wf : -wf);
		}
		else {
			return kf(data.dblVal);
		}
	case VT_I1:
		static_assert(sizeof(C) == sizeof(CHAR), "type mismatch: C vs VT_I1");
		return kc(data.cVal);
	case VT_DATE:
		return kz(DATE2q(data.date));
	case VT_BSTR: {
			std::string const str = ml::convert(CP_UTF8, data.bstrVal);
			return kp(const_cast<S>(str.c_str()));
		}
	default: {
			std::ostringstream buffer;
			buffer << "unsupported VARIANT type: 0x" << util::hexBytes(data.vt);
			return error2q(buffer.str());
		}
	}
}

I q::date2q(int yyyymmdd) {
	int const year = yyyymmdd / 10000;
	int const month = (yyyymmdd % 10000) / 100;
	int const mday = yyyymmdd % 100;
	return ymd(year, month, mday);
}

I q::time2q(int hhmmssfff) {
	int const hh = hhmmssfff / 10000000;
	int const mm = (hhmmssfff % 10000000) / 100000;
	int const ssfff = hhmmssfff % 100000;
	return (hh * 60 + mm) * 60000 + ssfff;
}

I q::date2q(char const* dateStr) throw(std::runtime_error) {
	assert(dateStr != NULL);
	int year = 0, month = 0, mday = 0;
	static char const* FMTs[] = { "%4d.%2d.%2d", "%4d-%2d-%2d", "%4d/%2d/%2d" };
	for (int i = 0; i < _countof(FMTs); ++i) {
#		ifdef _MSC_VER
		int const result = ::sscanf_s(dateStr, FMTs[i], &year, &month, &mday);
#		else
		int const result = std::sscanf(dateStr, FMTs[i], &year, &month, &mday);
#		endif
		if (result == 3) {
			return ymd(year, month, mday);
		}
	}
	throw std::runtime_error("invalid date string");
}

I q::date2q(std::string const& dateStr) throw(std::runtime_error) {
	return date2q(dateStr.c_str());
}

std::time_t tm2time_t(std::tm const& tm) {
#	ifdef _MSC_VER

	//NOTE: MSVC's <time.h> cannot handle anything before Unix epoch! Use Win32 API instead.
	::SYSTEMTIME systime = {
		tm.tm_year + 1900,
		tm.tm_mon + 1,
		tm.tm_wday,
		tm.tm_mday,
		tm.tm_hour,
		tm.tm_min,
		tm.tm_sec,
		0
	};
	::FILETIME filetime = { 0 };
	if (!::SystemTimeToFileTime(&systime, &filetime))
		return static_cast<std::time_t>(-1);

	//@ref https://support.microsoft.com/en-us/kb/167296
	LONGLONG const result = *reinterpret_cast<LONGLONG*>(&filetime) / 10000000LL - 11644473600LL;
	if ((std::numeric_limits<std::time_t>::min() > result) || (result > std::numeric_limits<std::time_t>::max()))
		return static_cast<std::time_t>(-1);
	return static_cast<std::time_t>(result);

#	else//_MSC_VER

#	error TO BE IMPLEMENTED
	// Detect local time zone offset and adjust accordingly
	std::tm tzOffset = { 0 };
	std::time_t tzOffsetProbe = 0;
	std::localtime_r(&tzOffsetProbe, &tzOffset);
	tm.tm_hour += tzOffset.tm_hour;
	tm.tm_min += tzOffset.tm_min;

	return std::mktime(&tm);

#	endif//_MSC_VER
}

q::tm_ext time_t2tm(std::time_t const time, int millis = 0) {
#	ifdef _MSC_VER

	//NOTE: MSVC's <time.h> cannot handle anything before Unix epoch! Use Win32 API instead.
	//@ref https://support.microsoft.com/en-us/kb/167296
	LONGLONG ft64 = time * 10000000LL + millis * 10000LL + 116444736000000000LL;
	::FILETIME filetime = {
		static_cast<DWORD>(ft64),
		static_cast<DWORD>(ft64 >> 32)
	};
	q::tm_ext tm;
	std::memset(&tm, 0, sizeof(tm));
	::SYSTEMTIME systime = { 0 };
	if (!::FileTimeToSystemTime(&filetime, &systime)) {
		std::memset(&tm, -1, sizeof(tm));
		return tm;
	}

	tm.tm_year = systime.wYear - 1900;
	tm.tm_mon = systime.wMonth - 1;
	tm.tm_mday = systime.wDay;
	tm.tm_wday = systime.wDayOfWeek;
	tm.tm_hour = systime.wHour;
	tm.tm_min = systime.wMinute;
	tm.tm_sec = systime.wSecond;
	tm.tm_millis = systime.wMilliseconds;
	return tm;

#	else//_MSC_VER

#	error TO BE IMPLEMENTED

#	endif//_MSC_VER
}

// Normal year: ydays for each month
int const MONTH_DAYS[13] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

//@ref http://www.codeguru.com/cpp/cpp/cpp_mfc/article.php/c765/An-ATL-replacement-for-COleDateTime.htm
//@ref http://www.codeproject.com/Articles/144159/Time-Format-Conversion-Made-Easy
F q::DATE2q(::DATE date) throw(std::runtime_error) {
	// Negative DATEs are not continuous!
	if (date < 0) {
		F const d = std::ceil(date);
		date = d - (date - d);
	}

	static J const MIN_DATE = -657434L;	//~ year 0100
	static J const MAX_DATE = 2958465L;	//~ year 9999
	if ((date < MIN_DATE) || (MAX_DATE < date)) {
		throw std::runtime_error("DATE out of range");
	}

	std::tm tm = { 0 };
	// # days since 1899.12.30
	J nDays1899 = static_cast<J>(std::floor(date));
	// Seconds since midnight
	F nSecs = (date - nDays1899) * (60L * 60 * 24);
	// # days since 0000.01.01
	J nDaysAbs = nDays1899 + 693959L;
	// Known: 0000.01.01 was a Saturday
	tm.tm_wday = static_cast<int>((nDaysAbs - 1) % 7L);
	
	// # 400-year blocks since 0000.01.01
	J n400Years = static_cast<J>(nDaysAbs / 146097L);
	J nDaysIn400Year = nDaysAbs % 146097L;
	// # century since beginning of the 400-year block (1st century had an extra day)
	J nCenturyIn400Year = static_cast<J>((nDaysIn400Year - 1) / 36524L);
	
	// If a 4-year block contains a leap year
	bool leapIn4Year = true;
	J n4Years, nDaysIn4Year;
	// Special treatment for a leap century
	if (nCenturyIn400Year == 0) {
		n4Years = static_cast<J>(nDaysIn400Year / 1461L);
		nDaysIn4Year = static_cast<J>(nDaysIn400Year % 1461L);
	}
	else {
		// Day within the century
		J nDaysInCentury = (nDaysIn400Year - 1) % 36524L;
		// 1st 4-year increment had only 1460 days
		n4Years = static_cast<J>((nDaysInCentury + 1) / 1461L);
		leapIn4Year = (n4Years != 0);
		// Day within 4-year block (yr1.01.01 => 0; yr4.12.31 => 1460)
		nDaysIn4Year = leapIn4Year ? static_cast<J>((nDaysInCentury + 1) % 1461L) : nDaysInCentury;
	}

	J nYearsIn4Year, nDaysInYear;
	if (leapIn4Year) {
		// Year within the 4-year block (1st year had 366 days)
		nYearsIn4Year = (nDaysIn4Year - 1) / 365;
		nDaysInYear = (nYearsIn4Year != 0) ? ((nDaysIn4Year - 1) % 365) : nDaysIn4Year;
	}
	else {
		nYearsIn4Year = nDaysIn4Year / 365;
		nDaysInYear = nDaysIn4Year % 365;
	}
	tm.tm_year = static_cast<int>(n400Years * 400 + nCenturyIn400Year * 100 + n4Years * 4 + nYearsIn4Year) - 1900;
	// std::tm::tm_yday is 0-based, buth std::tm::tm_mday is 1-based!
	tm.tm_yday = static_cast<int>(nDaysInYear++);
	
	// Special handling for leap years before/on/after yyyyy.02.29
	bool feb29 = false;
	if ((nYearsIn4Year == 0) && leapIn4Year) {	// A leap year
		if (nDaysInYear == MONTH_DAYS[2] + 1) {
			feb29 = true;
			tm.tm_mon = 2 - 1;
			tm.tm_mday = 29;
		}
		else if (nDaysInYear > MONTH_DAYS[2] + 1) {
			--nDaysInYear;
		}
	}
	if (!feb29) {
		// Month number always >= n/32, so reduce loop time
		for (tm.tm_mon = static_cast<int>(nDaysInYear / 32 + 1);
			nDaysInYear > MONTH_DAYS[tm.tm_mon];
			++tm.tm_mon);
		assert(tm.tm_mon >= 1);
		tm.tm_mday = static_cast<int>(nDaysInYear - MONTH_DAYS[--tm.tm_mon]);
	}

	// Shortcut for integral (date-only) DATEs
	J nSecsOnly = static_cast<J>(nSecs);
	if (nSecs == 0.) {
		tm.tm_hour = tm.tm_min = tm.tm_sec = 0;
	}
	else {
		tm.tm_sec = static_cast<int>(nSecsOnly % 60L);
		J nMins = nSecsOnly / 60L;
		tm.tm_min = static_cast<int>(nMins % 60);
		tm.tm_hour = static_cast<int>(nMins / 60);
	}
	// DST info is not available
	tm.tm_isdst = -1;

	// Convert back to a q datetime
	std::time_t const time = tm2time_t(tm);
	if (time == static_cast<std::time_t>(-1)) {
		throw std::runtime_error("DATE out of range for system std::time_t");
	}
	return Cookbook::zu(time + (nSecs - nSecsOnly));
}

//@ref http://www.codeguru.com/cpp/cpp/cpp_mfc/article.php/c765/An-ATL-replacement-for-COleDateTime.htm
//@ref http://www.codeproject.com/Articles/144159/Time-Format-Conversion-Made-Easy
::DATE q::q2DATE(K d) throw(std::runtime_error) {
	tm_ext const tm = q2tm(d);
	int const year = tm.tm_year + 1900;
	int const month = tm.tm_mon + 1;
	assert(year < 9999);
	assert((1 <= month) && (month <= 12));

	bool const leap = (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
	int const nDaysInMonth =
		MONTH_DAYS[month] - MONTH_DAYS[month - 1] +
		((leap & (month == 2) && (tm.tm_mday >= 29)) ? 1 : 0);
	assert((1 <= tm.tm_mday) && (tm.tm_mday <= nDaysInMonth));
	assert((0 <= tm.tm_hour) && (tm.tm_hour < 24));
	assert((0 <= tm.tm_min) && (tm.tm_min < 60));
	assert((0 <= tm.tm_sec) && (tm.tm_sec <= 60));	//possible leap second
	assert((0 <= tm.tm_millis) && (tm.tm_millis < 1000));

	long date =
		year * 365L + year / 4 - year / 100 + year / 400 +
		MONTH_DAYS[month - 1] + tm.tm_mday;
	
	// If leap year and before March, subtract 1 day
	if ((month <= 2) && leap) --date;
	// Offset from 1899-12-30
	date -= 693959L;
	// Calculate fractional date
	double time = (tm.tm_hour * 3600. + tm.tm_min * 60. + tm.tm_sec + tm.tm_millis / 1000.) / 86400.;

	return date + ((date >= 0) ? time : -time);
}
