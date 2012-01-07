
#include <ccmath/clamp.h>
#include <ccmath/abs.h>

#include <boost/mpl/int.hpp>

#include <cppunit/TestSuite.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>

#include <OpenEXR/ImathVec.h>
#include <ccmath/traits/imath_type_traits.h>
#include <ccmath/floor.h>

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

class FloorTest : public CppUnit::TestFixture
{
		//Declare the suite
		CPPUNIT_TEST_SUITE( FloorTest );
		//Declare each fixture test case
		CPPUNIT_TEST( test_pod_real_floor ); 
		CPPUNIT_TEST( test_imath_vec_floor ); 
		CPPUNIT_TEST( test_custom_vec_floor ); 
		//End the suite declaration
		CPPUNIT_TEST_SUITE_END();

	public:

		void test_pod_real_floor();
		void test_imath_vec_floor();
		void test_custom_vec_floor();
};

/****************************************************************************************************/

void FloorTest::test_pod_real_floor()
{
	float x;
	x =   5.7; ccmath::floor(x);	CPPUNIT_ASSERT( (x - 5.f) < std::numeric_limits<float>::epsilon() );
	                                  
	x =  5.7; ccmath::floor(x, x);	CPPUNIT_ASSERT( (x - 5.f) < std::numeric_limits<float>::epsilon() );
}

/****************************************************************************************************/

void FloorTest::test_imath_vec_floor()
{
	Imath::Vec2<float> v(2.3, 5.7); ccmath::floor(v); CPPUNIT_ASSERT( v == Imath::Vec2<int>(2, 5) );
	Imath::Vec3<float> x(3.4, 5.7, 8.6), r; ccmath::floor(r, x); CPPUNIT_ASSERT( r == Imath::Vec3<float>(3, 5, 8) );
}

/****************************************************************************************************/

void FloorTest::test_custom_vec_floor()
{
	//_custom_vec3 = MyVector<int>(-7, 8, 25); ccmath::clamp(_custom_vec3, 1, 10); CPPUNIT_ASSERT( _custom_vec3 == MyVector<int>(1, 8, 10) );
}

/****************************************************************************************************/

int main(int argc, char* argv[])
{
	std::cout << "ccmath::floor test." << std::endl;

	CppUnit::TextTestRunner runner;

	runner.addTest( FloorTest::suite() );
	runner.setOutputter( CppUnit::CompilerOutputter::defaultOutputter( &runner.result(), std::cerr ) );

	runner.run("", false, true, true);

	return EX_OK;

}

