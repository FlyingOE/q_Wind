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

		TEST_METHOD(canSplitStrings)
		{
			std::vector<std::string> expected, actual;
			expected = { "Hello", "world!", "", "Next", "" };
			actual = util::split("Hello world!  Next ", ' ');
			Assert::IsTrue(std::equal(actual.cbegin(), actual.cend(), expected.cbegin()),
				L"ASCII string split by space", LINE_INFO());

			expected = { "username", "Password!", "group", "", "..." };
			actual = util::split("username:Password!:group::...", ':');
			Assert::IsTrue(std::equal(actual.cbegin(), actual.cend(), expected.cbegin()),
				L"ASCII string split by normal character", LINE_INFO());

			expected = { "No_delim" };
			actual = util::split("No_delim", ':');
			Assert::IsTrue(std::equal(actual.cbegin(), actual.cend(), expected.cbegin()),
				L"ASCII single-token string split", LINE_INFO());

			expected = {};
			actual = util::split("", ':');
			Assert::IsTrue(std::equal(actual.cbegin(), actual.cend(), expected.cbegin()),
				L"ASCII empty string split", LINE_INFO());
		}

		TEST_METHOD(canSplitWStrings)
		{
			std::vector<std::wstring> expected, actual;
			expected = { L"Hello", L"world!", L"", L"≤‚ ‘", L"" };
			actual = util::split(L"Hello world!  ≤‚ ‘ ", L' ');
			Assert::IsTrue(std::equal(actual.cbegin(), actual.cend(), expected.cbegin()),
				L"Unicode string split by space", LINE_INFO());

			expected = { L"username", L"Password!", L"group", L"", L"°≠°≠" };
			actual = util::split(L"username£∫Password!£∫group£∫£∫°≠°≠", L'£∫');
			Assert::IsTrue(std::equal(actual.cbegin(), actual.cend(), expected.cbegin()),
				L"Unicode string split by Unicode character", LINE_INFO());

			expected = { L"No_delim≤‚ ‘" };
			actual = util::split(L"No_delim≤‚ ‘", L'£∫');
			Assert::IsTrue(std::equal(actual.cbegin(), actual.cend(), expected.cbegin()),
				L"Unicode single-token string split", LINE_INFO());

			expected = {};
			actual = util::split(L"", L'£∫');
			Assert::IsTrue(std::equal(actual.cbegin(), actual.cend(), expected.cbegin()),
				L"Unicode empty string split", LINE_INFO());
		}

		TEST_METHOD(canChooseLiteral)
		{
			Assert::AreEqual('A', LITERAL(char, 'A'),
				L"literal narrow character", LINE_INFO());
			Assert::AreEqual("Hello world!", LITERAL(char, "Hello world!"),
				L"literal narrow string", LINE_INFO());

			Assert::AreEqual(L'A', LITERAL(wchar_t, 'A'),
				L"literal wide character", LINE_INFO());
			Assert::AreEqual(L"Hello world!", LITERAL(wchar_t, "Hello world!"),
				L"literal wide string", LINE_INFO());
		}

	};
}