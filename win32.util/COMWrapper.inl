#ifndef __COMWRAPPER_H__
static_assert(0, "Include win32.util/COMWrapper.h instead!");
#endif

//#include <iostream>

template <typename Interf>
com::COM_ptr<Interf>::COM_ptr(Interf* interf) : interf_(interf) {
}

template <typename Interf>
com::COM_ptr<Interf>::~COM_ptr() {
	reset();
}

template <typename Interf>
Interf* com::COM_ptr<Interf>::get() const {
	return interf_;
}

template <typename Interf>
Interf& com::COM_ptr<Interf>::operator*() const {
	if (!isValid()) {
		throw std::runtime_error("NULL COM object");
	}
	else {
		return *get();
	}
}

template <typename Interf>
Interf* com::COM_ptr<Interf>::operator->() const {
	if (!isValid()) {
		throw std::runtime_error("NULL COM object");
	} else {
		return get();
	}
}

template <typename Interf>
void com::COM_ptr<Interf>::set(Interf* interf) {
	reset(interf);
	if (NULL != interf_) {
		ULONG const refs = interf_->AddRef();
//		std::cout << "0x" << util::hexBytes(interf_) << " (++=" << refs << ')' << std::endl;
	}
}

template <typename Interf>
void com::COM_ptr<Interf>::reset(Interf* interf) {
	if (isValid()) {
		ULONG const refs = interf_->Release();
//		std::cout << "0x" << util::hexBytes(interf_) << " (--=" << refs << ')' << std::endl;
		interf_ = NULL;
	}
	interf_ = interf;
}

template <typename Interf>
Interf* com::COM_ptr<Interf>::release() {
	Interf* interf = interf_;
	interf_ = NULL;
	return interf;
}

template <typename Interf>
bool com::COM_ptr<Interf>::isValid() const {
	return interf_ != NULL;
}