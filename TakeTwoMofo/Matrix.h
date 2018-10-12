#pragma once


namespace TakeOne
{
	class Vector;	// forward declaration

	enum Dimension { X, Y, Z };

	class Matrix
	{
	public: // constructors
		Matrix(void);
		Matrix(float, float, float, float,
			   float, float, float, float,
			   float, float, float, float,
			   float, float, float, float );
		Matrix(const Matrix&);

	public: // destructor
		~Matrix(void);

	public: // equality
		bool Equals(const Matrix&) const;

	public: //operators
		Matrix operator+	(Matrix &);
		Matrix operator-	(Matrix &);
		Matrix operator*	(Matrix &);
		Vector operator*	(Vector &);
		Matrix operator*	(float);
		Matrix& operator=	(const Matrix& rhs);	

	public: // arithmetic
		static void Add		 ( const Matrix&, const Matrix&, Matrix& );
		static void Subtract ( const Matrix&, const Matrix&, Matrix& );
		static void Multiply ( const Matrix&,		float,	 Matrix& );
		static void Multiply ( const Matrix&, const Matrix&, Matrix& );
		static void Transform( const Matrix&, Vector&);

	public: //Transformations
		static void MakeTranslation(const float, const float, const float, Matrix&, bool);
		static void MakeScaling(const float, const float, const float, Matrix&, bool);
		static void MakeRotation(const Dimension, const float, Matrix&, bool);
		static void SetToIdentity(Matrix& m);

	public: // output
		void OutputToConsole() const;

	public: // fields
		float _m[4][4];

	private: //float equals
		bool FloatEquals(const float, const float) const;
		Vector * _mResult;
	};
}
