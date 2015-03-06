#ifndef __MATRIXDATAPARSER_H__
#define __MATRIXDATAPARSER_H__

namespace Wind {

	class MatrixDataParser {
	public:
		explicit MatrixDataParser(::VARIANT const& array);
		K parse() const throw();

	protected:
		template <typename T>
		class SafeArrayAccessor;
		
		template <typename T>
		struct qTypeTraits;

		template <typename T, typename Traits = qTypeTraits<T> >
		static K parseSafeArray(::SAFEARRAY& array) throw();

	private:
		MatrixDataParser(MatrixDataParser const&);	// explicitly disabled

		::VARIANT const& data_;
	};

}//namespace Wind

#include "MatrixDataParser.inl"

#endif//__MATRIXDATAPARSER_H__