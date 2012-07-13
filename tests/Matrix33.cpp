
#include "Matrix33.h"
#include "Vector3.h"

Matrix33::Matrix33() {}

Matrix33::Matrix33(float a00, float a01, float a02, 
		float a10, float a11, float a12, 
		float a20, float a21, float a22)
{
	_data[0][0] = a00; _data[0][1] = a01; _data[0][2] = a02;
	_data[1][0] = a10; _data[1][1] = a11; _data[1][2] = a12;
	_data[2][0] = a20; _data[2][1] = a21; _data[2][2] = a22;
}

Matrix33::Matrix33(const Vector3& x, const Vector3& y, const Vector3& z)
{
	_data[0][0] = x(0); _data[0][1] = y(0); _data[0][2] = z(0);
	_data[1][0] = x(1); _data[1][1] = y(1); _data[1][2] = z(1);
	_data[2][0] = x(2); _data[2][1] = y(2); _data[2][2] = z(2);
}

float Matrix33::operator() (unsigned int row, unsigned int col) const
{
	return _data[row][col];
}


//Matrix multiplication
const Matrix33 Matrix33::operator*(const Matrix33& rhs) const
{
	Matrix33 m;
	unsigned int row, col, i;
	for (row = 0; row < 3; ++row)
	{
		for (col = 0; col < 3; ++col)
		{
			float& elem = m._data[row][col];
			elem = 0.f;
			for (i = 0; i < 3; ++i)
			{
				elem += _data[row][i] * rhs._data[i][col];
			}
		}
	}
	return m;
}

void Matrix33::operator=(const Matrix33& m)
{
	unsigned int row, col;
	for (row = 0; row < 3; ++row)
	{
		for (col = 0; col < 3; ++col)
		{
			_data[row][col] = m._data[row][col];
		}
	}
}

bool Matrix33::operator==(const Matrix33& m) const
{
	unsigned int row, col;
	for (row = 0; row < 3; ++row)
	{
		for (col = 0; col < 3; ++col)
		{
			if (_data[row,col] - m._data[row,col] > std::numeric_limits<float>::epsilon() )
				return false; // floats don't match
		}
	}
	return true; // all values matched.
}

/****************************************************************************************************/

//Matrix stream output
std::ostream& operator<<(std::ostream& os, const Matrix33& m)
{
	os << "[" << m(0,0) << ", " << m(0,1) << ", " << m(0,1) << "]" << std::endl; 
	os << "[" << m(1,0) << ", " << m(1,1) << ", " << m(1,2) << "]" << std::endl; 
	os << "[" << m(2,0) << ", " << m(2,1) << ", " << m(2,2) << "]"; 
	return os;
}

