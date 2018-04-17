#include "stdafx.h"
#include "CppUnitTest.h"

#include <cstdlib>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test_Wind
{
	TEST_CLASS(Test_Wind_API)
	{
	public:
		
		TEST_METHOD(canRunTestScript)
		{
			char const* CMD = "%QHOME%\\"
#				if _WIN64
					"w64"
#				else
					"w32"
#				endif
					"\\q.exe Test_Wind.q -s 4 -q > Test_Wind.q.log 2>&1";
			Assert::AreEqual(0, std::system(CMD), L"Failed to run q test script! Check Test_Wind.q.log.");
		}

	};
}