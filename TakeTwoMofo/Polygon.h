#pragma once
#include "Vector.h"

namespace TakeOne
{
	class Polygon
	{
	public:
		Polygon(void);
		~Polygon(void);
	public:
		short _indices[3];
		Vector MakeNormal( Vector& a,  Vector& b, Vector& c);
	};
}
