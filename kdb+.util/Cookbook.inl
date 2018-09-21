//NOTE Include this only after <k.h> and <ctime>
#ifndef __COOKBOOK_INL__
#define __COOKBOOK_INL__

#include "win32.util/msvc.h"

#include <cassert>
#include <ctime>

// Utilities to deal with q datetime/timestamp data types in C.
//@see http://code.kx.com/wiki/Cookbook/InterfacingWithC#Strings_and_datetimes
namespace q {
	namespace Cookbook {

		inline F zu(F u) {
			return u / 86400. - 10957;
		}
		inline F zu(I u) {	// kdb+ datetime from unix
			return zu(static_cast<F>(u));
		}
		inline F uz(F f) {	// unix from kdb+ datetime
			return 86400 * (f + 10957);
		}
		inline J pu(F u) {
			return static_cast<J>(8.64e13 * (u / 86400. - 10957));
		}
		inline J pu(I u) {	// kdb+ timestamp from unix, use ktj(Kj,n) to create timestamp from n
			return pu(static_cast<F>(u));
		}
		inline I up(J f) {	// unix from kdb+ timestamp
			return static_cast<I>((f / 8.64e13 + 10957) * 86400);
		}
		/*// Unsafe!
		inline std::tm* lt(int kd) {
			std::time_t t = uz(kd);
			return std::localtime(&t);
		}
		*/
		template <typename T>
		tm_ext* lt_r(T kd, tm_ext* res) {
			F tt = uz(kd);
			std::time_t t = static_cast<std::time_t>(tt);
			res->tm_millis = static_cast<int>(std::round((tt - t) * 1000));
#			ifdef _MSC_VER
			::errno_t err = ::localtime_s(res, &t);
			assert(err == 0);
			return res;
#			else
			return std::localtime_r(&t, res);
#			endif
		}
		/*// Unsafe!
		inline std::tm* gt(int kd) {
			std::time_t t = uz(kd);
			return std::gmtime(&t);
		}
		*/
		template <typename T>
		tm_ext* gt_r(T kd, tm_ext* res) {
			F tt = uz(kd);
			std::time_t t = static_cast<std::time_t>(tt);
			tm_ext tm = time_t2tm(t, static_cast<int>(std::round((tt - t) * 1000)));
			std::memcpy(res, &tm, sizeof(tm));
			return res;
			/*
			res->tm_millis = static_cast<int>(std::round((tt - t) * 1000));
#			ifdef _MSC_VER
			::errno_t err = ::gmtime_s(res, &t);
			assert(err == 0);
			return res;
#			else
			return std::gmtime_r(&t, res);
#			endif
			*/
		}
		inline char* fdt(std::tm* ptm, char* d) {
			std::strftime(d, (4 + 1 + 2 + 1 + 2) + 1, "%Y.%m.%d", ptm);
			return d;
		}
		inline void tsms(unsigned ts, char*h, char*m, char*s, short*mmm) {
			*h = ts / 3600000;
			ts -= 3600000 * (*h);
			*m = ts / 60000;
			ts -= 60000 * (*m);
			*s = ts / 1000;
			ts -= 1000 * (*s);
			*mmm = ts;
		}
		inline char* ftsms(unsigned ts, char* d){
			char h, m, s;
			short mmm;
			tsms(ts, &h, &m, &s, &mmm);
			int const count = std::snprintf(d, (2 + 1 + 2 + 1 + 2 + 1 + 3) + 1, "%02d:%02d:%02d.%03d", h, m, s, mmm);
			assert(count == 2 + 1 + 2 + 1 + 2 + 1 + 3);
			return d;
		}

	}//namespace q::Cookbook
}//namespace q

#endif//__COOKBOOK_INL__