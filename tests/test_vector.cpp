
//#include <ccnoise/value_noise.h>


#include <cppunit/TestSuite.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/CompilerOutputter.h>

#include <sysexits.h>
#include <iostream>
#include <vector>
#include <limits>

#include "Vector3.h"
#include "Matrix33.h"


class VectorTest : public CppUnit::TestFixture
{
		//Declare the suite
		CPPUNIT_TEST_SUITE( VectorTest );
		//Declare each fixture test case
		CPPUNIT_TEST( test_vector_equality ); 
		CPPUNIT_TEST( test_vector_assign ); 
		CPPUNIT_TEST( test_vector_multiplication ); 
		CPPUNIT_TEST( test_vector_division ); 
		CPPUNIT_TEST( test_vector_addition ); 

		CPPUNIT_TEST( test_matrix_equality );
		CPPUNIT_TEST( test_vector_matrix_mult );
		//End the suite declaration
		CPPUNIT_TEST_SUITE_END();

	public:

		void test_vector_equality();
		void test_vector_assign();
		void test_vector_multiplication();
		void test_vector_division();
		void test_vector_addition();

		void test_matrix_equality();

		void test_vector_matrix_mult();

	private:

};

void VectorTest::test_vector_equality()
{
	Vector3 v1(1.f, 2.f, 3.f);
	Vector3 v2(1.f, 2.f, 3.f);
	Vector3 v3(1.1f, 2.f, 3.f);

	CPPUNIT_ASSERT( v1 == v2 );
	CPPUNIT_ASSERT( v3 != v2 );
}

void VectorTest::test_vector_assign()
{
	Vector3 v1(1.f, 2.f, 3.f);
	Vector3 v2;

	v2 = v1;

	CPPUNIT_ASSERT( v1 == v2 );
}

void VectorTest::test_vector_multiplication()
{
	Vector3 v1(1.f, 2.f, 3.f);
	Vector3 v2(4.f, 5.f, 6.f);

	float ans = v1*v2; // dot product

	CPPUNIT_ASSERT( (ans - 32.f) < std::numeric_limits<float>::epsilon() );
}

void VectorTest::test_vector_division()
{
	CPPUNIT_ASSERT( (Vector3(89.6f, 160.f, 230.4f) / 64.f) == Vector3(1.4f, 2.5f, 3.6f) );
}

void VectorTest::test_vector_addition()
{
	CPPUNIT_ASSERT( (Vector3(0.f, 0.f, 0.f) + Vector3(1.f, 2.f, 3.f)) == Vector3(1.f, 2.f, 3.f) );
}

void VectorTest::test_matrix_equality()
{
	Matrix33 m(Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1));
	CPPUNIT_ASSERT( m == m );
}

void VectorTest::test_vector_matrix_mult()
{
	Matrix33 m(Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1));
	Vector3 v(1.f, 2.f, 3.f);
	CPPUNIT_ASSERT( (v*m) == v );
}

/****************************************************************************************************/

int main(int argc, char* argv[])
{
	std::cout << "Vector3 and Matrix33 tests." << std::endl;

	//spline interpolation tests:

	CppUnit::TextTestRunner runner;

	runner.addTest( VectorTest::suite() );
	runner.setOutputter( CppUnit::CompilerOutputter::defaultOutputter( &runner.result(), std::cerr ) );

	runner.run("", false, true, true);

	return EX_OK;
}

