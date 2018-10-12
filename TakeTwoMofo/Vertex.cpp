#include "StdAfx.h"
#include "Vertex.h"

using namespace TakeOne;

Vertex::Vertex(Vector& pos)
{
	_vec = pos;
}
Vertex::Vertex()
{
	
}
Vertex::~Vertex(void)
{
}
void Vertex::SaveZ(float oneOverDepth)
{
	_savedZ = _vec._z * oneOverDepth;
}
void Vertex::Dehomogenize()
{
	_vec.DeHomogenize();
}
