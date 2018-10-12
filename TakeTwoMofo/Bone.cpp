#include "StdAfx.h"
#include "Bone.h"

using namespace TakeOne;

Bone::Bone()
{
	_info.worldMatrix = &_transform;
	_info.rotationOnly = &_mainRotation;
}

Bone::Bone(int polys, int verts)
{
	BuildArrays(polys, verts);

	_info.worldMatrix = &_transform;
	_info.rotationOnly = &_mainRotation;
	
}

Bone::~Bone(void)
{
	delete [] _polys;
	delete [] _verts;
	delete [] _transVerts;
}

void Bone::BuildArrays(int polys, int verts)
{
	_polys = new TakeOne::Polygn[polys];
	_verts = new TakeOne::Vertex[verts];
	_transVerts = new TakeOne::Vertex[verts];

	_numPolys = polys;
	_numVerts = verts;
	_info.numPolys = _numPolys;
	_info.numVerts = _numVerts;
	_info.polys = _polys;
	_info.vert = _verts;
	_info.transVerts = _transVerts;
}

void Bone::TranslatePos()
{
	Matrix tmp;
	Matrix::MakeTranslation(_relPos->_x, _relPos->_y, _relPos->_z, tmp, false);
	_translation = _translation * tmp;
}

void Bone::RotateXPos()
{
	Matrix tmp;
	Matrix::MakeRotation(X, _relXRot, tmp, false);
	_rotation = _rotation * tmp;
}

void Bone::RefreshNormals()
{
	if(_polys != NULL)
	{
		Vector nrm;

		for(int i = 0; i < _numPolys; i++)
		{
			nrm = _polys[i].MakeNormal();
			nrm.MakeUnit();
			_polys[i]._indices[0]->_normal.Add(nrm); 
			_polys[i]._indices[1]->_normal.Add(nrm); 
			_polys[i]._indices[2]->_normal.Add(nrm); 
		}

		for(int i = 0; i < _numVerts; i++)
		{
			_transVerts[i]._normal.MakeUnit();
			_verts[i]._normal = _transVerts[i]._normal;
		}
	}
}
void Bone::Draw(Rasterizer * _r)
{
	_r->SubmitToDraw(_info);
}
void Bone::Update(float time)
{

}