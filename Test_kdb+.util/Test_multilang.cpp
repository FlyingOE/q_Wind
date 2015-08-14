#include "stdafx.h"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "kdb+.util/type_convert.h"

namespace Test_ml
{
	//@ref http://chars.suikawiki.org/string?s=
	TEST_CLASS(Test_multilang)
	{
	public:

		TEST_METHOD(canConvertUnicodeString)
		{
			Assert::AreEqual(std::wstring(L"CapitalEdge 国富投资"),
				ml::convert(ml::CP_GB18030, "CapitalEdge \xB9\xFA\xB8\xBB\xCD\xB6\xD7\xCA"),
				L"GB18030 -> UTF-16 \"CapitalEdge 国富投资\"", LINE_INFO());
			Assert::AreEqual(std::wstring(L"CapitalEdge 国富投资"),
				ml::convert(CP_UTF8, "CapitalEdge \xE5\x9B\xBD\xE5\xAF\x8C\xE6\x8A\x95\xE8\xB5\x84"),
				L"UTF-8 -> UTF-16 \"CapitalEdge 国富投资\"", LINE_INFO());

			Assert::AreEqual(std::string("CapitalEdge \xB9\xFA\xB8\xBB\xCD\xB6\xD7\xCA"),
				ml::convert(ml::CP_GB18030, L"CapitalEdge 国富投资"),
				L"GB18030 <- UTF-16 \"CapitalEdge 国富投资\"", LINE_INFO());
			Assert::AreEqual(std::string("CapitalEdge \xE5\x9B\xBD\xE5\xAF\x8C\xE6\x8A\x95\xE8\xB5\x84"),
				ml::convert(CP_UTF8, L"CapitalEdge 国富投资"),
				L"UTF-8 <- UTF-16 \"CapitalEdge 国富投资\"", LINE_INFO());
		}


		TEST_METHOD(canConvertMultilangString)
		{
			Assert::AreEqual(std::string("CapitalEdge \xE5\x9B\xBD\xE5\xAF\x8C\xE6\x8A\x95\xE8\xB5\x84"),
				ml::convert(ml::CP_GB18030, CP_UTF8, "CapitalEdge \xB9\xFA\xB8\xBB\xCD\xB6\xD7\xCA"),
				L"GB18030 -> UTF8 \"CapitalEdge 国富投资\"", LINE_INFO());

			Assert::AreEqual(std::string("CapitalEdge \xB9\xFA\xB8\xBB\xCD\xB6\xD7\xCA"),
				ml::convert(CP_UTF8, ml::CP_GB18030, "CapitalEdge \xE5\x9B\xBD\xE5\xAF\x8C\xE6\x8A\x95\xE8\xB5\x84"),
				L"GB18030 <- UTF8 \"CapitalEdge 国富投资\"", LINE_INFO());
		}

	};
}