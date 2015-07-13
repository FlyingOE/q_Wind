#include "stdafx.h"

#if defined(_MSC_VER) && (_MSC_VER <= 1800)
//@ref http://stackoverflow.com/questions/9052224/error4error-c3861-snprintf-identifier-not-found
//@ref https://msdn.microsoft.com/en-us/library/2ts7cx93(v=vs.80).aspx
//@ref http://blogs.msdn.com/b/vcblog/archive/2014/06/18/crt-features-fixes-and-breaking-changes-in-visual-studio-14-ctp1.aspx
#include <cstdio>
int(*const std::snprintf)(char*, std::size_t, char const*, ...) = ::sprintf_s;
#endif