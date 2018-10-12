#pragma once
#include <iostream>  // this is included only for std::ostream in order to implement operator<<()

namespace TakeOne
{
	class Vector
	{
		// object management
	public:
		Vector(void);							// default constructor
		Vector(float x, float y, float z);		// parameterised constructor
		Vector(const Vector& rhs);				// copy constructor
		~Vector(void);							// destructor

		// operators
	public:
		Vector& operator=(const Vector& rhs); // assignment operator
		Vector operator-(const Vector& rhs);	
		Vector operator+(const Vector& rhs);
		Vector operator/(const float rhs);
		Vector operator*(const float rhs);

		// arithmetic operations
	public:
		Vector& Add(const Vector& rhs);
		Vector& Subtract(const Vector& rhs);
		Vector& Multiply(const float scalar);
		Vector& Divide(const float scalar);

		// other operations
	public:
		float  Magnitude();
		void   MakeUnit();
		void   DeHomogenize();
		float  DotProduct(Vector& rhs);
		Vector CrossProduct(Vector& rhs);
		static void Lerp(Vector& out, Vector& one, Vector& two, float val);
		static Vector Lerp(Vector& one, Vector& two, float val);

	public:
		// output operator implemented for printing
		friend std::ostream& operator<<(std::ostream&, const Vector&);

		// components describing this vector
	public:
		float _x;
		float _y;
		float _z;
		float _w;
	};

}


