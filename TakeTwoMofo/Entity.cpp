#include "StdAfx.h"
#include "Entity.h"

using namespace TakeOne;

Entity::Entity(Vector position)
{
}

Entity::~Entity(void)
{
}
void Entity::Update(float time)
{
	_control->Feed(_physObj->GetLocation(), _physObj->GetState());
	_physObj->Feed(_control->GetVChange(), _flag1, _flag2);

	_model->Animate(time, _control->GetAnimationType());
	_model->Update(time);
}

void Entity::Draw(TakeOne::Rasterizer *_r)
{
	_model->_translation = _physObj->GetTranslation();
	_model->_rotation = _control->GetRotation();
	

	_model->Draw(_r);
}