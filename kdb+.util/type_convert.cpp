#include "stdafx.h"
#include "type_convert.h"

#include "util.h"
#include "K_ptr.h"
#include <cassert>
#include <cmath>
#include <ctime>
#include <algorithm>

int q::UTC_OFFSET = 0;

#define ENUMmin	(20)
#define ENUMmax	(76)

long long q::q2Dec(K data) throw(std::string) {
	if (data == K_NIL) {
		throw std::string("nil decimal");
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
		throw std::string("not a decimal");
	}
}

std::vector<long long> q::qList2Dec(K data) throw(std::string) {
	if (data == K_NIL) {
		throw std::string("nil decimal list");
	}
	else if (data->t <= 0) {
		throw std::string("not a decimal list");
	}
	assert(data->n >= 0);
	std::vector<long long> result(static_cast<size_t>(data->n), 0L);
	switch (data->t) {
	case KB: {
		struct Converter {
			long long operator()(G x) const { return x ? 1 : 0; }
		};
		std::transform(kG(data), kG(data) + data->n, result.begin(), Converter());
		break;
	}
	case KG:
		std::copy(kG(data), kG(data) + data->n, result.begin());
		break;
	case KH:
		std::copy(kH(data), kH(data) + data->n, result.begin());
		break;
	case KI:
		std::copy(kI(data), kI(data) + data->n, result.begin());
		break;
	case KJ:
		std::copy(kJ(data), kJ(data) + data->n, result.begin());
		break;
	default:
		throw std::string("not a decimal list");
	}
	return result;
}

double q::q2Fp(K data) throw(std::string) {
	if (data == K_NIL) {
		throw std::string("nil floating-point");
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
		throw std::string("not a floating-point");
	}
}

std::vector<double> q::qList2Fp(K data) throw(std::string) {
	if (data == K_NIL) {
		throw std::string("nil floating-point list");
	}
	else if (data->t <= 0) {
		throw std::string("not a floating-point list");
	}
	assert(data->n >= 0);
	std::vector<double> result(static_cast<size_t>(data->n), 0.);
	switch (data->t) {
	case KB:
	case KG:
	case KH:
	case KI:
	case KJ: {
		std::vector<long long> decs = qList2Dec(data);
		struct Converter {
			double operator()(long long x) const { return static_cast<double>(x); }
		};
		std::transform(decs.begin(), decs.end(), result.begin(), Converter());
		break;
	}
	case KE:
		std::copy(kE(data), kE(data) + data->n, result.begin());
		break;
	case KF:
		std::copy(kF(data), kF(data) + data->n, result.begin());
		break;
	default:
		throw std::string("not a floating-point list");
	}
	return result;
}

std::string q::q2String(K data) throw(std::string) {
	if (data == K_NIL) {
		throw std::string("nil string or symbol");
	}
	switch (data->t) {
	case KC:												// char list (a.k.a. string)
		assert(data->n >= 0);
		return std::string(kC(data), kC(data) + data->n);
	case -KS:												// symbol
		return std::string(data->s);
	default:
		if ((-ENUMmax <= data->t) && (data->t <= -ENUMmin)) {	// enumerated symbol
			K_ptr sym(k(0, "value", r1(data), K_NIL));
			assert(sym);
			return std::string(sym->s);
		}
		else {
			throw std::string("not a char list or symbol");
		}
	}
}

std::vector<std::string> q::qList2String(K data) throw(std::string) {
	if (data == K_NIL) {
		throw std::string("nil string or symbol list");
	}
	std::vector<std::string> result;
	switch (data->t) {
	case 0:													// char list list (a.k.a. string list)
		assert(data->n >= 0);
		result.reserve(static_cast<size_t>(data->n));
		for (J i = 0; i < data->n; ++i) {
			result.push_back(q2String(kK(data)[i]));
		}
		break;
	case KS:												// symbol list
		assert(data->n >= 0);
		result.reserve(static_cast<size_t>(data->n));
		for (J i = 0; i < data->n; ++i) {
			result.push_back(kS(data)[i]);
		}
		break;
	default:
		if ((ENUMmin <= data->t) && (data->t <= ENUMmax)) {	// enumerated symbol list
			K_ptr syms(k(0, "value", r1(data), K_NIL));
			assert(syms && (syms->n == data->n));
			assert(syms->n >= 0);
			result.reserve(static_cast<size_t>(syms->n));
			for (J i = 0; i < data->n; ++i) {
				result.push_back(kS(syms)[i]);
			}
		}
		else {
			throw std::string("not a string or symbol list");
		}
	}
	assert(result.size() == data->n);
	return result;
}

std::wstring q::q2WString(K data, UINT frCP) throw(std::string) {
	std::string const input = q2String(data);
	return ml::convert(frCP, input.c_str());
}

std::vector<std::wstring> q::qList2WString(K data, UINT frCP) throw(std::string) {
	std::vector<std::string> const input = qList2String(data);
	std::vector<std::wstring> result;
	result.reserve(input.size());
	for (auto i = input.begin(); i != input.end(); ++i) {
		result.push_back(ml::convert(frCP, i->c_str()));
	}
	return result;
}

std::map<std::string, std::string> q::qDict2StringMap(K data) throw(std::string) {
	if (data == K_NIL) {
		throw std::string("nil dict");
	}
	std::vector<std::string> keys, vals;
	switch (data->t) {
	case 0:
		if (data->n != 0) {
			throw std::string("not a dict or empty list");
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
		throw std::string("not a dict");
	}
	assert(keys.size() == vals.size());
	std::map<std::string, std::string> result;
	for (auto k = keys.begin(), v = vals.begin(); k != keys.end(); ++k, ++v) {
		result[*k] = *v;
	}
	return result;
}

std::map<std::wstring, std::wstring> q::qDict2WStringMap(K data, UINT frCP) throw(std::string) {
	std::map<std::string, std::string> const input = qDict2StringMap(data);
	std::map<std::wstring, std::wstring> result;
	for (auto i = input.begin(); i != input.end(); ++i) {
		std::string const& k = i->first;
		std::string const& v = i->second;
		result[ml::convert(frCP, k.c_str())] = ml::convert(frCP, v.c_str());
	}
	return result;
}

q::tm_ext q::q2tm(K data) throw(std::string) {
	if (data == K_NIL) {
		throw std::string("nil date or datetime");
	}
	tm_ext result;
	std::memset(&result, 0, sizeof(tm_ext));
	switch (data->t) {
	case -KD:
		Cookbook::gt_r((data->i == ni) ? 0 : data->i, &result);
		break;
	case -KZ:
		Cookbook::gt_r((data->f == nf) ? 0. : data->f, &result);
		break;
	default:
		throw std::string("not a date or datetime");
	}
	return result;
}

std::vector<q::tm_ext> q::qList2tm(K data) throw(std::string) {
	if (data == K_NIL) {
		throw std::string("nil date or datetime list");
	}
	else if (data->t <= 0) {
		throw std::string("not a date or datetime list");
	}
	assert(data->n >= 0);
	std::vector<tm_ext> result(static_cast<size_t>(data->n));
	switch (data->t) {
	case KD:
		for (size_t i = 0; i < data->n; ++i) {
			Cookbook::gt_r((kI(data)[i] == ni) ? 0 : kI(data)[i], &result[i]);
		}
		break;
	case KZ:
		for (size_t i = 0; i < data->n; ++i) {
			Cookbook::gt_r((kF(data)[i] == nf) ? 0. : kF(data)[i], &result[i]);
		}
		break;
	default:
		throw std::string("not a date or datetime list");
	}
	return result;
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

I q::date2q(char const* dateStr) throw(std::string) {
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
	throw std::string("invalid date string");
}

I q::date2q(std::string const& dateStr) throw(std::string) {
	return date2q(dateStr.c_str());
}

//@ref http://www.codeguru.com/cpp/cpp/cpp_mfc/article.php/c765/An-ATL-replacement-for-COleDateTime.htm
F q::DATE2q(::DATE date) throw(std::string) {
	// Negative DATEs are not continuous!
	if (date < 0) {
		F const d = std::ceil(date);
		date = d - (date - d);
	}

	static J const MIN_DATE = -657434L;	//~ year 0100
	static J const MAX_DATE = 2958465L;	//~ year 9999
	if ((date < MIN_DATE) || (MAX_DATE < date)) {
		throw std::string("DATE out of range");
	}

	// Normal year: ydays for each month
	static J const MONTH_DAYS[13] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

	std::tm tm = { 0 };
	// # days since 1899.12.30
	J nDays1899 = static_cast<J>(date);
	// Seconds since midnight
	F nSecs = (date - std::floor(date)) * (60L * 60 * 24);
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
		tm.tm_hour = UTC_OFFSET;
		tm.tm_min = tm.tm_sec = 0;
	}
	else {
		tm.tm_sec = static_cast<int>(nSecsOnly % 60L);
		J nMins = nSecsOnly / 60L;
		tm.tm_min = static_cast<int>(nMins % 60);
		tm.tm_hour = static_cast<int>(nMins / 60) + UTC_OFFSET;
	}
	// DST info is not available
	tm.tm_isdst = -1;

	// Convert back to a q datetime
	std::time_t const time = std::mktime(&tm);
	if ((time < std::numeric_limits<I>::min()) || (std::numeric_limits<I>::max() < time)) {
		throw std::string("DATE out of range for datetime");
	}
	return Cookbook::zu(time + (nSecs - nSecsOnly));
}