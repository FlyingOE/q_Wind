#ifndef __FIELDMAPPER_H__
#define __FIELDMAPPER_H__

#include "FieldAccessors.h"

#include <string>
#include <map>

namespace Wind {
	namespace mapper {

		template <typename T>
		class Fields {
		public:
			typedef Wind::accessor::FieldAccessor<T> field_accessor;
			typedef std::map<std::string, std::unique_ptr<field_accessor> > map_type;

		public:
			K getFields() const;

			void addField(char const* fieldName, field_accessor* accessor) throw(std::string);
			void addField(std::string const& fieldName, field_accessor* accessor) throw(std::string);

			field_accessor const* operator[](char const* fieldName) throw(std::string);
			field_accessor const* operator[](std::string const& fieldName) throw(std::string);

		private:
			map_type fields_;
		};

	}//namespace Wind::mapper
}//namespace Wind

#include "FieldMapper.inl"

#endif//__FIELDMAPPER_H__