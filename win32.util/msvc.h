#ifndef __WIN32_UTIL__MSVC_H__
#define __WIN32_UTIL__MSVC_H__

// Work-around MSVC's lack of std::snprintf(...) till VS2013
#if defined(_MSC_VER) && (_MSC_VER <= 1800)
namespace std {
	extern int(*const snprintf)(char* s, size_t n, char const* format, ...);
}
#endif

#endif//__WIN32_UTIL__MSVC_H__