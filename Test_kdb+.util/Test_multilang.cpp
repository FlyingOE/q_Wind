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

		TEST_METHOD(canConvertUnicodeStringSpecial)
		{
			Assert::AreEqual(std::wstring(L"GFTZ-NAS/持仓收益率&实时估值_new.xlsb"),
				ml::convert(ml::CP_GB18030, "GFTZ-NAS/\xB3\xD6\xB2\xD6\xCA\xD5\xD2\xE6\xC2\xCA&\xCA\xB5\xCA\xB1\xB9\xC0\xD6\xB5_new.xlsb"),
				L"GB18030 -> UTF-16 \"GFTZ-NAS/持仓收益率&实时估值_new.xlsb\"", LINE_INFO());

			Assert::AreEqual(std::string("GFTZ-NAS/\xB3\xD6\xB2\xD6\xCA\xD5\xD2\xE6\xC2\xCA&\xCA\xB5\xCA\xB1\xB9\xC0\xD6\xB5_new.xlsb"),
				ml::convert(ml::CP_GB18030, L"GFTZ-NAS/持仓收益率&实时估值_new.xlsb"),
				L"GB18030 <- UTF-16 \"GFTZ-NAS/持仓收益率&实时估值_new.xlsb\"", LINE_INFO());
		}

		TEST_METHOD(canConvertMultilangStringSpecial)
		{
			Assert::AreEqual(std::string("GFTZ-NAS/\xE6\x8C\x81\xE4\xBB\x93\xE6\x94\xB6\xE7\x9B\x8A\xE7\x8E\x87&\xE5\xAE\x9E\xE6\x97\xB6\xE4\xBC\xB0\xE5\x80\xBC_new.xlsb"),
				ml::convert(ml::CP_GB18030, CP_UTF8, "GFTZ-NAS/\xB3\xD6\xB2\xD6\xCA\xD5\xD2\xE6\xC2\xCA&\xCA\xB5\xCA\xB1\xB9\xC0\xD6\xB5_new.xlsb"),
				L"GB18030 -> UTF8 \"GFTZ-NAS/持仓收益率&实时估值_new.xlsb\"", LINE_INFO());

			Assert::AreEqual(std::string("GFTZ-NAS/\xB3\xD6\xB2\xD6\xCA\xD5\xD2\xE6\xC2\xCA&\xCA\xB5\xCA\xB1\xB9\xC0\xD6\xB5_new.xlsb"),
				ml::convert(CP_UTF8, ml::CP_GB18030, "GFTZ-NAS/\xE6\x8C\x81\xE4\xBB\x93\xE6\x94\xB6\xE7\x9B\x8A\xE7\x8E\x87&\xE5\xAE\x9E\xE6\x97\xB6\xE4\xBC\xB0\xE5\x80\xBC_new.xlsb"),
				L"GB18030 <- UTF8 \"GFTZ-NAS/持仓收益率&实时估值_new.xlsb\"", LINE_INFO());
		}

		TEST_METHOD(cannotConvertInvalidMultilangString)
		{
			UINT codepage = CP_UTF8;
			std::string data = "GFTZ-NAS/\xE6\x8C\x81\xE4\xBB\x93\xE6\x94\xB6\xE7\x8E\x3F\xE5\xAE\x9E\xE6\x97\xB6\xE4\xBC\xB0\xE5\x80\xBC_new.xlsb";
			auto const tester = [&]{ return ml::convert(codepage, data.c_str()); };

			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on invalid UTF-8 codepoint", LINE_INFO());

			codepage = ml::CP_GB18030;
			data = "GFTZ-NAS/\xE6\x8C\x81\xE4\xBB\x93\xE6\x94\xB6\xE7\x9B\x8A\xE7\x7E\x87&\xE5\xAE\x9E\xE6\x97\xB6\xE4\xBC\xB0\xE5\x80\xBC_new.xlsb";
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on invalid GB18030 codepoint", LINE_INFO());
		}

	};
}