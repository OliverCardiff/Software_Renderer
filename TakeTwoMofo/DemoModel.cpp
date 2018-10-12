#include "StdAfx.h"
#include "DemoModel.h"
#include "MD2Loader.h"
#include "Bone.h"

using namespace TakeOne;

DemoModel::DemoModel(void)
{
	_demo = new Bone();
	//MD2Loader::LoadModel("triax_tracks.md2", *_demo);
	MD2Loader::LoadModel("body.md2", *_demo);

	for(int i = 0;i < _demo->_numVerts; i++)
	{
		_demo->_verts[i]._vec._y -= 25;
		_demo->_transVerts[i]._vec._y -= 25;
	}
	_demo->RefreshNormals();
	_demo->_info.textured = false;
	_bones.push_back(_demo);
	_rota = 0;
	_rotb = 0;
	_rotc = 0;
	Matrix::MakeScaling(4,4,4,_demo->_scaling, false);
}

DemoModel::~DemoModel(void)
{
}

void DemoModel::Animate(float time, int type)
{
	float ttime = (time/1000);
	Matrix temp, temp2, temp3;

	_rota += ROT_AMOUNT * ttime;
	_rotb += ROT_AMOUNT * ttime;
	_rotc += ROT_AMOUNT * ttime;

	Matrix::MakeRotation(X, _rota, temp, false);
	Matrix::MakeRotation(Y, _rotb, temp2, false);
	Matrix::MakeRotation(Z, _rotc, temp3, false);

	Matrix::Multiply(temp, temp2, _demo->_rotation);
	Matrix::Multiply(_demo->_rotation, temp3, temp);
	_demo->_rotation = temp;

}