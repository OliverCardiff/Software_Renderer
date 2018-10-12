#include "stdafx.h"
#include "Matrix.h"
#include "Vector.h"

using namespace TakeOne;

#define M_HEIGHT 4
#define M_WIDTH 4
#define ACCURACY 0.00001

//default constructor
Matrix::Matrix(void)
{
	_mResult = new Vector();
	SetToIdentity(*this);
}
//floats -> matrix constructor
Matrix::Matrix(float m00, float m10, float m20, float m30,
			   float m01, float m11, float m21, float m31,
			   float m02, float m12, float m22, float m32,
			   float m03, float m13, float m23, float m33)
{
	_mResult = new Vector();
	_m[0][0] = m00; _m[1][0] = m10; _m[2][0] = m20; _m[3][0] = m30; 
	_m[0][1] = m01; _m[1][1] = m11; _m[2][1] = m21; _m[3][1] = m31; 
	_m[0][2] = m02; _m[1][2] = m12; _m[2][2] = m22; _m[3][2] = m32; 
	_m[0][3] = m03; _m[1][3] = m13; _m[2][3] = m23; _m[3][3] = m33; 
}
//copy constructor
Matrix::Matrix(const Matrix& other)
{
	_mResult = new Vector();
	for(int i = 0; i < M_HEIGHT; i++)
	{
		for(int j = 0; j < M_WIDTH; j++)
		{
			_m[i][j] = other._m[i][j];
		}
	}
}

//Destructor
Matrix::~Matrix(void)
{
	delete _mResult;
}

bool Matrix::Equals(const Matrix& m0) const
{
	bool match = true;

	for(int i = 0; i < M_HEIGHT; i++)
	{
		for(int j = 0; j < M_WIDTH; j++)
		{
			if(!FloatEquals(_m[i][j], m0._m[i][j]))
			{
				match = false;
			}

		}
	}
	return match;
}

bool Matrix::FloatEquals(const float lhs, const float rhs) const
{
	assert(sizeof(float) == sizeof(int));
	if(lhs == rhs)
		return true;
	int intDiff = abs(*(int*)&lhs - *(int*)&rhs);
	if(intDiff <= 1)
		return true;
	return false;
	
}

//Operators
Matrix Matrix::operator +(Matrix & rhs)
{
	Matrix result;
	
	Add(*this, rhs, result);

	return result;

}

Matrix Matrix::operator -(Matrix & rhs)
{
	Matrix result;
	
	Subtract(*this, rhs, result);

	return result;

}

Matrix Matrix::operator *(Matrix & rhs)
{
	Matrix result;
	
	Multiply(*this, rhs, result);

	return result;

}

Matrix Matrix::operator *(float rhs)
{
	Matrix result;
	
	Multiply(*this, rhs, result);

	return result;

}
Vector Matrix::operator *(Vector & rhs)
{
	Vector v = rhs;
	Transform(*this, v);
	return v;
}

Matrix& Matrix::operator =(const Matrix &rhs)
{
	for(int i = 0; i < M_HEIGHT; i++)
	{
		for(int j = 0; j < M_WIDTH; j++)
		{
			_m[i][j] = rhs._m[i][j];
		}
	}
	return *this;
}
// ---static stuff time!--- //

void Matrix::Add(const Matrix& m0, const Matrix& m1, Matrix& mResult)
{
	for(int i = 0; i < M_HEIGHT; i++)
	{
		for(int j = 0; j < M_WIDTH; j++)
		{
			mResult._m[i][j] = m0._m[i][j] + m1._m[i][j];
		}
	}
}

//Subracts one matrix from another
void Matrix::Subtract(const Matrix & m0, const Matrix & m1, Matrix & mResult)
{
	for(int i = 0; i < M_HEIGHT; i++)
	{
		for(int j = 0; j < M_WIDTH; j++)
		{
			mResult._m[i][j] = m0._m[i][j] - m1._m[i][j];
		}
	}
}

//Multiplies two matrices together
void Matrix::Multiply(const Matrix & m0, const Matrix & m1, Matrix & mResult)
{
	for(int i = 0; i < M_HEIGHT; i++)
	{
		for(int j = 0; j < M_WIDTH; j++)
		{
			mResult._m[i][j] = (m0._m[0][j] * m1._m[i][0]) + (m0._m[1][j] * m1._m[i][1]) + (m0._m[2][j] * m1._m[i][2]) + (m0._m[3][j] * m1._m[i][3]);
		}
	}
}

//Multiplies a matrix with a scalar
void Matrix::Multiply(const Matrix & m0, float scalar, Matrix & mResult)
{
	for(int i = 0; i < M_HEIGHT; i++)
	{
		for(int j = 0; j < M_WIDTH; j++)
		{
			mResult._m[i][j] = m0._m[i][j] * scalar;
		}
	}
}

//Applies a transformation matrix to a 4x1 Vector, before Homogenizing it
void Matrix::Transform(const Matrix & m0, Vector & v0)
{
	m0._mResult->_x = m0._m[0][0] * v0._x + m0._m[1][0] * v0._y + m0._m[2][0] * v0._z + m0._m[3][0] * v0._w;
	m0._mResult->_y = m0._m[0][1] * v0._x + m0._m[1][1] * v0._y + m0._m[2][1] * v0._z + m0._m[3][1] * v0._w;
	m0._mResult->_z = m0._m[0][2] * v0._x + m0._m[1][2] * v0._y + m0._m[2][2] * v0._z + m0._m[3][2] * v0._w;
	m0._mResult->_w = m0._m[0][3] * v0._x + m0._m[1][3] * v0._y + m0._m[2][3] * v0._z + m0._m[3][3] * v0._w;

	//_mResult.DeHomogenize();

	v0 = *m0._mResult;
}

//Transformation matrix creation


//Creates Scaling Matrix
void Matrix::MakeScaling(const float x, const float y, const float z, Matrix & result, bool IDFirst)
{
	if(IDFirst)
	{
		SetToIdentity(result);
	}
	
	result._m[0][0] = x;
	result._m[1][1] = y;
	result._m[2][2] = z;
		
}
//Creates a translation matrix
void Matrix::MakeTranslation(const float x, const float y, const float z, Matrix & result, bool IDFirst)
{
	if(IDFirst)
	{
		SetToIdentity(result);
	}

	result._m[3][0] = x;
	result._m[3][1] = y;
	result._m[3][2] = z;

}
//Creates a Rotation matrix
void Matrix::MakeRotation(const Dimension dim, const float angle, Matrix & result, bool IDFirst)
{
	if(IDFirst)
	{
		SetToIdentity(result);
	}

	float sine = sin(angle);
	float cosine = cos(angle);

	switch(dim)
	{
	case X:
		result._m[1][1] = cosine; result._m[2][1] = sine;
		result._m[1][2] = -sine; result._m[2][2] = cosine;
		break;
	case Y:
		result._m[0][0] = cosine; result._m[2][0] = sine;
		result._m[0][2] = -sine; result._m[2][2] = cosine;
		break;
	case Z:
		result._m[0][0] = cosine; result._m[1][0] = sine;
		result._m[0][1] = -sine; result._m[1][1] = cosine;
		break;
	default:
		break;
	};

}

void Matrix::SetToIdentity(Matrix& m)
{
	for(int i = 0; i < M_HEIGHT; i++)
	{
		for(int j = 0; j < M_WIDTH; j++)
		{
			if(i != j)
			{
				m._m[i][j] = 0;
			}
			else 
			{
				m._m[i][j] = 1;
			}
		}
	}
}
