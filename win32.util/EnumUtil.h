#ifndef __ENUMUTIL_H__
#define __ENUMUTIL_H__

#include <map>
#include <string>
#include <vector>

// Mapping between enum and string
//@ref http://www.codeproject.com/Articles/42035/Enum-to-String-and-Vice-Versa-in-C


#define BEGIN_ENUM_STRING(E)	\
	template<>	\
	struct ::enum_util::Enum<E> : public ::enum_util::EnumBase<Enum<E>, E> {	\
		static void registerAll()
//		{
#define ENUM_STRING(e)	\
			registerEnumString(e, #e)
//			;
//		}
#define END_ENUM_STRING	\
	}


namespace enum_util {

	template <typename C, typename E>
	class EnumBase {
	public:
		static std::vector<std::string> getAllStrings();

		static std::string const& toString(E const e);

		static E fromString(char const* str);
		static E fromString(std::string const& str) { return fromString(str.c_str()); }

		static bool isValidString(char const* str);
		static bool isValidString(std::string const& str) { return isValidString(str.c_str()); }

	protected:
		typedef std::map<std::string, E> str2enum;

		explicit EnumBase() {}
		virtual ~EnumBase() {}

		static void registerEnumString(E const e, char const* str);

	private:
		static str2enum& getMap();

		// Disabled operations
		EnumBase(EnumBase const&);
		EnumBase const& operator=(EnumBase const&);
	};

	template <class E>
	struct Enum : public EnumBase<Enum<E>, E> {
		static void registerAll();
	};

}//namespace enum_util


#include "EnumUtil.inl"

#endif//__ENUMUTIL_H__