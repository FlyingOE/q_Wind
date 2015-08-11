#include "stdafx.h"
#include "WindEvent.h"

#include "util.h"
#include "WindData.h"

#include "kdb+.util/util.h"
#include <sstream>

Wind::Event::Event(::WQEvent const& event) : ::WQEvent(event) {
	this->pQuantData = (event.pQuantData == NULL) ? NULL : new Data(*event.pQuantData);
}

Wind::Event::Event(::WQEvent& event) : ::WQEvent(event) {
	this->pQuantData = (event.pQuantData == NULL) ? NULL : new Data(*event.pQuantData);
}

Wind::Event::~Event() throw() {
	clear();
}

void Wind::Event::clear() {
	Data* data = static_cast<Data*>(this->pQuantData);
	if (data != NULL) data->clear();
	delete data;
	this->pQuantData = NULL;
}

::WQEvent Wind::Event::release() {
	::WQEvent dup(*this);	// ownership transfer
	this->pQuantData = NULL;
	return dup;
}

K Wind::Event::parse() const throw() {
	if (ErrCode != WQERR_OK) {
		return q::error2q(util::error2Text(ErrCode));
	}
	else if (this->pQuantData == NULL) {
		std::ostringstream buffer;
		buffer << "No data: " << *this;
		return q::error2q(buffer.str());
	}
	else {
		return static_cast<Data*>(this->pQuantData)->parse();
	}
}