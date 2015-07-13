// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX	// Get rid of the annoying min/max macros
// Windows Header Files:
#include <windows.h>

// Work-around MSVC's lack of std::snprintf(...) till VS2013
#if defined(_MSC_VER) && (_MSC_VER <= 1800)
namespace std {
	extern int(*const snprintf)(char* s, size_t n, char const* format, ...);
}
#endif

#define KXVER 3
#include <k.h>

// TODO: reference additional headers your program requires here
#include <TDBAPI.h>