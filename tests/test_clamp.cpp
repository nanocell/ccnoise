
#include <ccmath/clamp.h>
#include <ccmath/abs.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>

#include <OpenEXR/ImathVec.h>

#include <sysexits.h>
#include <iostream>
#include <limits>

/****************************************************************************************************/

class ClampTest : public CppUnit::TestFixture
{
		//Declare the suite
		CPPUNIT_TEST_SUITE( ClampTest );
		//Declare each fixture test case
		CPPUNIT_TEST( test_pod_integer_clamp ); 
		CPPUNIT_TEST( test_pod_real_clamp ); 
		//CPPUNIT_TEST( test_container_clamp ); 
		CPPUNIT_TEST( test_imath_vec_clamp ); 
		//End the suite declaration
		CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();

		void test_pod_integer_clamp();
		void test_pod_real_clamp();
		void test_container_clamp();
		void test_imath_vec_clamp();

	private:
		std::vector<int> _int_input_vector;
		std::vector<int> _int_result_vector;
		std::vector<float> _flt_vector;

		Imath::Vec2<int> _imath_vec2_int;

};

/****************************************************************************************************/

void ClampTest::setUp()
{
	_int_input_vector.resize(3);
	_int_input_vector[0] = -15;
	_int_input_vector[1] = 5;
	_int_input_vector[2] = 20;

	_int_result_vector.resize(3);
	_int_result_vector[0] = 1;
	_int_result_vector[1] = 5;
	_int_result_vector[2] = 10;

	_imath_vec2_int.setValue(15, -20);
}

/****************************************************************************************************/

void ClampTest::tearDown()
{

}

/****************************************************************************************************/

void ClampTest::test_pod_integer_clamp()
{
	CPPUNIT_ASSERT( ccmath::clamp(5, 1, 10) == 5);
	CPPUNIT_ASSERT( ccmath::clamp(-5, 1, 10) == 1);
	CPPUNIT_ASSERT( ccmath::clamp(15, 1, 10) == 10);

	CPPUNIT_ASSERT( ccmath::clamp(-50, -10, 10) == -10);
	CPPUNIT_ASSERT( ccmath::clamp(50, -10, 10) == 10); 
}

/****************************************************************************************************/

void ClampTest::test_pod_real_clamp()
{
	CPPUNIT_ASSERT( ccmath::abs(ccmath::clamp(5.f, 1.f, 10.f) - 5.f) < std::numeric_limits<float>::epsilon() );
	CPPUNIT_ASSERT( ccmath::abs(ccmath::clamp(-5.f, 1.f, 10.f) - 1.f) < std::numeric_limits<float>::epsilon() );
	CPPUNIT_ASSERT( ccmath::abs(ccmath::clamp(15.f, 1.f, 10.f) - 10.f) < std::numeric_limits<float>::epsilon() );	

	CPPUNIT_ASSERT( ccmath::abs(ccmath::clamp(-50.f, -10.f, 10.f) + 10.f) < std::numeric_limits<float>::epsilon() );
	CPPUNIT_ASSERT( ccmath::abs(ccmath::clamp(50.f, -10.f, 10.f) - 10.f) < std::numeric_limits<float>::epsilon() );
}

/****************************************************************************************************/

void ClampTest::test_container_clamp()
{
	//CPPUNIT_ASSERT( ccmath::clamp<3>(_int_input_vector, 1, 10) == _int_result_vector );
}

/****************************************************************************************************/

void ClampTest::test_imath_vec_clamp()
{
	CPPUNIT_ASSERT( ccmath::clamp<2>(_imath_vec2_int, 1, 10) == Imath::Vec2<int>(10, 1) );
}

/****************************************************************************************************/

template <int size, typename T>
static inline T clampgen(T x, T a, T b)
{
	return (x < a ? a : (x > b ? b : x) );
}

/****************************************************************************************************/

int main(int argc, char* argv[])
{
	std::cout << "ccmath::clamp test." << std::endl;

	CppUnit::TextTestRunner runner;

	runner.addTest( ClampTest::suite() );
	runner.setOutputter( CppUnit::CompilerOutputter::defaultOutputter( &runner.result(), std::cerr ) );

	runner.run("", false, true, true);

	return EX_OK;

}

