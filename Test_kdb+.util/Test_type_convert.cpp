#include "stdafx.h"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "kdb+.util/type_convert.h"

#include "kdb+.util/K_ptr.h"

namespace Test_q
{		
	TEST_CLASS(TestQ2Cpp)
	{
	public:

		TEST_METHOD(canConvertToDecimal)
		{
			q::K_ptr data;

			data.reset(kb(true));
			Assert::AreEqual(1LL, q::q2Dec(data.get()),
				L"q)1b", LINE_INFO());

			data.reset(kg(0xF3));
			Assert::AreEqual(243LL, q::q2Dec(data.get()),
				L"q)0xF3", LINE_INFO());

			data.reset(kh(27856));
			Assert::AreEqual(27856LL, q::q2Dec(data.get()),
				L"q)27856h", LINE_INFO());

			data.reset(ki(476883888));
			Assert::AreEqual(476883888LL, q::q2Dec(data.get()),
				L"q)476883888i", LINE_INFO());

			data.reset(kj(7413760571230203088L));
			Assert::AreEqual(7413760571230203088LL, q::q2Dec(data.get()),
				L"q)7413760571230203088j", LINE_INFO());
		}

	};
}

/////////////////////////////////////////////////////////////////////////////

#include <sstream>

namespace Microsoft{
	namespace VisualStudio {
		namespace CppUnitTestFramework {

			template<>
			static std::wstring ToString<long long>(long long const& value) {
				std::wostringstream buffer;
				buffer << value;
				return buffer.str();
			}

		}
	}
}//namespace Microsoft
