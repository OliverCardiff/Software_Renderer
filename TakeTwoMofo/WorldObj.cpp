#include "StdAfx.h"
#include "WorldObj.h"

using namespace TakeOne;

WorldObj::WorldObj(void)
{
	_final = new Matrix();
	_relPos = NULL;
}

WorldObj::~WorldObj(void)
{
	delete _relPos;
	delete _final;
}
float WorldObj::GetRand()
{
	return (((float) rand())/RAND_MAX - 0.5f) * 2.0f;
}
float WorldObj::GetURand()
{
	return ((float) rand())/RAND_MAX;
}
void WorldObj::AssembleTransforms()
{
	_transform = _translation * _rotation * _scaling;
	_mainRotation = _rotation;
}
void WorldObj::Draw(TakeOne::Rasterizer *_r)
{
}
void WorldObj::Update(float time)
{
}
void WorldObj::AssembleOnlyTranslate()
{
	_transform = _translation;
}