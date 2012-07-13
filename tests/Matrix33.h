
#ifndef __MATRIX33_H__
#define __MATRIX33_H__

#include <iostream>

class Vector3;

class Matrix33
{
	public:
		Matrix33();

		Matrix33(float a00, float a01, float a02, 
				float a10, float a11, float a12, 
				float a20, float a21, float a22);

		Matrix33(const Vector3& x, const Vector3& y, const Vector3& z);
		
		//Indexing operator
		float operator() (unsigned int row, unsigned int col) const;
	
		//Matrix multiplication
		const Matrix33 operator*(const Matrix33& rhs) const;
		
		void operator=(const Matrix33& m);

		bool operator==(const Matrix33& m) const;


	protected:
		float _data[3][3];
};

/****************************************************************************************************/

//Matrix stream output
std::ostream& operator<<(std::ostream& os, const Matrix33& m);

#endif

