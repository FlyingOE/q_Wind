#ifndef __KDB__UTIL_H__
static_assert(0, "Include kdb+.util/util.h instead!");
#endif

template <typename Char, typename Traits>
std::basic_ostream<Char, Traits>& q::operator<<(std::basic_ostream<Char, Traits>& os, std::tm const& tm) {
	char buffer[(10 + 1 + 8) + 1] = { '\0' };
	std::strftime(buffer, _countof(buffer), "%Y-%m-%dT%H:%M:%S", &tm);
	assert(buffer[_countof(buffer)] == '\0');	// buffer should be properly terminated
	return os << buffer;
}