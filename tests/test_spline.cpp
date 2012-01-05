
//#include <ccnoise/value_noise.h>

#include <ccmath/spline.h>
#include <ccmath/abs.h>

#include <cppunit/TestSuite.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/CompilerOutputter.h>

#include <boost/math/special_functions/next.hpp>

#include <sysexits.h>
#include <iostream>
#include <vector>
#include <limits>


template<typename T>
struct MyLimits
{
	static const T MaxUlp = 8;
};

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

	template<typename scalarT>
	friend MyVector<T> operator*(const MyVector<T>& lhs, const scalarT rhs)
	{
		return MyVector<T>(lhs._data[0] * rhs, lhs._data[1] * rhs, lhs._data[2] * rhs );
	}
	
	template<typename scalarT>
	friend MyVector<T> operator*(scalarT lhs, const MyVector<T>& rhs)
	{
		return rhs*lhs;
	}

	//std::ostream& operator<<(std::ostream& os, const MyVector<T>& v)
	std::ostream& operator<<(std::ostream& os)
	{
		os << "[" << this->_data[0] << ", " << this->_data[1] << ", " << this->_data[2] << "]"; return os;
	}
	
	//template<typename scalarT>
	//MyVector<T> operator*(float s, const MyVector<T>& v)
	//{
	//	return v*s;
	//}

	protected:
		T _data[3];
};


class SplineTest : public CppUnit::TestFixture
{
		//Declare the suite
		CPPUNIT_TEST_SUITE( SplineTest );
		//Declare each fixture test case
		CPPUNIT_TEST( test_float_spline ); 
		//CPPUNIT_TEST( test_float_array_spline ); 
		//CPPUNIT_TEST( test_vector_spline ); 
		//End the suite declaration
		CPPUNIT_TEST_SUITE_END();

	public:

		void test_float_spline();
		void test_float_array_spline();
		void test_vector_spline();

	private:

};

void SplineTest::test_float_spline()
{
	std::vector<float> xknots;
	xknots.push_back(0.1f);
	xknots.push_back(0.9f);
	xknots.push_back(0.5f);
	xknots.push_back(0.3f);
	//xknots.push_back(0.7f);


	float fstart = 0.f;
	float fend = 1.f;
	float finc = 0.1;
	float sample;
	float fresult;
	
	std::cout << "x = [";
	for (sample = fstart; sample < fend+finc; sample += finc)
	{
		std::cout << sample << " ";
	}
	
	std::cout << "];" << std::endl;

	std::cout << "y = [";
	for (sample = fstart; sample < fend+finc; sample += finc)
	{
		std::cout << ccmath::spline(sample, xknots) << " ";
	}	
	std::cout << "];" << std::endl;

	std::cout << "knots: " << xknots[0] << ", " << xknots[1] << ", " << xknots[2] << ", " << xknots[3] << std::endl;

	//Compare spline results using Ulp as opposed to machine epsilon. It happens occasionally that the epsilon is smaller than the float difference when
	//the floats are supposed to be equal.
	std::cout << "spline(-5, knots) == 0.9  =>  " << (boost::math::float_distance(ccmath::spline(-5.f,xknots), xknots[1]) < MyLimits<float>::MaxUlp) << std::endl;
	std::cout << "spline(-5, knots) == 0.9  =>  " << (boost::math::float_distance(ccmath::spline(0.f,xknots), xknots[1]) < MyLimits<float>::MaxUlp) << std::endl;
	
	std::cout << "spline( 1, knots) == 0.5  =>  " << (boost::math::float_distance(ccmath::spline( 1.f,xknots), xknots[2]) < MyLimits<float>::MaxUlp) << std::endl;
	std::cout << "spline( 1, knots) == 0.5  =>  " << (boost::math::float_distance(ccmath::spline( 3.f,xknots), xknots[2]) < MyLimits<float>::MaxUlp) << std::endl;

	//std::cout << "spline( 1, knots) == 0.5  =>  " << std::fixed << std::setprecision(15) << ccmath::spline( 1.f,xknots) << ", " << xknots[2] << std::endl;
}

void SplineTest::test_vector_spline()
{
	typedef MyVector<float> vecf;
	std::vector<vecf> xknots;
	xknots.push_back( vecf(0.1f, 0.1f, 0.1f) );
	xknots.push_back( vecf(0.9f, 0.9f, 0.9f) );
	xknots.push_back( vecf(0.5f, 0.5f, 0.5f) );
	xknots.push_back( vecf(0.3f, 0.3f, 0.3f) );

	vecf v = vecf(0.5f, 0.1f, 0.3f) * 1.5;

	v = 1.0f * v;

	//vecf v = ccmath::spline(0.2f, xknots);

}

void SplineTest::test_float_array_spline()
{
	size_t numknots = 4;
	float xknots[4] = { 0.1f, 0.9f, 0.5f, 0.3f };

	float fstart = 0.f;
	float fend = 1.f;
	float finc = 0.1;
	float sample;
	float fresult;
	
	std::cout << "x = [";
	for (sample = fstart; sample < fend+finc; sample += finc)
	{
		std::cout << sample << " ";
	}
	
	std::cout << "];" << std::endl;

	std::cout << "y = [";
	for (sample = fstart; sample < fend+finc; sample += finc)
	{
		std::cout << ccmath::spline(sample, xknots, numknots) << " ";
	}	
	std::cout << "];" << std::endl;

	CPPUNIT_ASSERT( ccmath::abs(ccmath::spline(-5.f,xknots,numknots) - xknots[1]) < std::numeric_limits<float>::epsilon() );
	CPPUNIT_ASSERT( ccmath::abs(ccmath::spline( 0.f,xknots,numknots) - xknots[1]) < std::numeric_limits<float>::epsilon() );
	CPPUNIT_ASSERT( ccmath::abs(ccmath::spline( 1.f,xknots,numknots) - xknots[2]) < std::numeric_limits<float>::epsilon() );
	CPPUNIT_ASSERT( ccmath::abs(ccmath::spline( 3.f,xknots,numknots) - xknots[2]) < std::numeric_limits<float>::epsilon() );
}

//namespace ccn = ccnoise;

int main(int argc, char* argv[])
{
	std::cout << "ccmath::spline interpolation test." << std::endl;

	//spline interpolation tests:

	CppUnit::TextTestRunner runner;

	runner.addTest( SplineTest::suite() );
	runner.setOutputter( CppUnit::CompilerOutputter::defaultOutputter( &runner.result(), std::cerr ) );

	runner.run("", false, true, true);

	return EX_OK;
}

