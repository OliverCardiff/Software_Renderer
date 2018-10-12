#pragma once
#include <iostream>  // this is included only for std::ostream in order to implement operator<<()

namespace TakeOne
{
	class Vector2
	{
		// object management
	public:
		Vector2(void);							// default constructor
		Vector2(float x, float y);		// parameterised constructor
		Vector2(const Vector2& rhs);				// copy constructor
		~Vector2(void);							// destructor

		// operators
	public:
		Vector2& operator=(const Vector2& rhs); // assignment operator
		Vector2 operator-(const Vector2& rhs);	
		Vector2 operator+(const Vector2& rhs);
		Vector2 operator/(const float rhs);
		Vector2 operator*(const float rhs);

		// arithmetic operations
	public:
		Vector2& Add(const Vector2& rhs);
		Vector2& Subtract(const Vector2& rhs);
		Vector2& Multiply(const float scalar);
		Vector2& Divide(const float scalar);

		// other operations
	public:
		float  Magnitude();
		void   MakeUnit();
		float  DotProduct(Vector2& rhs);

		// components describing this Vector2
	public:
		float _x;
		float _y;
	};

}


