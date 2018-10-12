#include "StdAfx.h"
#include "Player.h"
#include "MHuman.h"
#include "AIController.h"
#include "PlayerPhys.h"
#include "Camera.h"

using namespace TakeOne;

Player::Player(PhysicsManager * pm, ControlManager * cm, Vector position, Camera * cam, int ID) : Entity(position)
{
	_model = new MHuman();
	_model->_relPos = new Vector(position);
	_control = new AIController(cam,ID);
	_physObj = new PlayerPhys();
	Identity = ID;
	pm->Submit(_physObj, true);
	cm->Submit(_control, true);
	_physObj->_relPos = new Vector(position);
}

Player::~Player(void)
{
}
void Player::SetPosition(Vector p)
{
	_control->Feed(p, Running);
	*_physObj->_relPos = p;
}
