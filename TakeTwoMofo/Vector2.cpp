#include "stdafx.h"

using namespace TakeOne;

// default constructor
Vector2::Vector2(void)
{
	_x = _y = 0;
}

// parameterised constructor
Vector2::Vector2(float x, float y)
{
	_x = x;
	_y = y;
}

// copy constructor
Vector2::Vector2(const Vector2& rhs)
{
	_x = rhs._x;
	_y = rhs._y;
}

// destructor
Vector2::~Vector2(void)
{
	// do nothing
}

// assignment operator
Vector2& Vector2::operator=(const Vector2& rhs)
{	
	_x = rhs._x;
	_y = rhs._y;

	return *this;
}

Vector2 Vector2::operator+(const Vector2& rhs)
{
	Vector2 v = *this;
	v.Add(rhs);

	return v;
}

Vector2 Vector2::operator-(const Vector2& rhs)
{
	Vector2 v = *this;
	v.Subtract(rhs);

	return v;
}
Vector2 Vector2::operator*(const float rhs)
{
	Vector2 v = *this;
	v.Multiply(rhs);

	return v;
}
Vector2 Vector2::operator/(const float rhs)
{
	Vector2 v = *this;
	v.Divide(rhs);

	return v;
}
// ARITHMETIC OPERATIONS //------------------------------

Vector2& Vector2::Add(const Vector2& rhs)
{
	_x += rhs._x;
	_y += rhs._y;
	//adds the two Vector2s together and returns a new one
	return *this;
}

Vector2& Vector2::Subtract(const Vector2& rhs)
{
	_x -= rhs._x;
	_y -= rhs._y;
	return *this;
}

Vector2& Vector2::Multiply(const float scalar)
{
	_x *= scalar;
	_y *= scalar;

	return *this;
}

Vector2& Vector2::Divide(const float scalar)
{
	_x /= scalar;
	_y /= scalar;

	return *this;
}

// END ARITHMETIC OPERATIONS //--------------------------

// OTHER OPERATIONSN //----------------------------------

float Vector2::Magnitude()
{
	return sqrt(_x * _x + _y * _y);
}

void  Vector2::MakeUnit()
{
	float total = this->Magnitude();

	_x /= total;
	_y /= total;
}

float Vector2::DotProduct(Vector2& v)
{
	return (_x * v._x) + (_y * v._y);
}

