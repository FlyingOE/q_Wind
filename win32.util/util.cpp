#include "stdafx.h"
#include "util.h"

#if defined(_MSC_VER) && (_MSC_VER <= 1800)
//@ref http://stackoverflow.com/questions/9052224/error4error-c3861-snprintf-identifier-not-found
//@ref https://msdn.microsoft.com/en-us/library/2ts7cx93(v=vs.80).aspx
//@ref http://blogs.msdn.com/b/vcblog/archive/2014/06/18/crt-features-fixes-and-breaking-changes-in-visual-studio-14-ctp1.aspx
#include <cstdio>
int(*const std::snprintf)(char*, size_t, char const*, ...) = ::sprintf_s;
#endif

util::hexByteGenerator util::hexBytes(char const* p, size_t n, bool reverse) {
	return hexBytes(reinterpret_cast<unsigned char const*>(p), n, reverse);
}

util::hexByteGenerator util::hexBytes(unsigned char const* p, size_t n, bool reverse) {
	return hexByteGenerator(p, p + n, reverse);
}