#include "stdafx.h"
#include "MatrixDataParser.h"

#include "win32.util/util.h"


Wind::MatrixDataParser::MatrixDataParser(::VARIANT const& array) : data_(array) {
}

//@ref struct Wind::MatrixDataParser::qTypeTraits<::VARIANT>
K Wind::MatrixDataParser::parse() const throw() {
	if ((data_.vt & ::VT_ARRAY) != ::VT_ARRAY) {
		return q::error2q("Wind MatrixData is not an array");
	}

	::SAFEARRAY* array = data_.parray;
	if (array == NULL) {
		return q::error2q("Wind MatrixData contains a NULL array");
	}
	
	switch (data_.vt ^ ::VT_ARRAY) {
	case ::VT_I2:
		return parseSafeArray<H>(*array);
	case ::VT_I4:
	case ::VT_UI2:
		return parseSafeArray<I>(*array);
	case ::VT_I8:
	case ::VT_UI4:
	case ::VT_UI8:
		return parseSafeArray<J>(*array);
	case ::VT_R4:
		return parseSafeArray<E>(*array);
	case ::VT_R8:
		return parseSafeArray<F>(*array);
	case ::VT_DATE: {
			q::K_ptr result(parseSafeArray<DATE>(*array));
			return util::qConvertArray3D<q::type_traits<F>, util::qDateTypeTraits>(result.get());
		}
	case ::VT_BSTR:
		return parseSafeArray<::BSTR>(*array);
	case ::VT_VARIANT:
		return parseSafeArray<::VARIANT>(*array);
	default: {
			std::ostringstream buffer;
			buffer << "unsupported VARIANT SAFEARRAY type: 0x" << ::util::hexBytes(data_.vt);
			return q::error2q(buffer.str());
		}
	}
}