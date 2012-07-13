
#include "Vector3.h"
#include "Matrix33.h"

Vector3::Vector3() 
{

}

Vector3::Vector3(float x, float y, float z) 
{ 
	_data[0] = x; 
	_data[1] = y; 
	_data[2] = z; 
}
		
float& Vector3::operator() (unsigned int i)
{
	return _data[i];
}

const float& Vector3::operator() (unsigned int i) const
{
	return _data[i];
}


//Vector equality operator
bool Vector3::operator==(const Vector3& rhs) const 
{ 
	for (unsigned int i = 0; i < 3; ++i)
	{
		if ( _data[i] - rhs._data[i] > std::numeric_limits<float>::epsilon() )
			return false; //mismatching float values
	}
	return true;
} 

//Vector inequality operator
bool Vector3::operator!=(const Vector3& rhs) const 
{ 
	return !(*this == rhs);
}

//Multiplying two vectors will perform a dot/inner product
float Vector3::operator*(const Vector3& rhs) const
{
	float f = 0.f;
	for (unsigned int i = 0; i < 3; ++i)
		f += _data[i] * rhs._data[i];
	return f;
}


Vector3 Vector3::operator*(const Matrix33& rhs)
{
	Vector3 v;
	unsigned col, i;
	for (col = 0; col < 3; ++col)
	{
		float& f = v(col);
		f = 0.f;
		for (i = 0; i < 3; ++i)
		{
			f += _data[i] * rhs(i,col);
		}
	}
	return v;
}

Vector3 Vector3::operator*(float rhs) const
{
	return Vector3(_data[0] * rhs, _data[1] * rhs, _data[2] * rhs );
}

//Vector-scalar division
Vector3 Vector3::operator/(float rhs) const
{
	return Vector3(_data[0]/rhs, _data[1]/rhs, _data[2]/rhs );
}


//vector addition operator
Vector3 Vector3::operator+(const Vector3& rhs) const
{
	return Vector3(_data[0] + rhs._data[0], _data[1] + rhs._data[1], _data[2] + rhs._data[2] );
}

//Vector subtraction operator
Vector3 Vector3::operator-(const Vector3 rhs) const
{
	return Vector3(_data[0] - rhs._data[0], _data[1] - rhs._data[1], _data[2] - rhs._data[2] );
}


//Vector assignment
void Vector3::operator=(const Vector3& v)
{
	_data[0] = v._data[0];
	_data[1] = v._data[1];
	_data[2] = v._data[2];
}

/****************************************************************************************************/

//Scalar-vector multiplication
Vector3 operator*(float lhs, const Vector3& rhs)
{
	return rhs*lhs;
}

/****************************************************************************************************/

std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
	os << "[" << v(0) << ", " << v(1) << ", " << v(2) << "]"; 
	return os;
}

/****************************************************************************************************/

