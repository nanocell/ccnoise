
#include <ccmath/clamp.h>
#include <ccmath/abs.h>

#include <boost/mpl/int.hpp>

#include <cppunit/TestSuite.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>

#include <OpenEXR/ImathVec.h>

#include <ccmath/traits/type_traits.h>
#include <ccmath/traits/imath_type_traits.h>

#include <sysexits.h>
#include <iostream>
#include <limits>

//Custom 3 dimensional vector class for testing purposes
template<typename T>
struct MyVector
{
	typedef MyVector<T> type;
	typedef T value_type;
	typedef unsigned int size_type;

	typedef boost::mpl::int_<3> size;	

	MyVector() {}
	MyVector(T x, T y, T z) { _data[0] = x; _data[1] = y; _data[2] = z; }

	T& operator[](unsigned int i) { return _data[i]; }
	const T& operator[](unsigned int i) const { return _data[i]; }

	bool operator==(const MyVector<T>& rhs) const { return (this->_data[0] == rhs._data[0]) && (this->_data[1] == rhs._data[1]) && (this->_data[0] == rhs._data[0]); } 

	protected:
		T _data[3];
};


/****************************************************************************************************/

class ClampTest : public CppUnit::TestFixture
{
		//Declare the suite
		CPPUNIT_TEST_SUITE( ClampTest );
		//Declare each fixture test case
		CPPUNIT_TEST( test_pod_integer_clamp ); 
		CPPUNIT_TEST( test_pod_real_clamp ); 
		CPPUNIT_TEST( test_imath_vec_clamp ); 
		CPPUNIT_TEST( test_custom_vec_clamp ); 
		//End the suite declaration
		CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();

		void test_pod_integer_clamp();
		void test_pod_real_clamp();
		void test_container_clamp();
		void test_imath_vec_clamp();
		void test_custom_vec_clamp();

	private:
		std::vector<int> _int_input_vector;
		std::vector<int> _int_result_vector;
		std::vector<float> _flt_vector;

		Imath::Vec2<int> _imath_vec2_int;
		MyVector<int> _custom_vec3;

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
	_custom_vec3 = MyVector<int>(-7, 8, 15); //I'm not sure why the type deduction doesn't work automatically
}

/****************************************************************************************************/

void ClampTest::tearDown()
{

}

/****************************************************************************************************/

void ClampTest::test_pod_integer_clamp()
{
	int x;
	
	x =   5; ccmath::clamp(x, 1, 10);	CPPUNIT_ASSERT(x == 5);

	x =  -5; ccmath::clamp(x, 1, 10);	CPPUNIT_ASSERT(x == 1);
	x =  15; ccmath::clamp(x, 1, 10);	CPPUNIT_ASSERT(x == 10);

	x = -50; ccmath::clamp(x, -10, 10);	CPPUNIT_ASSERT(x == -10);
	x =  50; ccmath::clamp(x, -10, 10);	CPPUNIT_ASSERT(x == 10); 
}

/****************************************************************************************************/

void ClampTest::test_pod_real_clamp()
{
	float x;
	x =   5; ccmath::clamp(x, 1.f, 10.f);	CPPUNIT_ASSERT( (x - 5.f) < std::numeric_limits<float>::epsilon() );
	                                  
	x =  -5; ccmath::clamp(x, 1.f, 10.f);	CPPUNIT_ASSERT( (x - 1.f) < std::numeric_limits<float>::epsilon() );
	x =  15; ccmath::clamp(x, 1.f, 10.f);	CPPUNIT_ASSERT( (x - 10.f) < std::numeric_limits<float>::epsilon() );	
                                    
	x = -50; ccmath::clamp(x, -10.f, 10.f); CPPUNIT_ASSERT( (x + 10.f) < std::numeric_limits<float>::epsilon() );
	x =  50; ccmath::clamp(x, -10.f, 10.f); CPPUNIT_ASSERT( (x - 10.f) < std::numeric_limits<float>::epsilon() );
}

/****************************************************************************************************/

void ClampTest::test_imath_vec_clamp()
{
	_imath_vec2_int.setValue(15,-20); ccmath::clamp(_imath_vec2_int, 1, 10); CPPUNIT_ASSERT( _imath_vec2_int == Imath::Vec2<int>(10, 1) );
}

/****************************************************************************************************/

void ClampTest::test_custom_vec_clamp()
{
	_custom_vec3 = MyVector<int>(-7, 8, 25); ccmath::clamp(_custom_vec3, 1, 10); CPPUNIT_ASSERT( _custom_vec3 == MyVector<int>(1, 8, 10) );
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

