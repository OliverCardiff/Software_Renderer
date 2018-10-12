#include "StdAfx.h"
#include "PlayerPhys.h"

using namespace TakeOne;
const float GRAVITY = 10;

PlayerPhys::PlayerPhys(void)
{
}

PlayerPhys::~PlayerPhys(void)
{
}

void PlayerPhys::Feed(Vector& vChange, bool flag1, bool flag2) 
{
	if(vChange._y > 1)
	{
		_velocity._y = vChange._y;
	}
	_velocity._x = vChange._x;
	_velocity._z = vChange._z;	


}
PhysicalState PlayerPhys::GetState() 
{
	return _phys;
}
Matrix& PlayerPhys::GetTranslation() 
{
	Matrix::MakeTranslation(_relPos->_x, _relPos->_y, _relPos->_z, _translation, false);
	return _translation;
}
Vector& PlayerPhys::GetLocation() 
{
	return *_relPos;
}
void PlayerPhys::Update(float time) 
{
	if(_velocity._y <= -5) _phys = Falling;
	else if(_velocity.Magnitude() == 0) _phys = Still;
	else _phys = Running;

	_relPos->Add(_velocity);
	
	_velocity._y -= GRAVITY * (time/1000);


}