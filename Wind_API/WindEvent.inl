#ifndef __WINDEVENT_H__
static_assert(0, "Include Wind_API/WindEvent.h instead!");
#endif

#include <iomanip>

template <typename Char, typename Traits>
std::basic_ostream<Char, Traits>& Wind::operator<<(std::basic_ostream<Char, Traits>& os, ::WQEvent const& event) {
	os << "{(" << event.Version << ')'
		<< "Type:[" << event.EventType << ']' << util::eventType2Text(event.EventType) << ','
		<< "ID:" << event.EventID << ','
		<< "ReqID:" << event.RequestID << ',';
	if (event.ErrCode != WQERR_OK) {
		os << "Err:[" << event.ErrCode << ']' << util::error2Text(event.ErrCode) << ',';
	}
	return os << "Data:*" << std::setiosflags(std::ios::uppercase) << std::hex
		<< std::setfill('0') << std::setw(sizeof(event.pQuantData) * 2)
		<< event.pQuantData
		<< '}';
}