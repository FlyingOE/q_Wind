#ifndef __COMWRAPPER_H__
#define __COMWRAPPER_H__
#pragma comment(lib, "win32.util.lib")

#include <Ole2.h>
#include <vector>

// Utilities to interface with COM library
//@ref https://support.microsoft.com/en-us/kb/216686

namespace com {

	// A std::auto_ptr<>-like manager for ::IDispatch
	template <typename Interf>
	class COM_ptr {
	public:
		COM_ptr(Interf* interf = NULL);
		~COM_ptr();
		bool isValid() const;
		Interf* get() const;
		Interf& operator*() const;
		Interf* operator->() const;
		void set(Interf* interf);
		void reset(Interf* interf = NULL);
		Interf* release();
	private:
		Interf* interf_;
	};

	// Initialize COM library (call once per thread)
	bool Initialize();
	// Uninitialize COM library (call once per Initialize() call)
	bool Uninitialize();

#	ifdef _MSC_VER
		//@ref https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(C4290);k(VS.ErrorList)&rd=true
#		pragma warning(disable: 4290)
#	endif

	std::string CLSID2str(CLSID const& clsId) throw(std::runtime_error);

	::VARIANT noOptParam();	//COM call optional parameter filler

	::IDispatch* CreateCOM(LPCOLESTR progId, DWORD context) throw(std::runtime_error);

	VARIANT COMCall(WORD context, ::IDispatch* dispatch, LPCOLESTR propName, std::vector<VARIANT> const& args) throw(std::runtime_error);

}//namespace com

#include "COMWrapper.inl"

#endif//__COMWRAPPER_H__