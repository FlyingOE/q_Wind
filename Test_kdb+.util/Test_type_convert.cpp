#include "stdafx.h"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "kdb+.util/type_convert.h"

#include "kdb+.util/K_ptr.h"
#include "win32.util/CodeConvert.h"
#include "win32.util/hexDump.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace q {
	namespace test {
		bool VariantEqual(::VARIANT const& a, ::VARIANT const& b);
	}
}

namespace Test_q
{		
	TEST_CLASS(Test_q2cpp)
	{
	public:

		TEST_METHOD(canConvertToDecimal)
		{
			testNumberConvert<long long>(&q::q2Dec);

			q::K_ptr data;

			data.reset(kh(nh));
			Assert::AreEqual<long long>(nh, q::q2Dec(data.get()),
				L"q)0Nh", LINE_INFO());
			data.reset(kh(wh));
			Assert::AreEqual<long long>(wh, q::q2Dec(data.get()),
				L"q)0Wh", LINE_INFO());

			data.reset(ki(ni));
			Assert::AreEqual<long long>(ni, q::q2Dec(data.get()),
				L"q)0Ni", LINE_INFO());
			data.reset(ki(wi));
			Assert::AreEqual<long long>(wi, q::q2Dec(data.get()),
				L"q)0Wi", LINE_INFO());

			data.reset(kj(nj));
			Assert::AreEqual<long long>(nj, q::q2Dec(data.get()),
				L"q)0Nj", LINE_INFO());
			data.reset(kj(wj));
			Assert::AreEqual<long long>(wj, q::q2Dec(data.get()),
				L"q)0Wj", LINE_INFO());
		}

		TEST_METHOD(canConvertToFloat)
		{
			testNumberConvert<double>(&q::q2Fp);

			q::K_ptr data;

			data.reset(ke(967.23981));
			Assert::AreEqual(static_cast<double>(967.23981f), q::q2Fp(data.get()), 0.,
				L"q)967.23981e", LINE_INFO());
			data.reset(ke(nf));
			Assert::IsTrue(std::isnan(q::q2Fp(data.get())),
				L"q)0Ne", LINE_INFO());
			data.reset(ke(wf));
			Assert::IsTrue(std::isinf(q::q2Fp(data.get())),
				L"q)0We", LINE_INFO());

			data.reset(kf(7413760.571230203088));
			Assert::AreEqual(7413760.571230203088, q::q2Fp(data.get()), 0.,
				L"q)7413760.571230203088f", LINE_INFO());
			data.reset(kf(nf));
			Assert::IsTrue(std::isnan(q::q2Fp(data.get())),
				L"q)0n", LINE_INFO());
			data.reset(kf(wf));
			Assert::IsTrue(std::isinf(q::q2Fp(data.get())),
				L"q)0w", LINE_INFO());
		}
		
		template <typename T, typename Func>
		void testNumberConvert(Func const& func) {
			q::K_ptr data;

#			define NUMCONV_TESTER(val, msg, line)	\
			Assert::IsTrue((std::numeric_limits<T>::min() <= (val)) && ((val) <= std::numeric_limits<T>::max()));	\
			Assert::AreEqual(static_cast<T>((val)), func(data.get()), (msg), (line))

			data.reset(kb(true));
			NUMCONV_TESTER(1LL,
				L"q)1b", LINE_INFO());

			data.reset(kg(0xF3));
			NUMCONV_TESTER(243LL,
				L"q)0xF3", LINE_INFO());

			data.reset(kh(27856));
			NUMCONV_TESTER(27856LL,
				L"q)27856h", LINE_INFO());

			data.reset(ki(476883888));
			NUMCONV_TESTER(476883888LL,
				L"q)476883888i", LINE_INFO());

			data.reset(kj(7413760571230203088L));
			NUMCONV_TESTER(7413760571230203088LL,
				L"q)7413760571230203088j", LINE_INFO());


#			undef NUMCONV_TESTER
		}

		TEST_METHOD(cannotConvertNonDecimal) {
			q::K_ptr data;
			auto const tester = [&]{ q::q2Dec(data.get()); };

			data.reset(kf(47.68839));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on float", LINE_INFO());

			data.reset(ktn(KJ, 0));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on list", LINE_INFO());
		}

		TEST_METHOD(cannotConvertNonFloat) {
			q::K_ptr data;
			auto const tester = [&]{ q::q2Fp(data.get()); };

			data.reset(ktj(-KP, 4768839));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on timestamp", LINE_INFO());

			data.reset(ktn(KF, 0));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on list", LINE_INFO());
		}

		TEST_METHOD(canConvertToDecimalList)
		{
			testNumberListConvert<long long>(&q::qList2Dec);
		}

		TEST_METHOD(canConvertToFloatList)
		{
			testNumberListConvert<double>(&q::qList2Fp);

			q::K_ptr data;
			std::vector<double> expected(3);

			data.reset(ktn(KE, 3));
			kE(data.get())[0] = 967.23981f ;	expected[0] = 967.23981f ;
			kE(data.get())[1] = 391.54297f ;	expected[1] = 391.54297f ;
			kE(data.get())[2] = -81.235458f;	expected[2] = -81.235458f;
			Assert::AreEqual(expected, q::qList2Fp(data.get()),
				L"q)967.23981 391.54297 -81.235458e", LINE_INFO());

			data.reset(ktn(KF, 3));
			kF(data.get())[0] =  7413760.571230203088;	expected[0] =  7413760.571230203088;
			kF(data.get())[1] =  4099561.0598474741;	expected[1] =  4099561.0598474741;
			kF(data.get())[2] = -6108817.2462768853;	expected[2] = -6108817.2462768853;
			Assert::AreEqual(expected, q::qList2Fp(data.get()),
				L"q)7413760.571230203088 4099561.0598474741 -6108817.2462768853f", LINE_INFO());
		}

		template <typename T, typename Func>
		void testNumberListConvert(Func const& func) {
			q::K_ptr data;
			std::vector<T> expected(2);

			data.reset(ktn(KB, 2));
			kG(data.get())[0] = true ;	expected[0] = static_cast<T>(1LL);
			kG(data.get())[1] = false;	expected[1] = static_cast<T>(0LL);
			Assert::AreEqual(expected, func(data.get()),
				L"q)101b", LINE_INFO());

			expected.resize(3);

			data.reset(ktn(KG, 3));
			kG(data.get())[0] = 0xF3;	expected[0] = static_cast<T>(243LL);
			kG(data.get())[1] = 0x00;	expected[1] = static_cast<T>(  0LL);
			kG(data.get())[2] = 0x16;	expected[2] = static_cast<T>( 22LL);
			Assert::AreEqual(expected, func(data.get()),
				L"q)0xF30016", LINE_INFO());

			data.reset(ktn(KH, 3));
			kH(data.get())[0] =  27856;	expected[0] = static_cast<T>( 27856LL);
			kH(data.get())[1] =      0;	expected[1] = static_cast<T>(     0LL);
			kH(data.get())[2] = -14733;	expected[2] = static_cast<T>(-14733LL);
			Assert::AreEqual(expected, func(data.get()),
				L"q)27856 0 -14733h", LINE_INFO());

			data.reset(ktn(KI, 3));
			kI(data.get())[0] =  476883888;	expected[0] = static_cast<T>( 476883888LL);
			kI(data.get())[1] =          0;	expected[1] = static_cast<T>(         0LL);
			kI(data.get())[2] = -773686186;	expected[2] = static_cast<T>(-773686186LL);
			Assert::AreEqual(expected, func(data.get()),
				L"q)476883888 0 -773686186i", LINE_INFO());

			data.reset(ktn(KJ, 3));
			kJ(data.get())[0] = 7413760571230203088L;	expected[0] = static_cast<T>(7413760571230203088LL);
			kJ(data.get())[1] =                   0L;	expected[1] = static_cast<T>(                  0LL);
			kJ(data.get())[2] =     -26995309665792L;	expected[2] = static_cast<T>(    -26995309665792LL);
			Assert::AreEqual(expected, func(data.get()),
				L"q)7413760571230203088 0 26995309665792j", LINE_INFO());
		}

		TEST_METHOD(cannotConvertNonDecimalList) {
			q::K_ptr data;
			auto const tester = [&]{ q::qList2Dec(data.get()); };

			data.reset(ki(4768839));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on atom", LINE_INFO());

			data.reset(ktn(KF, 1));
			kF(data.get())[0] = 47.68839;
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on float list", LINE_INFO());
		}

		TEST_METHOD(cannotConvertNonFloatList) {
			q::K_ptr data;
			auto const tester = [&]{ q::qList2Fp(data.get()); };

			data.reset(kf(4768.839));
			Assert::ExpectException<std::runtime_error>(tester, L"fail on atom", LINE_INFO());

			data.reset(ktn(KC, 1));
			kC(data.get())[0] = 'A';
			Assert::ExpectException<std::runtime_error>(tester, L"fail on char list", LINE_INFO());
		}

		TEST_METHOD(canConvertString)
		{
			q::K_ptr data;
			char const *testee = "Hello world!";

			data.reset(ks(const_cast<S>(testee)));
			Assert::AreEqual(testee, q::q2String(data.get()).c_str(),
				L"q)`$\"...\"", LINE_INFO());

			data.reset(ks(""));
			Assert::AreEqual("", q::q2String(data.get()).c_str(),
				L"q)`", LINE_INFO());

			data.reset(kp(const_cast<S>(testee)));
			Assert::AreEqual(testee, q::q2String(data.get()).c_str(),
				L"q)\"...\"", LINE_INFO());

			data.reset(kp(""));
			Assert::AreEqual("", q::q2String(data.get()).c_str(),
				L"q)\"\"", LINE_INFO());
		}


		TEST_METHOD(cannotConvertNonString) {
			q::K_ptr data;
			auto const tester = [&]{ q::q2String(data.get()); };

			data.reset(kf(4768.839));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on number", LINE_INFO());

			data.reset(kc('A'));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on char", LINE_INFO());

			data.reset(ktn(KS, 1));
			kS(data.get())[0] = ss("`symbol");
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on symbol list", LINE_INFO());

			data.reset(ktn(0, 1));
			kK(data.get())[0] = kp("\"char list\"");
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on char-list list", LINE_INFO());
		}

		TEST_METHOD(canConvertStringList)
		{
			q::K_ptr data;
			std::vector<std::string> expected(3);
			expected[0] = "Hello world!";
			expected[1] = "";
			expected[2] = "\x00\x01\xFE\xFF";

			data.reset(ktn(KS, expected.size()));
			for (size_t i = 0; i < expected.size(); ++i) {
				kS(data.get())[i] = ss(const_cast<S>(expected[i].c_str()));
			}
			Assert::AreEqual(expected, q::qList2String(data.get()),
				L"q)(`$\"...\";`;`$\"c\"$0x...)", LINE_INFO());

			data.reset(ktn(KS, 0));
			Assert::AreEqual(std::vector<std::string>(), q::qList2String(data.get()),
				L"q)`$()", LINE_INFO());

			data.reset(ktn(0, expected.size()));
			for (size_t i = 0; i < expected.size(); ++i) {
				kK(data.get())[i] = kp(const_cast<S>(expected[i].c_str()));
			}
			Assert::AreEqual(expected, q::qList2String(data.get()),
				L"q)(\"...\";\"\";\"c\"$0x...)", LINE_INFO());

			data.reset(ktn(0, 0));
			Assert::AreEqual(std::vector<std::string>(), q::qList2String(data.get()),
				L"q)()", LINE_INFO());
		}

		TEST_METHOD(cannotConvertNonStringList)
		{
			q::K_ptr data;
			auto const tester = [&]{ q::qList2String(data.get()); };

			data.reset(kf(4768.839));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on atom", LINE_INFO());

			data.reset(kp("ABC"));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on char list", LINE_INFO());

			data.reset(ks("ABC"));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on symbol", LINE_INFO());
		}

		TEST_METHOD(canConvertDateTime)
		{
			q::K_ptr data;
			q::tm_ext result;

			data.reset(kd(5703));
			result = q::q2tm(data.get());
			Assert::AreEqual(2015 - 1900, result.tm_year,
				L"q)(2015.08.13).year", LINE_INFO());
			Assert::AreEqual(8 - 1, result.tm_mon,
				L"q)(2015.08.13).month", LINE_INFO());
			Assert::AreEqual(13, result.tm_mday,
				L"q)(2015.08.13).day", LINE_INFO());

			data.reset(kt(40019070));
			result = q::q2tm(data.get());
			Assert::AreEqual(11, result.tm_hour,
				L"q)(11:06:59.070).hh", LINE_INFO());
			Assert::AreEqual(6, result.tm_min,
				L"q)(11:06:59.070).mm", LINE_INFO());
			Assert::AreEqual(59, result.tm_sec,
				L"q)(11:06:59.070).ss", LINE_INFO());
			Assert::AreEqual(70, result.tm_millis,
				L"q)(11:06:59.070).millis", LINE_INFO());

			data.reset(kz(5703.5250254207986));
			result = q::q2tm(data.get());
			Assert::AreEqual(2015 - 1900, result.tm_year,
				L"q)(2015.08.13T12:36:02.196).year", LINE_INFO());
			Assert::AreEqual(8 - 1, result.tm_mon,
				L"q)(2015.08.13T12:36:02.196).month", LINE_INFO());
			Assert::AreEqual(13, result.tm_mday,
				L"q)(2015.08.13T12:36:02.196).day", LINE_INFO());
			Assert::AreEqual(12, result.tm_hour,
				L"q)(2015.08.13T12:36:02.196).hour", LINE_INFO());
			Assert::AreEqual(36, result.tm_min,
				L"q)(2015.08.13T12:36:02.196).minute", LINE_INFO());
			Assert::AreEqual(2, result.tm_sec,
				L"q)(2015.08.13T12:36:02.196).second", LINE_INFO());
			Assert::AreEqual(196, result.tm_millis,
				L"q)(2015.08.13T12:36:02.196).millis", LINE_INFO());
		}

		TEST_METHOD(canConvertZeroDateTime)
		{
			q::K_ptr data;
			q::tm_ext result;

			data.reset(kd(0));
			result = q::q2tm(data.get());
			Assert::AreEqual(2000 - 1900, result.tm_year,
				L"q)(`date$0).year", LINE_INFO());
			Assert::AreEqual(1 - 1, result.tm_mon,
				L"q)(`date$0).month", LINE_INFO());
			Assert::AreEqual(1, result.tm_mday,
				L"q)(`date$0).day", LINE_INFO());

			data.reset(kt(0));
			result = q::q2tm(data.get());
			Assert::AreEqual(0, result.tm_hour,
			L"q)(`time$0).hh", LINE_INFO());
			Assert::AreEqual(0, result.tm_min,
			L"q)(`time$0).mm", LINE_INFO());
			Assert::AreEqual(0, result.tm_sec,
			L"q)(`time$0).ss", LINE_INFO());
			Assert::AreEqual(0, result.tm_millis,
			L"q)(`time$0).millis", LINE_INFO());

			data.reset(kz(0.));
			result = q::q2tm(data.get());
			Assert::AreEqual(2000 - 1900, result.tm_year,
			L"q)(`datetime$0).year", LINE_INFO());
			Assert::AreEqual(1 - 1, result.tm_mon,
			L"q)(`datetime$0).month", LINE_INFO());
			Assert::AreEqual(1, result.tm_mday,
			L"q)(`datetime$0).day", LINE_INFO());
			Assert::AreEqual(0, result.tm_hour,
			L"q)(`datetime$0).hour", LINE_INFO());
			Assert::AreEqual(0, result.tm_min,
			L"q)(`datetime$0).minute", LINE_INFO());
			Assert::AreEqual(0, result.tm_sec,
			L"q)(`datetime$0).second", LINE_INFO());
			Assert::AreEqual(0, result.tm_millis,
			L"q)(`datetime$0).millis", LINE_INFO());
		}

		TEST_METHOD(cannotConvertNonDateTime)
		{
			q::K_ptr data;
			auto const tester = [&]{ q::q2tm(data.get()); };

			data.reset(ki(5703));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on integer", LINE_INFO());

			data.reset(kf(5703.5250254207986));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on floating-point", LINE_INFO());

			data.reset(ktn(KD, 1));
			kI(data.get())[0] = 5703;
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on date list", LINE_INFO());

			data.reset(ktn(KZ, 1));
			kF(data.get())[0] = 5703.5250254207986;
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on datetime list", LINE_INFO());
		}

		TEST_METHOD(cannotConvertInfDateTime)
		{
			q::K_ptr data;
			auto const tester = [&]{ q::q2tm(data.get()); };

			data.reset(kd(wi));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on +0Wd", LINE_INFO());
			data.reset(kd(-wi));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on -0Wd", LINE_INFO());

			data.reset(kt(wi));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on +0Wt", LINE_INFO());
			data.reset(kt(-wi));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on -0Wt", LINE_INFO());

			data.reset(kf(wf));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on +0Wz", LINE_INFO());
			data.reset(kf(-wf));
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on -0Wz", LINE_INFO());

			data.reset(ktn(KD, 1));
			kI(data.get())[0] = wi;
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on (+0Wd)", LINE_INFO());
			data.reset(ktn(KD, 2));
			kI(data.get())[0] = 5703;
			kI(data.get())[1] = -wi;
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on (2015.08.13 -0Wd)", LINE_INFO());

			data.reset(ktn(KT, 1));
			kI(data.get())[0] = wi;
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on (+0Wt)", LINE_INFO());
			data.reset(ktn(KT, 2));
			kI(data.get())[0] = 40019070;
			kI(data.get())[1] = -wi;
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on (11:06:59.070 -0Wt)", LINE_INFO());

			data.reset(ktn(KZ, 1));
			kF(data.get())[0] = wf;
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on (+0Wz)", LINE_INFO());

			data.reset(ktn(KZ, 2));
			kF(data.get())[0] = 5703.5250254207986;
			kF(data.get())[1] = -wf;
			Assert::ExpectException<std::runtime_error>(tester,
				L"fail on (2015.08.13T12:36:02.196 -0Wz)", LINE_INFO());
		}

		TEST_METHOD(canConvertDateString)
		{
			test_q2date(20150813, 5703);
			test_q2date(20000101, 0);
			test_q2date(19000301, -36465);
		}

		void test_q2date(int yyyymmdd, int expected) {
			std::wostringstream msg;
			msg << yyyymmdd << L" => D";
			Assert::AreEqual(expected, q::date2q(yyyymmdd),
				msg.str().c_str(), LINE_INFO());

			msg.clear();	msg.str(L"");
			std::ostringstream buffer;
			msg << L"'.'sv " << yyyymmdd << L" = > D";
			buffer << std::right << std::setfill('0')
				<< std::setw(4) << (yyyymmdd / 10000) << '.'
				<< std::setw(2) << (yyyymmdd / 100 % 100) << '.'
				<< std::setw(2) << (yyyymmdd % 100);
			Assert::AreEqual(expected, q::date2q(buffer.str()),
				msg.str().c_str(), LINE_INFO());

			msg.clear();	msg.str(L"");
			buffer.clear();	buffer.str("");
			msg << L"'-'sv " << yyyymmdd << L" = > D";
			buffer << std::right << std::setfill('0')
				<< std::setw(4) << (yyyymmdd / 10000) << '-'
				<< std::setw(2) << (yyyymmdd / 100 % 100) << '-'
				<< std::setw(2) << (yyyymmdd % 100);
			Assert::AreEqual(expected, q::date2q(buffer.str()),
				msg.str().c_str(), LINE_INFO());

			msg.clear();	msg.str(L"");
			buffer.clear();	buffer.str("");
			msg << L"'/'sv " << yyyymmdd << L" = > D";
			buffer << std::right << std::setfill('0')
				<< std::setw(4) << (yyyymmdd / 10000) << '/'
				<< std::setw(2) << (yyyymmdd / 100 % 100) << '/'
				<< std::setw(2) << (yyyymmdd % 100);
			Assert::AreEqual(expected, q::date2q(buffer.str()),
				msg.str().c_str(), LINE_INFO());
		}

		TEST_METHOD(canConvertTimeString)
		{
			Assert::AreEqual(45362196, q::time2q(123602196),
				L"123602196 => T", LINE_INFO());
			Assert::AreEqual(0, q::time2q(0),
				L"000000000 => T", LINE_INFO());
			Assert::AreEqual(86399999, q::time2q(235959999),
				L"235959999 => T", LINE_INFO());
			Assert::AreEqual(86400000, q::time2q(240000000),
				L"240000000 => T", LINE_INFO());
		}

		TEST_METHOD(canConvertFromDATE)
		{
			double const EPSILON = .001 / 86400L / 2;

			Assert::AreEqual(5703.5250254166667, q::DATE2q(42229.5250254166667), EPSILON,
				L"DATE(2015.08.13T12:36:02.196) => D", LINE_INFO());

			//@see https://msdn.microsoft.com/en-us/library/82ab7w69.aspx
#			pragma region
			Assert::AreEqual(-36529., q::DATE2q(-3.), EPSILON,
				L"DATE(1899.12.27T00:00) => D", LINE_INFO());
			Assert::AreEqual(-36527.5, q::DATE2q(-2.5), EPSILON,
				L"DATE(1899.12.28T12:00) => D", LINE_INFO());
			Assert::AreEqual(-36528., q::DATE2q(-2.), EPSILON,
				L"DATE(1899.12.28T00:00) => D", LINE_INFO());
			Assert::AreEqual(-36527., q::DATE2q(-1.), EPSILON,
				L"DATE(1899.12.29T00:00) => D", LINE_INFO());
			Assert::AreEqual(-36525.25, q::DATE2q(-.75), EPSILON,
				L"DATE(1899.12.30T18:00) => D", LINE_INFO());
			Assert::AreEqual(-36525.5, q::DATE2q(-.5), EPSILON,
				L"DATE(1899.12.30T12:00) => D", LINE_INFO());
			Assert::AreEqual(-36525.75, q::DATE2q(-.25), EPSILON,
				L"DATE(1899.12.30T06:00) => D", LINE_INFO());
			Assert::AreEqual(-36526., q::DATE2q(0.), EPSILON,
				L"DATE(1899.12.30T00:00) => D", LINE_INFO());
			Assert::AreEqual(-36525.75, q::DATE2q(.25), EPSILON,
				L"DATE(1899.12.30T06:00) => D", LINE_INFO());
			Assert::AreEqual(-36525.5, q::DATE2q(.5), EPSILON,
				L"DATE(1899.12.30T12:00) => D", LINE_INFO());
			Assert::AreEqual(-36525.25, q::DATE2q(.75), EPSILON,
				L"DATE(1899.12.30T18:00) => D", LINE_INFO());
			Assert::AreEqual(-36525., q::DATE2q(1.), EPSILON,
				L"DATE(1899.12.31T00:00) => D", LINE_INFO());
			Assert::AreEqual(-36524., q::DATE2q(2.), EPSILON,
				L"DATE(1900.01.01T00:00) => D", LINE_INFO());
			Assert::AreEqual(-36521., q::DATE2q(5.), EPSILON,
				L"DATE(1900.01.04T00:00) => D", LINE_INFO());
			Assert::AreEqual(-36520.75, q::DATE2q(5.25), EPSILON,
				L"DATE(1900.01.04T06:00) => D", LINE_INFO());
			Assert::AreEqual(-36520.5, q::DATE2q(5.5), EPSILON,
				L"DATE(1900.01.04T12:00) => D", LINE_INFO());
			Assert::AreEqual(-36520.125, q::DATE2q(5.875), EPSILON,
				L"DATE(1900.01.04T21:00) => D", LINE_INFO());
#			pragma endregion

			Assert::AreEqual(-10957., q::DATE2q(25569.), EPSILON,
				L"DATE(1970.01.01T00:00) => D", LINE_INFO());
		}

		TEST_METHOD(canConvertToDATE)
		{
			double const EPSILON = .001 / 86400L / 2;
			q::K_ptr d;

			d.reset(kz(5703.5250254166667));
			Assert::AreEqual(42229.5250254166667, q::q2DATE(d.get()), EPSILON,
				L"D(2015.08.13T12:36:02.196) => DATE", LINE_INFO());

			//@see https://msdn.microsoft.com/en-us/library/82ab7w69.aspx
#			pragma region
			d.reset(kz(q::DATE2q(-3.)));
			Assert::AreEqual(-3., q::q2DATE(d.get()), EPSILON,
				L"D(1899.12.27T00:00) => DATE", LINE_INFO());
			d.reset(kz(q::DATE2q(-2.5)));
			Assert::AreEqual(-2.5, q::q2DATE(d.get()), EPSILON,
				L"D(1899.12.28T12:00) => DATE", LINE_INFO());
			d.reset(kz(q::DATE2q(-2.)));
			Assert::AreEqual(-2., q::q2DATE(d.get()), EPSILON,
				L"D(1899.12.28T00:00) => DATE", LINE_INFO());
			d.reset(kz(q::DATE2q(-1.)));
			Assert::AreEqual(-1., q::q2DATE(d.get()), EPSILON,
				L"D(1899.12.29T00:00) => DATE", LINE_INFO());
			d.reset(kz(q::DATE2q(-.75)));
			Assert::AreEqual(+.75, std::abs(q::q2DATE(d.get())), EPSILON,	//https://msdn.microsoft.com/en-us/library/82ab7w69.aspx?f=255&MSPPError=-2147217396
				L"D(1899.12.30T18:00) => DATE", LINE_INFO());
			d.reset(kz(q::DATE2q(-.5)));
			Assert::AreEqual(+.5, std::abs(q::q2DATE(d.get())), EPSILON,	//https://msdn.microsoft.com/en-us/library/82ab7w69.aspx?f=255&MSPPError=-2147217396
				L"D(1899.12.30T12:00) => DATE", LINE_INFO());
			d.reset(kz(q::DATE2q(-.25)));
			Assert::AreEqual(+.25, std::abs(q::q2DATE(d.get())), EPSILON,	//https://msdn.microsoft.com/en-us/library/82ab7w69.aspx?f=255&MSPPError=-2147217396
				L"D(1899.12.30T06:00) => DATE", LINE_INFO());
			d.reset(kz(q::DATE2q(0.)));
			Assert::AreEqual(0., q::q2DATE(d.get()), EPSILON,
				L"D(1899.12.30T00:00) => DATE", LINE_INFO());
			d.reset(kz(q::DATE2q(.25)));
			Assert::AreEqual(.25, q::q2DATE(d.get()), EPSILON,
				L"D(1899.12.30T06:00) => DATE", LINE_INFO());
			d.reset(kz(q::DATE2q(.5)));
			Assert::AreEqual(.5, q::q2DATE(d.get()), EPSILON,
				L"D(1899.12.30T12:00) => DATE", LINE_INFO());
			d.reset(kz(q::DATE2q(.75)));
			Assert::AreEqual(.75, q::q2DATE(d.get()), EPSILON,
				L"D(1899.12.30T18:00) => DATE", LINE_INFO());
			d.reset(kz(q::DATE2q(1.)));
			Assert::AreEqual(1., q::q2DATE(d.get()), EPSILON,
				L"D(1899.12.31T00:00) => DATE", LINE_INFO());
			d.reset(kz(q::DATE2q(2.1)));
			Assert::AreEqual(2.1, q::q2DATE(d.get()), EPSILON,
				L"D(1900.01.01T00:00) => DATE", LINE_INFO());
			d.reset(kz(q::DATE2q(5.)));
			Assert::AreEqual(5., q::q2DATE(d.get()), EPSILON,
				L"D(1900.01.04T00:00) => DATE", LINE_INFO());
			d.reset(kz(q::DATE2q(5.25)));
			Assert::AreEqual(5.25, q::q2DATE(d.get()), EPSILON,
				L"D(1900.01.04T06:00) => DATE", LINE_INFO());
			d.reset(kz(q::DATE2q(5.5)));
			Assert::AreEqual(5.5, q::q2DATE(d.get()), EPSILON,
				L"D(1900.01.04T12:00) => DATE", LINE_INFO());
			d.reset(kz(q::DATE2q(5.875)));
			Assert::AreEqual(5.875, q::q2DATE(d.get()), EPSILON,
				L"D(1900.01.04T21:00) => DATE", LINE_INFO());
#			pragma endregion

			d.reset(kz(q::DATE2q(25569.)));
			Assert::AreEqual(25569., q::q2DATE(d.get()), EPSILON,
				L"D(1970.01.01T00:00) => DATE", LINE_INFO());
		}
		
		TEST_METHOD(canConvertToVariant)
		{
#			define VARIANT_TESTER(qVal, VTid, assignVal, message, lineInfo)	{\
				expect.vt = (VTid);	\
				expect.assignVal;	\
				q::K_ptr d((qVal));	\
				actual = q::q2Variant(d.get());	\
				Assert::IsTrue(q::test::VariantEqual(expect, actual), (message), (lineInfo));	\
				::VariantClear(&actual);	\
				::VariantClear(&expect);	\
			}

			::VARIANT expect, actual;
			::VariantInit(&expect);
			::VariantInit(&actual);
			std::wostringstream buffer;
			try {
				VARIANT_TESTER(ka(101), VT_ERROR, scode = DISP_E_PARAMNOTFOUND,
					L"(::) => VARIANT(DISP_E_PARAMNOTFOUND)", LINE_INFO());

				VARIANT_TESTER(kb(true), VT_BOOL, boolVal = VARIANT_TRUE,
					L"1b => VARIANT_TRUE", LINE_INFO());
				VARIANT_TESTER(kb(false), VT_BOOL, boolVal = VARIANT_FALSE,
					L"0b => VARIANT_FALSE", LINE_INFO());

				VARIANT_TESTER(kg(0xF3), VT_UI1, bVal = 0xF3,
					L"0xF3 => VARIANT(VT_UI1, 243)", LINE_INFO());

				VARIANT_TESTER(kh(27856), VT_I2, iVal = 27856,
					L"27856h => VARIANT(VT_I2, 27856)", LINE_INFO());
				buffer.str(L"");		buffer.clear();
				buffer << L"0Nh => VARIANT(VT_I2, " << nh << L')';
				VARIANT_TESTER(kh(nh), VT_I2, iVal = nh, buffer.str().c_str(), LINE_INFO());
				buffer.str(L"");		buffer.clear();
				buffer << L"0Wh => VARIANT(VT_I2, " << wh << L')';
				VARIANT_TESTER(kh(wh), VT_I2, iVal = wh, buffer.str().c_str(), LINE_INFO());

				VARIANT_TESTER(ki(476883888), VT_I4, lVal = 476883888,
					L"476883888i => VARIANT(VT_I4, 476883888)", LINE_INFO());
				buffer.str(L"");		buffer.clear();
				buffer << L"0Ni => VARIANT(VT_I4, " << ni << L')';
				VARIANT_TESTER(ki(ni), VT_I4, lVal = ni, buffer.str().c_str(), LINE_INFO());
				buffer.str(L"");		buffer.clear();
				buffer << L"0Wi => VARIANT(VT_I4, " << wi << L')';
				VARIANT_TESTER(ki(wi), VT_I4, lVal = wi, buffer.str().c_str(), LINE_INFO());

				VARIANT_TESTER(kj(7413760571230203088L), VT_I8, llVal = 7413760571230203088L,
					L"7413760571230203088j => VARIANT(VT_I8, 7413760571230203088)", LINE_INFO());
				buffer.str(L"");		buffer.clear();
				buffer << L"0Nj => VARIANT(VT_I8, " << nj << L')';
				VARIANT_TESTER(kj(nj), VT_I8, llVal = nj, buffer.str().c_str(), LINE_INFO());
				buffer.str(L"");		buffer.clear();
				buffer << L"0Wj => VARIANT(VT_I8, " << wj << L')';
				VARIANT_TESTER(kj(wj), VT_I8, llVal = wj, buffer.str().c_str(), LINE_INFO());

				VARIANT_TESTER(ke(967.23981f), VT_R4, fltVal = 967.23981f,
					L"967.23981e => VARIANT(VT_R4, 967.23981)", LINE_INFO());
				buffer.str(L"");		buffer.clear();
				buffer << L"0Ne => VARIANT(VT_R4, " << std::numeric_limits<FLOAT>::quiet_NaN() << L')';
				VARIANT_TESTER(ke(nf), VT_R4, fltVal = std::numeric_limits<FLOAT>::quiet_NaN(),
					buffer.str().c_str(), LINE_INFO());
				buffer.str(L"");		buffer.clear();
				buffer << L"0We => VARIANT(VT_R4, " << std::numeric_limits<FLOAT>::infinity() << L')';
				VARIANT_TESTER(ke(wf), VT_R4, fltVal = std::numeric_limits<FLOAT>::infinity(),
					buffer.str().c_str(), LINE_INFO());
				buffer.str(L"");		buffer.clear();
				buffer << L"-0We => VARIANT(VT_R4, " << - std::numeric_limits<FLOAT>::infinity() << L')';
				VARIANT_TESTER(ke(-wf), VT_R4, fltVal = - std::numeric_limits<FLOAT>::infinity(),
					buffer.str().c_str(), LINE_INFO());

				VARIANT_TESTER(kf(7413760.571230203088), VT_R8, dblVal = 7413760.571230203088,
					L"7413760.571230203088f => VARIANT(VT_R8, 7413760.571230203088)", LINE_INFO());
				buffer.str(L"");		buffer.clear();
				buffer << L"0Nf => VARIANT(VT_R8, " << std::numeric_limits<DOUBLE>::quiet_NaN() << L')';
				VARIANT_TESTER(kf(nf), VT_R8, dblVal = std::numeric_limits<DOUBLE>::quiet_NaN(),
					buffer.str().c_str(), LINE_INFO());
				buffer.str(L"");		buffer.clear();
				buffer << L"0Wf => VARIANT(VT_R8, " << std::numeric_limits<DOUBLE>::infinity() << L')';
				VARIANT_TESTER(kf(wf), VT_R8, dblVal = std::numeric_limits<DOUBLE>::infinity(),
					buffer.str().c_str(), LINE_INFO());
				buffer.str(L"");		buffer.clear();
				buffer << L"-0Wf => VARIANT(VT_R8, " << - std::numeric_limits<DOUBLE>::infinity() << L')';
				VARIANT_TESTER(kf(-wf), VT_R8, dblVal = - std::numeric_limits<DOUBLE>::infinity(),
					buffer.str().c_str(), LINE_INFO());
			}
			catch (std::runtime_error& ex) {
				::VariantClear(&actual);
				::VariantClear(&expect);
				Assert::Fail(ml::convert(CP_UTF8, ex.what()).c_str(), LINE_INFO());
			}

#			undef VARIANT_TESTER
		}

		TEST_METHOD(canConvertFromVariant)
		{
#			define k_TESTER(VTid, assignVal, qid, qtype, qVal, qField, message, lineInfo)	{\
				::VARIANT var;	\
				::VariantInit(&var);	\
				var.vt = (VTid);	\
				var.assignVal;	\
				q::K_ptr k;	\
				k.reset(q::Variant2q(var));	\
				Assert::AreEqual<signed char>((qid), k->t, (message), (lineInfo));	\
				Assert::AreEqual<qtype>((qVal), k->qField, (message), (lineInfo));	\
				::VariantClear(&var);	\
			}

			std::wostringstream buffer;

			k_TESTER(VT_ERROR, scode = DISP_E_PARAMNOTFOUND, 101, char, 0, g,
				L"VARIANT(DISP_E_PARAMNOTFOUND) => (::)", LINE_INFO());
			
			k_TESTER(VT_BOOL, boolVal = VARIANT_TRUE, -KB, G, 1, g,
				L"VARIANT_TRUE => 1b", LINE_INFO());
			k_TESTER(VT_BOOL, boolVal = VARIANT_FALSE, -KB, G, 0, g,
				L"VARIANT_FALSE => 0b", LINE_INFO());

			k_TESTER(VT_UI1, bVal = 0xF3, -KG, G, 0xF3, g,
				L"VARIANT(VT_UI1, 243) => 0xF3", LINE_INFO());

			k_TESTER(VT_I2, iVal = 27856, -KH, H, 27856, h,
				L"VARIANT(VT_I2, 27856) => 27856h", LINE_INFO());
			buffer.clear();		buffer.str(L"");
			buffer << L"VARIANT(VT_I2, " << nh << L") => 0Nh";
			k_TESTER(VT_I2, iVal = nh, -KH, H, nh, h, buffer.str().c_str(), LINE_INFO());
			buffer.clear();		buffer.str(L"");
			buffer << L"VARIANT(VT_I2, " << wh << L") => 0Wh";
			k_TESTER(VT_I2, iVal = wh, -KH, H, wh, h, buffer.str().c_str(), LINE_INFO());

			k_TESTER(VT_I4, lVal = 476883888, -KI, I, 476883888, i,
				L"VARIANT(VT_I4, 476883888) => 476883888i", LINE_INFO());
			buffer.clear();		buffer.str(L"");
			buffer << L"VARIANT(VT_I4, " << ni << L") => 0Ni";
			k_TESTER(VT_I4, lVal = ni, -KI, I, ni, i, buffer.str().c_str(), LINE_INFO());
			buffer.clear();		buffer.str(L"");
			buffer << L"VARIANT(VT_I4, " << wi << L") => 0Wi";
			k_TESTER(VT_I4, lVal = wi, -KI, I, wi, i, buffer.str().c_str(), LINE_INFO());

			k_TESTER(VT_I8, llVal = 7413760571230203088L, -KJ, J, 7413760571230203088L, j,
				L"VARIANT(VT_I8, 7413760571230203088) => 7413760571230203088j", LINE_INFO());
			buffer.clear();		buffer.str(L"");
			buffer << L"VARIANT(VT_I8, " << nj << L") => 0Nj";
			k_TESTER(VT_I8, llVal = nj, -KJ, J, nj, j, buffer.str().c_str(), LINE_INFO());
			buffer.clear();		buffer.str(L"");
			buffer << L"VARIANT(VT_I8, " << wj << L") => 0Wj";
			k_TESTER(VT_I8, llVal = wj, -KJ, J, wj, j, buffer.str().c_str(), LINE_INFO());

			k_TESTER(VT_R4, fltVal = 967.23981f, -KE, E, 967.23981f, e,
				L"VARIANT(VT_R4, 967.23981) => 967.23981e", LINE_INFO());
			{
				buffer.clear();		buffer.str(L"");
				buffer << L"VARIANT(VT_R4, " << std::numeric_limits<FLOAT>::quiet_NaN() << L") => 0Ne";
				::VARIANT var;		::VariantInit(&var);
				var.vt = VT_R4;		var.fltVal = std::numeric_limits<FLOAT>::quiet_NaN();
				q::K_ptr k;			k.reset(q::Variant2q(var));
				Assert::AreEqual<signed char>(-KE, k->t, buffer.str().c_str(), LINE_INFO());
				Assert::IsTrue(std::isnan(k->e), buffer.str().c_str(), LINE_INFO());
				::VariantClear(&var);
			}
			buffer.clear();		buffer.str(L"");
			buffer << L"VARIANT(VT_R4, " << std::numeric_limits<FLOAT>::infinity() << L") => 0We";
			k_TESTER(VT_R4, fltVal = std::numeric_limits<FLOAT>::infinity(), -KE, F, wf, e,
				buffer.str().c_str(), LINE_INFO());
			buffer.clear();		buffer.str(L"");
			buffer << L"VARIANT(VT_R4, " << - std::numeric_limits<FLOAT>::infinity() << L") => -0We";
			k_TESTER(VT_R4, fltVal = - std::numeric_limits<FLOAT>::infinity(), -KE, F, -wf, e,
				buffer.str().c_str(), LINE_INFO());

			k_TESTER(VT_R8, dblVal = 7413760.571230203088, -KF, F, 7413760.571230203088, f,
				L"VARIANT(VT_R8, 7413760.571230203088) => 7413760.571230203088f", LINE_INFO());
			{
				buffer.clear();		buffer.str(L"");
				buffer << L"VARIANT(VT_R8, " << std::numeric_limits<DOUBLE>::quiet_NaN() << L") => 0Nf";
				::VARIANT var;		::VariantInit(&var);
				var.vt = VT_R8;		var.dblVal = std::numeric_limits<DOUBLE>::quiet_NaN();
				q::K_ptr k;			k.reset(q::Variant2q(var));
				Assert::AreEqual<signed char>(-KF, k->t, buffer.str().c_str(), LINE_INFO());
				Assert::IsTrue(std::isnan(k->f), buffer.str().c_str(), LINE_INFO());
				::VariantClear(&var);
			}
			buffer.clear();		buffer.str(L"");
			buffer << L"VARIANT(VT_R8, " << std::numeric_limits<DOUBLE>::infinity() << L") => 0Wf";
			k_TESTER(VT_R8, dblVal = std::numeric_limits<DOUBLE>::infinity(), -KF, F, wf, f,
				buffer.str().c_str(), LINE_INFO());
			buffer.clear();		buffer.str(L"");
			buffer << L"VARIANT(VT_R8, " << - std::numeric_limits<DOUBLE>::infinity() << L") => -0Wf";
			k_TESTER(VT_R8, dblVal = - std::numeric_limits<DOUBLE>::infinity(), -KF, F, -wf, f,
				buffer.str().c_str(), LINE_INFO());

#			undef k_TESTER
		}
	};
}

/////////////////////////////////////////////////////////////////////////////

#include <sstream>

namespace q {
	namespace test {

		template<typename T>
		std::wstring ToString(T const& value) {
			std::wostringstream buffer;
			buffer << value;
			return buffer.str();
		}

		template<typename E, typename Mapper>
		std::wstring ToString(std::vector<E> const& value, Mapper const& map) {
			std::wostringstream buffer;
			auto i = value.cbegin();
			buffer << '[';
			if (i != value.cend()) buffer << map(*i++);
			for (; i != value.cend(); ++i) {
				buffer << ',' << map(*i);
			}
			buffer << ']';
			return buffer.str();
		}

		bool VariantEqual(::VARIANT const& a, ::VARIANT const& b) {
#			define COMPARE_VARIANT_FIELD(VTid, field)	\
			case (VTid):	\
				return a.field == b.field;	\
			case (VTid) | VT_BYREF:	\
				return (a.p##field == b.p##field) || ((a.p##field != NULL) && (b.p##field != NULL) && (*a.p##field == *b.p##field))

			if (a.vt != b.vt) {
				return false;
			}
			else switch (a.vt) {
			COMPARE_VARIANT_FIELD(VT_ERROR, scode);
			COMPARE_VARIANT_FIELD(VT_BOOL, boolVal);
			COMPARE_VARIANT_FIELD(VT_UI1, bVal);
			COMPARE_VARIANT_FIELD(VT_I1, cVal);
			COMPARE_VARIANT_FIELD(VT_UI2, uiVal);
			COMPARE_VARIANT_FIELD(VT_I2, iVal);
			COMPARE_VARIANT_FIELD(VT_UI4, ulVal);
			COMPARE_VARIANT_FIELD(VT_I4, lVal);
			COMPARE_VARIANT_FIELD(VT_UI8, ullVal);
			COMPARE_VARIANT_FIELD(VT_I8, llVal);
			COMPARE_VARIANT_FIELD(VT_UINT, uintVal);
			COMPARE_VARIANT_FIELD(VT_INT, intVal);
			case VT_R4:
				return (a.fltVal == b.fltVal) || (std::isnan(a.fltVal) && std::isnan(b.fltVal));
			case VT_R4 | VT_BYREF:
				return (a.pfltVal == b.pfltVal) ||
					((a.pfltVal != NULL) && (b.pfltVal != NULL) &&
					((*a.pfltVal == *b.pfltVal) || (std::isnan(*a.pfltVal) && std::isnan(*b.pfltVal))));
			case VT_R8:
				return (a.dblVal == b.dblVal) || (std::isnan(a.dblVal) && std::isnan(b.dblVal));
			case VT_R8 | VT_BYREF:
				return (a.pdblVal == b.pdblVal) ||
					((a.pdblVal != NULL) && (b.pdblVal != NULL) &&
					((*a.pdblVal == *b.pdblVal) || (std::isnan(*a.pdblVal) && std::isnan(*b.pdblVal))));
			COMPARE_VARIANT_FIELD(VT_DATE, date);
			case VT_BSTR:
				return 0 == std::wcscmp(a.bstrVal, b.bstrVal);
			case VT_BSTR | VT_BYREF:
				return (a.pbstrVal == b.pbstrVal) ||
					((a.pbstrVal != NULL) && (b.pbstrVal != NULL) && (0 == std::wcscmp(*a.pbstrVal, *b.pbstrVal)));
			case VT_VARIANT | VT_BYREF:
				return (a.pvarVal == b.pvarVal) ||
					((a.pvarVal != NULL) && (b.pvarVal != NULL) && VariantEqual(*a.pvarVal, *b.pvarVal));
			case VT_EMPTY:
			case VT_NULL:
				return true;
			default: {
					std::ostringstream buffer;
					buffer << "nyi: VARIANT comparison for 0x" << util::hexBytes(a.vt);
					throw std::runtime_error(buffer.str());
				}
			}

#		undef COMPARE_VARIANT_FIELD
		}
	}
}//namespace q

namespace Microsoft{
	namespace VisualStudio {
		namespace CppUnitTestFramework {

			template<>
			static std::wstring ToString<long long>(long long const& value) {
				return q::test::ToString(value);
			}

			template<>
			static std::wstring ToString<std::vector<long long> >(std::vector<long long> const& value) {
				return q::test::ToString(value, [](long long x) { return x; });
			}

			template<>
			static std::wstring ToString<std::vector<double> >(std::vector<double> const& value) {
				return q::test::ToString(value, [](double x) { return x; });
			}

			template<>
			static std::wstring ToString<std::vector<std::string> >(std::vector<std::string> const& value) {
				return q::test::ToString(value, [](std::string const& x) -> std::wstring {
					return L'"' + ml::convert(q::DEFAULT_CP, x.c_str()) + L'"';
				});
			}

			template<>
			static std::wstring ToString<::VARIANT>(::VARIANT const& value) {
				std::wostringstream buffer;
				buffer << L"VARIANT(0x" << util::hexBytes(value.vt) << L")[" << util::hexBytes(value) << L']';
				return buffer.str();
			}

		}
	}
}//namespace Microsoft
