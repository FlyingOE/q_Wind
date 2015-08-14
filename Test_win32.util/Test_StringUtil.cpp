#include "stdafx.h"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "win32.util/StringUtil.h"

namespace Test_util
{
	TEST_CLASS(Test_StringUtil)
	{
	public:
		
		TEST_METHOD(canReplaceAll)
		{
			Assert::AreEqual<std::string>("20150814", util::replaceAll(std::string("2015-08-14"), "-", ""),
				L"normal string replacement", LINE_INFO());
			Assert::AreEqual<std::wstring>(L"20150814", util::replaceAll(std::wstring(L"2015-08-14"), L"-", L""),
				L"normal wstring replacement", LINE_INFO());

			Assert::AreEqual<std::string>("2015-08-14", util::replaceAll(std::string("2015-08-14"), "-", "-"),
				L"identity string replacement", LINE_INFO());
			Assert::AreEqual<std::wstring>(L"2015-08-14", util::replaceAll(std::wstring(L"2015-08-14"), L"-", L"-"),
				L"identity wstring replacement", LINE_INFO());
		}

	};
}