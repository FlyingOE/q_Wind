#include "stdafx.h"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "win32.util/util.h"

#include <sstream>

#include <iostream>
namespace Test_util
{		
	TEST_CLASS(TestHexBytes)
	{
	public:

		TEST_METHOD(canPrintUnsignedBytes) { printBytes<unsigned char>(); }

		TEST_METHOD(canPrintBytes) { printBytes<char>(); }

		TEST_METHOD(canPrintPointer) {
			int* const p = reinterpret_cast<int*>(69957480);
			std::ostringstream buffer;
			buffer << util::hexBytes(p);
			Assert::AreEqual("042B7768", buffer.str().c_str(), true,
				NULL, LINE_INFO());
		}

		template <typename Byte>
		void printBytes() {
			Byte const bytes[] = "\x00\x01\x02 0123456789ABCdef;/\xFE\xFF";

			std::ostringstream buffer;
			buffer << util::hexBytes(bytes, _countof(bytes) - 1);
			Assert::AreEqual("00010220303132333435363738394142436465663B2FFEFF", buffer.str().c_str(), true,
				L"normal byte sequence", LINE_INFO());

			buffer.clear();	buffer.str("");
			buffer << util::hexBytes(bytes, _countof(bytes) - 1, true);
			Assert::AreEqual("FFFE2F3B6665644342413938373635343332313020020100", buffer.str().c_str(), true,
				L"reversed byte sequence", LINE_INFO());
		}

		TEST_METHOD(canPrintBinaryDump) {
			int64_t const data = -9111249659239182176L;
			std::ostringstream buffer;
			buffer << util::hexBytes(data);
			Assert::AreEqual("818E56B5592650A0", buffer.str().c_str(), true,
				NULL, LINE_INFO());
		}
	};
}