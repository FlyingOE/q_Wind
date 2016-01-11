#ifndef __FIELDMAPPER_H__
#define __FIELDMAPPER_H__

#include "FieldAccessors.h"

#include <string>
#include <vector>
#include <map>

namespace Wind {
	namespace mapper {

		template <typename T>
		class Fields {
		public:
			typedef Wind::accessor::FieldAccessor<T> field_accessor;
			typedef std::map<std::string, std::unique_ptr<field_accessor> > map_type;
			typedef std::multimap<std::string, std::string> catalog_type;

		public:
			K getFields() const;
			K getFields(char const* category) const;
			K getFields(std::string const& category) const;

			void getFields(std::vector<std::string>& list) const;
			void getFields(char const* category, std::vector<std::string>& list) const;
			void getFields(std::string const& category, std::vector<std::string>& list) const;

			void addField(char const* fieldName, field_accessor* accessor) throw(std::string);
			void addField(std::string const& fieldName, field_accessor* accessor) throw(std::string);

			void addField(char const* category, char const* fieldName, field_accessor* accessor) throw(std::string);
			void addField(std::string const& category, std::string const& fieldName, field_accessor* accessor) throw(std::string);

			void getCategories(std::vector<std::string>& list) const;

			bool hasCategory(char const* category) const;
			bool hasCategory(std::string const& category) const;

			field_accessor const* operator[](char const* fieldName) throw(std::string);
			field_accessor const* operator[](std::string const& fieldName) throw(std::string);

		private:
			map_type fields_;
			catalog_type catalog_;
		};

	}//namespace Wind::mapper
}//namespace Wind

#include "FieldMapper.inl"

#endif//__FIELDMAPPER_H__