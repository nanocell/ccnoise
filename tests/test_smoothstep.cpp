
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
#include <ccmath/smoothstep.h>

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

class SmoothStepTest : public CppUnit::TestFixture
{
		//Declare the suite
		CPPUNIT_TEST_SUITE( SmoothStepTest );
		//Declare each fixture test case
		CPPUNIT_TEST( test_pod_real ); 
		CPPUNIT_TEST( test_imath_vec ); 
		CPPUNIT_TEST( test_custom_vec ); 
		//End the suite declaration
		CPPUNIT_TEST_SUITE_END();

	public:

		void test_pod_real();
		void test_imath_vec();
		void test_custom_vec();
};

/****************************************************************************************************/

void SmoothStepTest::test_pod_real()
{
	float r;
	for (float x = 0; x < 1; x+=0.1f)
	{
		ccmath::smoothstep(r,x);
		std::cout << "smooth(" << x << ") => " << r << std::endl;
	}
	
	for (float x = 0; x < 1; x+=0.05f)
	{
		ccmath::smoothstep(r,x, 0.3f, 0.8f);
		std::cout << "smooth(" << x << ", 0.3, 0.8) => " << r << std::endl;
	}
}

/****************************************************************************************************/

void SmoothStepTest::test_imath_vec()
{
	Imath::Vec2<float> v(2.3, 5.7); ccmath::smoothstep(v,v);
	Imath::Vec3<float> x(3.4, 5.7, 8.6), r; ccmath::smoothstep(r,x);
}

/****************************************************************************************************/

void SmoothStepTest::test_custom_vec()
{
	//_custom_vec3 = MyVector<int>(-7, 8, 25); ccmath::clamp(_custom_vec3, 1, 10); CPPUNIT_ASSERT( _custom_vec3 == MyVector<int>(1, 8, 10) );
}

/****************************************************************************************************/

int main(int argc, char* argv[])
{
	std::cout << "ccmath::floor test." << std::endl;

	CppUnit::TextTestRunner runner;

	runner.addTest( SmoothStepTest::suite() );
	runner.setOutputter( CppUnit::CompilerOutputter::defaultOutputter( &runner.result(), std::cerr ) );

	runner.run("", false, true, true);

	return EX_OK;

}

