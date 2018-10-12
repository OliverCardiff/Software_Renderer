#pragma once
#include "StdAfx.h"
#include "Model.h"
#include "Bone.h"

using namespace TakeOne;

Model::Model(void) 
{
}

Model::~Model(void)
{
	std::list<Bone*>::iterator i = _bones.begin();

	for(;i != _bones.end(); i++)
	{
		delete (*i);
	}
}
void Model::BuildBoneNormals()
{
	std::list<Bone*>::iterator i = _bones.begin();

	for(;i != _bones.end(); i++)
	{
		(*i)->RefreshNormals();
	}
}

void Model::Draw(TakeOne::Rasterizer *_r)
{	
	Matrix::MakeTranslation(_wrldOffset._x, _wrldOffset._y, _wrldOffset._z, _temp, false);
	_translation = _translation * _temp;
	AssembleTransforms();

	std::list<Bone*>::iterator i = _bones.begin();

	for(;i != _bones.end(); i++)
	{
		(*i)->AssembleTransforms();
		(*i)->_mainRotation  = _mainRotation * (*i)->_mainRotation;
		(*i)->_transform = _transform * (*i)->_transform;
		(*i)->Draw(_r);
	}
}

void Model::Update(float time)
{
	std::list<Bone*>::iterator i = _bones.begin();

	for(;i != _bones.end(); i++)
	{
		(*i)->Update(time);
	}
}

void Model::Animate(float time, int type)
{

}

