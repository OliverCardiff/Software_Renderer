#include "stdafx.h"

using namespace TakeOne;

// default constructor
Vector::Vector(void)
{
	_x = _y = _z = 0;
	_w = 1;

}

// parameterised constructor
Vector::Vector(float x, float y , float z)
{
	_x = x;
	_y = y;
	_z = z;
	_w = 1;

}

// copy constructor
Vector::Vector(const Vector& rhs)
{
	_x = rhs._x;
	_y = rhs._y;
	_z = rhs._z;
	_w = rhs._w;
}

// destructor
Vector::~Vector(void)
{
	// do nothing
}

// assignment operator
Vector& Vector::operator=(const Vector& rhs)
{	
	_x = rhs._x;
	_y = rhs._y;
	_z = rhs._z;
	_w = rhs._w;

	return *this;
}

Vector Vector::operator+(const Vector& rhs)
{
	Vector v = *this;
	v.Add(rhs);

	return v;
}

Vector Vector::operator-(const Vector& rhs)
{
	Vector v = *this;
	v.Subtract(rhs);

	return v;
}
Vector Vector::operator*(const float rhs)
{
	Vector v = *this;
	v.Multiply(rhs);

	return v;
}
Vector Vector::operator/(const float rhs)
{
	Vector v = *this;
	v.Divide(rhs);

	return v;
}
// ARITHMETIC OPERATIONS //------------------------------

Vector Vector::Lerp(Vector& one, Vector& two, float val)
{
	Vector result;

	result = ((one * val) + (two * (1-val)));
	
	return result;
}

void Vector::Lerp(Vector& out, Vector& one, Vector& two, float val)
{
	out = ((one * val) + (two * (1-val)));
}
Vector& Vector::Add(const Vector& rhs)
{
	_x += rhs._x;
	_y += rhs._y;
	_z += rhs._z;
	//adds the two vectors together and returns a new one
	return *this;
}

Vector& Vector::Subtract(const Vector& rhs)
{
	_x -= rhs._x;
	_y -= rhs._y;
	_z -= rhs._z;
	return *this;
}

Vector& Vector::Multiply(const float scalar)
{
	_x *= scalar;
	_y *= scalar;
	_z *= scalar;

	return *this;
}

Vector& Vector::Divide(const float scalar)
{
	_x /= scalar;
	_y /= scalar;
	_z /= scalar;

	return *this;
}

// END ARITHMETIC OPERATIONS //--------------------------

// OTHER OPERATIONSN //----------------------------------

float Vector::Magnitude()
{
	return sqrt((_x * _x) + (_y * _y) + (_z * _z));
}

void  Vector::MakeUnit()
{
	float total = this->Magnitude();

	if(total != 0)
	{
		_x /= total;
		_y /= total;
		_z /= total;
	}
}

float Vector::DotProduct(Vector& v)
{
	
	return (_x * v._x) + (_y * v._y) + (_z * v._z);
}

Vector Vector::CrossProduct(Vector& v)
{
	float newX = _y * v._z - _z * v._y;
	float newY = _z * v._x - _x * v._z;
	float newZ = _x * v._y - _y * v._x;

	return Vector(newX, newY, newZ);
}

void Vector::DeHomogenize()
{
	_x /= _w;
	_y /= _w;
	_z /= _w;
	_w = 1;
}
// END OTHER OPERATIONS //--------------------------------

// print
std::ostream& operator<<(std::ostream& out, const Vector& vec)
{
	return out << "<" << vec._x << ", " << vec._y << ", " << vec._z << ">" << std::endl;
}