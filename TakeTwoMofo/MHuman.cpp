#include "StdAfx.h"
#include "MHuman.h"

using namespace TakeOne;

const float LEG_MAX_ANGLE = PI/6;
const float LEG2_OFFSET = -PI/4;
const float SPEED_MOD_LEG = 5;
const float LEG2_ANGLE_MOD = PI/5.6f;
const float LEG2_MAX_BOOST = PI/6;

const float ARM_MAX_ANGLE = PI/6;
const float ARM2_OFFSET = 0;
const float SPEED_MOD_ARM = 5;
const float ARM2_ANGLE_MOD = -PI/3;
const float ARM2_MAX_BOOST = PI/6;



MHuman::MHuman(void)
{
	_timer = 0;
	BuildHuman();
	_wrldOffset._y = 47;
}

MHuman::~MHuman(void)
{
}

void MHuman::Animate(float time, int type)
{
	if(type == 1)
	{
		_timer += time/1000;

		LimbMove(LEG_MAX_ANGLE, 0, LEG2_OFFSET, LEG2_ANGLE_MOD,LEG2_MAX_BOOST, SPEED_MOD_LEG, _leg1a, _leg1b, _timer);
		LimbMove(LEG_MAX_ANGLE, PI, LEG2_OFFSET, LEG2_ANGLE_MOD,LEG2_MAX_BOOST, SPEED_MOD_LEG, _leg2a, _leg2b, _timer);

		LimbMove(ARM_MAX_ANGLE, PI,ARM2_OFFSET, ARM2_ANGLE_MOD,ARM2_MAX_BOOST, SPEED_MOD_ARM, _arm1a, _arm1b, _timer);
		LimbMove(ARM_MAX_ANGLE, 0, ARM2_OFFSET, ARM2_ANGLE_MOD,ARM2_MAX_BOOST, SPEED_MOD_ARM, _arm2a, _arm2b, _timer);

		_arm1a->RotateXPos();
		_arm2a->RotateXPos();

		TranslateXZ(*_arm1a->_relPos, *_arm1b->_relPos, _arm1a->_relXRot, _arm1a->_length);
		TranslateXZ(*_arm2a->_relPos, *_arm2b->_relPos, _arm2a->_relXRot, _arm2a->_length);

		_arm1b->TranslatePos();
		_arm2b->TranslatePos();

		_arm1b->RotateXPos();
		_arm2b->RotateXPos();
	}

	if(type == 2)
	{
		KeepStill();
	}
}

void MHuman::KeepStill()
{
	Matrix::MakeTranslation(0,25,-7, _arm1a->_translation, false);
	Matrix::MakeRotation(X, -PI/6, _arm1a->_rotation, true);
	
	_arm1b->_relPos->_y = 25 - _arm1a->_length * cos(-PI/6);
	_arm1b->_relPos->_z = -7 + _arm1a->_length * sin(-PI/6);
	Matrix::MakeTranslation(0,_arm1b->_relPos->_y, _arm1b->_relPos->_z, _arm1b->_translation, false);
	Matrix::MakeRotation(X, PI/8, _arm1b->_rotation, true);
	
	
	Matrix::MakeTranslation(0,25,7, _arm2a->_translation, false);
	Matrix::MakeRotation(X, PI/6, _arm2a->_rotation, true);
	
	_arm2b->_relPos->_y = 25 - _arm2a->_length * cos(PI/6);
	_arm2b->_relPos->_z = 7 + _arm2a->_length * sin(PI/6);
	Matrix::MakeTranslation(0,_arm2b->_relPos->_y, _arm2b->_relPos->_z, _arm2b->_translation, false);
	Matrix::MakeRotation(X, -PI/8, _arm2b->_rotation, true);
	

}
void MHuman::LimbMove(float maxAngle, float wholeOffset, float secondOffset, float angleMod, float maxBoost, float speed, TakeOne::Bone *a, TakeOne::Bone *b, float time)
{
	float firstAngle = sin((time * speed) +  wholeOffset) * maxAngle;
	float secondAngle = (sin((time * speed) + wholeOffset + secondOffset) * (maxAngle + maxBoost)) + angleMod;
	
	Matrix::MakeRotation(Z, firstAngle, a->_rotation, true);
	Matrix::MakeRotation(Z, secondAngle, b->_rotation, true);

	float x = a->_relPos->_x - sin(firstAngle) * a->_length;
	float y = a->_relPos->_y - cos(firstAngle) * a->_length;
	float z = a->_relPos->_z;

	Matrix::MakeTranslation(x,y,z, b->_translation, false);

	b->_relPos->_x = x;
	b->_relPos->_y = y;
	b->_relPos->_z = z;

}
void MHuman::TranslateXZ(Vector &first, Vector& second, float angle, float length)
{
	Vector t;

	t._y = first._y - length * cos(angle);
	t._z = first._z + length * sin(angle);

	second._x = 0;
	second._y = 1.4f;
	second._z = t._z - second._z;
}
void MHuman::BuildHuman()
{
	BuildFragment(_head, 1.5f, 7, 5, 7);
	_head->_relPos = new Vector(0, 40, 0);
	
	BuildFragment(_torso, 3, 24, 5, 14);
	_torso->_relPos = new Vector(0,30,0);

	BuildFragment(_leg1a, 7,7, 5,5);
	_leg1a->_relPos = new Vector(0, 1.5f,  -3.5);
	
	BuildFragment(_leg1b, 7,7, 5,5);
	_leg1b->_relPos = new Vector(0, -19.5f,  -3.5);

	BuildFragment(_leg2a, 7,7, 5,5);
	_leg2a->_relPos = new Vector(0, 1.5f,  3.5);

	BuildFragment(_leg2b, 7,7, 5,5);
	_leg2b->_relPos = new Vector(0, -19.5f,  3.5);
	

	BuildFragment(_arm1a, 5,5,4,4);
	_arm1a->_relPos = new Vector(0, 25, -7);
	_arm1a->_relXRot = -PI/6;
	_arm1a->TranslatePos();
	_arm1a->RotateXPos();


	BuildFragment(_arm1b, 5,5,4,4);
	_arm1b->_relPos = new Vector(0, 0, 0);
	_arm1b->_relPos->_y = _arm1a->_relPos->_y - _arm1a->_length * cos(_arm1a->_relXRot);
	_arm1b->_relPos->_z = _arm1a->_relPos->_z + _arm1a->_length * sin(_arm1a->_relXRot);
	_arm1b->TranslatePos();
	_arm1b->_relXRot = PI/8;
	_arm1b->RotateXPos();

	BuildFragment(_arm2a, 5,5,4,4);
	_arm2a->_relPos = new Vector(0, 25, 7);
	_arm2a->_relXRot = PI/6;
	_arm2a->TranslatePos();
	_arm2a->RotateXPos();

	BuildFragment(_arm2b, 5,5,4,4);
	_arm2b->_relPos = new Vector(0, 0, 0);
	_arm2b->_relPos->_y = _arm2a->_relPos->_y - _arm2a->_length * cos(_arm2a->_relXRot);
	_arm2b->_relPos->_z = _arm2a->_relPos->_z + _arm2a->_length * sin(_arm2a->_relXRot);
	_arm2b->TranslatePos();
	_arm2b->_relXRot = -PI/8;
	_arm2b->RotateXPos();

	BuildBoneNormals();

	_head->TranslatePos();
	_leg2b->TranslatePos();
	_leg2a->TranslatePos();
	_leg1a->TranslatePos();
	_leg1b->TranslatePos();
	_torso->TranslatePos();

}

void MHuman::BuildFragment(Bone*& b, float poleHeight, float boxHeight, float sqX, float sqZ)
{
	b = new Bone(16, 10);
	b->_length = boxHeight + (poleHeight * 2);

	b->_transVerts[0]._vec = b->_verts[0]._vec = Vector(sqX/2, -poleHeight, -sqZ/2);
	b->_transVerts[1]._vec = b->_verts[1]._vec = Vector(-sqX/2, -poleHeight, -sqZ/2);
	b->_transVerts[2]._vec = b->_verts[2]._vec = Vector(-sqX/2, -poleHeight, sqZ/2);
	b->_transVerts[3]._vec = b->_verts[3]._vec = Vector(sqX/2, -poleHeight, sqZ/2);
	b->_transVerts[4]._vec = b->_verts[4]._vec = Vector(0, 0, 0);
	b->_transVerts[5]._vec = b->_verts[5]._vec = Vector(sqX/2, -poleHeight-boxHeight, -sqZ/2);
	b->_transVerts[6]._vec = b->_verts[6]._vec = Vector(-sqX/2, -poleHeight-boxHeight, -sqZ/2);
	b->_transVerts[7]._vec = b->_verts[7]._vec = Vector(-sqX/2, -poleHeight-boxHeight, sqZ/2);
	b->_transVerts[8]._vec = b->_verts[8]._vec = Vector(sqX/2, -poleHeight-boxHeight, sqZ/2);
	b->_transVerts[9]._vec = b->_verts[9]._vec = Vector(0, -boxHeight-(poleHeight*2), 0);
	
	//top
	b->_polys[0]._indices[0] = &b->_transVerts[0];
	b->_polys[0]._indices[1] = &b->_transVerts[4];
	b->_polys[0]._indices[2] = &b->_transVerts[3];
	 
	b->_polys[1]._indices[0] = &b->_transVerts[3];
	b->_polys[1]._indices[1] = &b->_transVerts[4];
	b->_polys[1]._indices[2] = &b->_transVerts[2];

	b->_polys[2]._indices[0] = &b->_transVerts[2];
	b->_polys[2]._indices[1] = &b->_transVerts[4];
	b->_polys[2]._indices[2] = &b->_transVerts[1];

	b->_polys[3]._indices[0] = &b->_transVerts[1];
	b->_polys[3]._indices[1] = &b->_transVerts[4];
	b->_polys[3]._indices[2] = &b->_transVerts[0];

	//middle
	b->_polys[4]._indices[0] = &b->_transVerts[5];
	b->_polys[4]._indices[1] = &b->_transVerts[0];
	b->_polys[4]._indices[2] = &b->_transVerts[3];

	b->_polys[5]._indices[0] = &b->_transVerts[5];
	b->_polys[5]._indices[1] = &b->_transVerts[3];
	b->_polys[5]._indices[2] = &b->_transVerts[8];

	b->_polys[6]._indices[0] = &b->_transVerts[8];
	b->_polys[6]._indices[1] = &b->_transVerts[3];
	b->_polys[6]._indices[2] = &b->_transVerts[2];

	b->_polys[7]._indices[0] = &b->_transVerts[8];
	b->_polys[7]._indices[1] = &b->_transVerts[2];
	b->_polys[7]._indices[2] = &b->_transVerts[7];

	b->_polys[8]._indices[0] = &b->_transVerts[7];
	b->_polys[8]._indices[1] = &b->_transVerts[2];
	b->_polys[8]._indices[2] = &b->_transVerts[1];

	b->_polys[9]._indices[0] = &b->_transVerts[7];
	b->_polys[9]._indices[1] = &b->_transVerts[1];
	b->_polys[9]._indices[2] = &b->_transVerts[6];

	b->_polys[10]._indices[0] = &b->_transVerts[6];
	b->_polys[10]._indices[1] = &b->_transVerts[1];
	b->_polys[10]._indices[2] = &b->_transVerts[0];
	
	b->_polys[11]._indices[0] = &b->_transVerts[6];
	b->_polys[11]._indices[1] = &b->_transVerts[0];
	b->_polys[11]._indices[2] = &b->_transVerts[5];

	//bottom
	b->_polys[12]._indices[0] = &b->_transVerts[8];
	b->_polys[12]._indices[1] = &b->_transVerts[9];
	b->_polys[12]._indices[2] = &b->_transVerts[5];

	b->_polys[13]._indices[0] = &b->_transVerts[7];
	b->_polys[13]._indices[1] = &b->_transVerts[9];
	b->_polys[13]._indices[2] = &b->_transVerts[8];

	b->_polys[14]._indices[0] = &b->_transVerts[6];
	b->_polys[14]._indices[1] = &b->_transVerts[9];
	b->_polys[14]._indices[2] = &b->_transVerts[7];

	b->_polys[15]._indices[0] = &b->_transVerts[5];
	b->_polys[15]._indices[1] = &b->_transVerts[9];
	b->_polys[15]._indices[2] = &b->_transVerts[6];
	
	for(int i = 0;i < 16; i++)
	{
		b->_polys[i].textured = false;
	}

	_bones.push_back(b);
	 
}