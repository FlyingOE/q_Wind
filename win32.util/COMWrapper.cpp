#include "stdafx.h"
#include "COMWrapper.h"

//@see https://code.msdn.microsoft.com/office/CppAutomateExcel-be36992e
//@see https://support.microsoft.com/en-us/kb/196776
#include "CodeConvert.h"
#include "hexDump.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include <comdef.h>
#include <Ole2.h>

std::string com::getError(::HRESULT const& hr) {
	::_com_error const error(hr);
	std::ostringstream buffer;
	buffer << "[0x" << util::hexBytes(hr) << "] "
		<< ml::convert(CP_UTF8, error.ErrorMessage());
	return buffer.str();
}

//@see https://support.microsoft.com/en-us/kb/216686
bool com::Initialize() {
	::HRESULT const result = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	switch (result) {
	case S_OK:
		//std::cerr << "COM library initialized." << std::endl;
		return true;
	case S_FALSE:
		std::cerr << "COM library has already been initialized." << std::endl;
		return true;
	default:
		std::cerr << "COM library initialization error: " << getError(result) << std::endl;
		return true;
	}
}

bool com::Uninitialize() {
	::CoUninitialize();
	//std::cerr << "COM library uninitialized." << std::endl;
	return true;
}

std::string com::CLSID2str(CLSID const& clsId) throw(std::runtime_error) {
	LPOLESTR pClsId = NULL;
	HRESULT hr = ::StringFromCLSID(clsId, &pClsId);
	if (FAILED(hr)) {
		throw std::runtime_error(getError(hr));
	}
	assert(pClsId != NULL);
	std::wstring clsIdStr(pClsId);	//swap out memory management from COM API
	::CoTaskMemFree(pClsId);
	pClsId = NULL;

	return ml::convert(CP_ACP, clsIdStr.c_str());
}


::IDispatch* com::CreateCOM(LPCOLESTR progId, DWORD context) throw(std::runtime_error) {
	std::string const pId = ml::convert(CP_ACP, progId);

	// Lookup for CLSID
	::CLSID clsId = { 0 };
	HRESULT hr = ::CLSIDFromProgID(progId, &clsId);
	if (FAILED(hr)) {
		std::ostringstream buffer;
		buffer << "CLSIDFromProgID(\"" << pId << "\")" << ": " << getError(hr);
		throw std::runtime_error(buffer.str());
	}

	// Create COM server
	COM_ptr<::IDispatch> dispatch(NULL);
	hr = ::CoCreateInstance(clsId, NULL, context, IID_IDispatch, reinterpret_cast<LPVOID*>(&dispatch));
	if (FAILED(hr)) {
		assert(!dispatch.isValid());
		std::ostringstream buffer;

		// Wind�����WDF.Addin COM������ᵼ������Ĵ���Ҫ���ԡ���
		if (RPC_E_SERVERCALL_RETRYLATER == hr) {
			std::cerr << "CoCreateInstance(" << CLSID2str(clsId) << ", " << util::hexBytes(context) << ", ...)"
					<< " error: RPC_E_SERVERCALL_RETRYLATER. Retrying..." << std::endl;

			COM_ptr<IUnknown> unknown(NULL);
			IUnknown* unkw = NULL;
			hr = ::GetActiveObject(clsId, NULL, &unkw);
			if (FAILED(hr)) {
				assert(NULL == unkw);
				buffer << "GetActiveObject(" << CLSID2str(clsId) << ')' << ": " << getError(hr);
				throw std::runtime_error(buffer.str());
			}

			unknown.reset(unkw);	//swap out resource management
			assert(unknown.isValid());
			hr = unknown->QueryInterface(IID_IDispatch, reinterpret_cast<LPVOID*>(&dispatch));
			if (FAILED(hr)) {
				assert(!dispatch.isValid());
				buffer << "0x" << util::hexBytes(unknown.get()) << "->QueryInterface(IID_IDispatch)" << ": " << getError(hr);
				throw std::runtime_error(buffer.str());
			}
		}
		else {
			buffer << "CoCreateInstance(" << CLSID2str(clsId) << ", " << util::hexBytes(context) << ", ...)" << ": " << getError(hr);
			throw std::runtime_error(buffer.str());
		}
	}

	assert(dispatch.isValid());
	return dispatch.release();
}

VARIANT com::COMCall(WORD context, ::IDispatch* dispatch, LPCOLESTR propName, std::vector<VARIANT> const& args) throw(std::runtime_error)
{
	// Argument check & encoding conversion
	if (NULL == dispatch) {
		throw std::runtime_error("NULL IDispatch");
	}
	std::string const prop = ml::convert(CP_ACP, propName);

	// Look up for DISPID
	::DISPID dispId;
	HRESULT hr = dispatch->GetIDsOfNames(IID_NULL, const_cast<LPOLESTR*>(&propName), 1, LOCALE_USER_DEFAULT, &dispId);
	if (FAILED(hr)) {
		std::ostringstream buffer;
		buffer << "IDispatch::GetIDsOfNames(..., \"" << prop << "\", ...)" << ": " << getError(hr);
		throw std::runtime_error(buffer.str());
	}

	// COM invocation parameters
	::DISPPARAMS params = { NULL, NULL, 0, 0 };
	if (args.size() > std::numeric_limits<UINT>::max()) {
		throw std::runtime_error("argument list too long for UINT");
	}
	params.cArgs = static_cast<UINT>(args.size());
	params.rgvarg = const_cast<VARIANT*>(args.empty() ? NULL : &args[0]);
	::DISPID dispidNamed = DISPID_PROPERTYPUT;
	if (context & DISPATCH_PROPERTYPUT) {
		params.cNamedArgs = 1;
		params.rgdispidNamedArgs = &dispidNamed;
	}

	// COM service invocation
	VARIANT result;
	::VariantInit(&result);
	hr = dispatch->Invoke(dispId, IID_NULL, LOCALE_SYSTEM_DEFAULT, context, &params, &result, NULL, NULL);
	if (FAILED(hr)) {
		std::ostringstream buffer;
		buffer << "IDispatch::Invoke(\"" << prop << "\"=0x" << util::hexBytes(dispId) << ')' << ": " << getError(hr);
		throw std::runtime_error(buffer.str());
	}

	return result;
}