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

#define KXVER 3
#include <k.h>
#pragma comment(lib, "q.lib")

// TODO: reference additional headers your program requires here

//TODO: to work-around tdbapistruct.h's improper use of "typedef enum/struct XXX { ... };" style.
#pragma warning(disable: 4091)
#include <TDBAPI.h>
#pragma warning(default: 4091)