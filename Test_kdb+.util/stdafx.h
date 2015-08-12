// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"

// TODO: reference additional headers your program requires here
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#define KXVER 3
#include <k.h>

#ifndef NO_Q_LIB
static_assert(0, "Must define NO_Q_LIB in order to link with c.lib!");
#endif
#include "kdb+.hack/hack.h"
#pragma comment(lib, "c.lib")