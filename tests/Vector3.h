
#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <limits>

#include <iostream>

//Forward declaration of Matrix33 class.
class Matrix33;

//Custom 3 dimensional vector class for testing purposes
class Vector3
{
	public:
		//Constructors
		Vector3();
		Vector3(float x, float y, float z);

		//Index operator
		float& operator() (unsigned int i);
		const float& operator() (unsigned int i) const;

		//Vector equality operator
		bool operator==(const Vector3& rhs) const;
		
		//Vector inequality operator
		bool operator!=(const Vector3& rhs) const;
		
		//Multiplying two vectors will perform a dot/inner product
		float operator*(const Vector3& rhs) const;
		
		//Vector3 - Matrix33 multiplication
		Vector3 operator*(const Matrix33& rhs);
		
		//Vector-scalar multiplication
		Vector3 operator*(float rhs) const;
			
		//Vector-scalar division
		Vector3 operator/(float rhs) const;
		
		//vector addition operator
		Vector3 operator+(const Vector3& rhs) const;
		
		//Vector subtraction operator
		Vector3 operator-(const Vector3 rhs) const;

		//Vector assignment
		void operator=(const Vector3& v);
	
	protected:
		float _data[3];
};

/****************************************************************************************************/

//Scalar-vector multiplication
Vector3 operator*(float lhs, const Vector3& rhs);

//Vector stream output
std::ostream& operator<<(std::ostream& os, const Vector3& v);

#endif

