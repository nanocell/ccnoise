
#include <ccmath/clamp.h>
#include <ccmath/abs.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>

#include <OpenEXR/ImathVec.h>
#include <ccmath/imath_container_adaptor.h>

#include <sysexits.h>
#include <iostream>
#include <limits>

//Custom vector class for testing purposes
template<typename T>
struct MyVector
{
	typedef MyVector<T> type;
	typedef T value_type;
	typedef unsigned int size_type;
	
	static inline size_type size(const Imath::Vec2<T>& container) { return 3; }
	static inline void resize(Imath::Vec2<T>& container, size_type entries) { }


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

void ClampTest::test_imath_vec_clamp()
{
	CPPUNIT_ASSERT( ccmath::clamp<2>(_imath_vec2_int, 1, 10) == Imath::Vec2<int>(10, 1) );
}

/****************************************************************************************************/

void ClampTest::test_custom_vec_clamp()
{
	CPPUNIT_ASSERT( ccmath::clamp<2>(_custom_vec3, 1, 10) == MyVector<int>(1, 8, 10) );
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

