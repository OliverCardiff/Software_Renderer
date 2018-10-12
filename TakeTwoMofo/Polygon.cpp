#include "StdAfx.h"
#include "Polygon.h"

using namespace TakeOne;

Polygon::Polygon(void)
{
}

Polygon::~Polygon(void)
{
}

Vector Polygon::MakeNormal( Vector& a,  Vector& b, Vector& c)
{
	Vector start = b - a;
	Vector end = c - b;
	return start.CrossProduct(end);
}
