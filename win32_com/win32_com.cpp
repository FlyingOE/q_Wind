#include "stdafx.h"
#include "win32_com.h"

#include "win32.util/hexDump.h"
#include "win32.util/COMWrapper.h"
#include "kdb+.util/types.h"
#include "kdb+.util/util.h"
#include "kdb+.util/type_convert.h"
#include "kdb+.util/K_ptr.h"
#include <cassert>
#include <algorithm>
#include <map>
#include <sstream>

namespace com {
	static_assert(sizeof(J) >= sizeof(IDispatch*), "Insufficient data size: J vs IDispatch*");
	std::map<J, IDispatch*> INSTANCES;
}//namespace com

bool com::InitInstances() {
	return INSTANCES.empty();
}

bool com::UninitInstances() {
	for (auto p = INSTANCES.begin(); p != INSTANCES.end(); ++p) {
		COM_ptr<::IDispatch> dispatch(p->second);
		p->second = NULL;
		dispatch.reset(NULL);	//release COM object
	}
	INSTANCES.clear();
	return INSTANCES.empty();
}

IDispatch* getDispatch(K handle, J* id) throw(std::runtime_error) {
	long long h = q::q2Dec(handle);
	auto p = com::INSTANCES.find(static_cast<J>(h));
	if (p == com::INSTANCES.end()) {
		std::ostringstream buffer;
		buffer << "IDispatch of ID=0x" << util::hexBytes(h) << " not found";
		throw std::runtime_error(buffer.str());
	}
	if (id != NULL) *id = p->first;
	return p->second;
}

WIN32_COM_API K K_DECL createCOM(K progId, K serverType) {
	try{
		std::wstring const pid = q::q2WString(progId);
		long long const context = q::q2Dec(serverType);

		if ((context < std::numeric_limits<DWORD>::min()) || (std::numeric_limits<DWORD>::max() < context))
			throw std::runtime_error("invalid serverType");
		DWORD const ctx = static_cast<DWORD>(context);

		com::COM_ptr<::IDispatch> dispatch(com::CreateCOM(pid.c_str(), ctx));
		assert(dispatch.isValid());
		
		IDispatch* d = dispatch.get();
		J const result = *reinterpret_cast<I*>(&d);
		com::INSTANCES[result] = dispatch.release();
		return kj(result);
	}
	catch (std::runtime_error& ex) {
		return q::error2q(ex.what());
	}
}

WIN32_COM_API K K_DECL releaseCOM(K handle) {
	try {
		J id = 0;
		com::COM_ptr<::IDispatch> dispatch(getDispatch(handle, &id));
		size_t const count = com::INSTANCES.erase(id);
		assert(1 == count);
		dispatch.reset(NULL);	//release COM object
	}
	catch (std::runtime_error& ex) {
		return q::error2q(ex.what());
	}
	return K_NIL;
}

size_t parseArgs(K values, std::vector<::VARIANT>& args) throw(std::runtime_error) {
#	define CONVERT_LIST(atom, index) {\
		assert(values->n >= 0);	\
		q::K_ptr k;	\
		for (size_t i = 0; i < values->n; ++i) {	\
			k.reset(atom(index(values)[i]));	\
			args.push_back(q::q2Variant(k.get()));	\
		}	\
	}

	std::ostringstream buffer;
	if (values == K_NIL) {
		throw std::runtime_error("nil arguments");
	}
	else if (values->t < 0) {
		throw std::runtime_error("atomic argument list");
	}
	else switch (values->t) {
	case 0:
		assert(values->n >= 0);
		for (size_t i = 0; i < values->n; ++i) {
			args.push_back(q::q2Variant(kK(values)[i]));
		}
		break;
	case KB:
		CONVERT_LIST(kb, kG);
		break;
	case KG:
		CONVERT_LIST(kg, kG);
		break;
	case KH:
		CONVERT_LIST(kh, kH);
		break;
	case KI:
		CONVERT_LIST(ki, kI);
		break;
	case KJ:
		CONVERT_LIST(kj, kJ);
		break;
	case KE:
		CONVERT_LIST(ke, kE);
		break;
	case KF:
		CONVERT_LIST(kf, kF);
		break;
	case KC:
		CONVERT_LIST(kc, kC);
		break;
	case KS:
		CONVERT_LIST(ks, kS);
		break;
	case KD:
		CONVERT_LIST(kd, kI);
		break;
	case KZ:
		CONVERT_LIST(kz, kF);
		break;
	default:
		buffer << "unsupported argument list type: " << values->t;
		throw std::runtime_error(buffer.str());
	}
	return args.size();

#	undef CONVERT_LIST
}

WIN32_COM_API K K_DECL invokeCOM(K handle, K getOrPut, K property, K values) {
	try {
		J id = 0;
		::IDispatch* dispatch = getDispatch(handle, &id);
		if (com::INSTANCES.find(id) == com::INSTANCES.end()) {
			throw std::runtime_error("unrecognized COM handle");
		}
		assert(NULL != dispatch);

		long long const flag = q::q2Dec(getOrPut);
		if ((flag < std::numeric_limits<WORD>::min()) || (std::numeric_limits<WORD>::max() < flag)) {
			throw std::runtime_error("invalid getOrPut flag");
		}
		WORD const context = static_cast<WORD>(flag);

		std::wstring const propName = q::q2WString(property);

		std::vector<::VARIANT> args;
		try {
			parseArgs(values, args);
		}
		catch (std::runtime_error& ex) {
			std::for_each(args.begin(), args.end(), [](::VARIANT& v){ ::VariantClear(&v); });
			args.clear();
			throw ex;
		}

		::VARIANT result = com::COMCall(context, dispatch, propName.c_str(), args);
		if (result.vt == VT_DISPATCH) {
			com::COM_ptr<::IDispatch> dispatch(result.pdispVal);
			assert(dispatch.isValid());
			IDispatch* d = dispatch.get();
			J const res = *reinterpret_cast<I*>(&d);
			com::INSTANCES[res] = dispatch.release();
			return kj(res);
		}
		else try {
			return q::Variant2q(result);
		}
		catch (std::runtime_error& ex) {
			::VariantClear(&result);
			throw ex;
		}
		::VariantClear(&result);
	}
	catch (std::runtime_error& ex) {
		return q::error2q(ex.what());
	}
}