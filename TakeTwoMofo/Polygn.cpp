#include "StdAfx.h"
#include "Polygn.h"

using namespace TakeOne;

Polygn::Polygn(void)
{
	wireFrame = false;
}

Polygn::~Polygn(void)
{
}

Vector Polygn::MakeNormal()
{
	Vector start = _indices[0]->_vec - _indices[1]->_vec;
	Vector end = _indices[2]->_vec - _indices[1]->_vec;

	return end.CrossProduct(start);
}
void Polygn::AvgZ()
{
	_avgZ = ( _indices[0]->_savedZ + _indices[1]->_savedZ + _indices[2]->_savedZ) /3;
}
bool Polygn::operator <(const Polygn &rhs)
{
	if(_avgZ < rhs._avgZ)
	{
		return true;
	}
	return false;
}
bool Polygn::operator >(const Polygn &rhs)
{
	if(_avgZ > rhs._avgZ)
	{
		return true;
	}
	return false;
}
bool Polygn::Compare(TakeOne::Polygn *one, TakeOne::Polygn *two)
{
	if(one->_avgZ > two->_avgZ)
	{
		return true;
	}
	return false;
}