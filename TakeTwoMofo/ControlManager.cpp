#include "StdAfx.h"
#include "ControlManager.h"

using namespace TakeOne;

ControlManager::ControlManager(void)
{
}

ControlManager::~ControlManager(void)
{
}

void ControlManager::Update(float time, TakeOne::InputHandler *IH)
{
	std::list<Controller*>::iterator i = _players.begin();

	for(;i != _players.end(); i++)
	{
		(*i)->Update(time, IH);
	}

	std::list<Controller*>::iterator j = _AI.begin();

	for(;j != _AI.end(); j++)
	{
		(*j)->Update(time, IH);
	}
}

void ControlManager::Submit(TakeOne::Controller *c, bool player)
{
	if(player)
	{
		_players.push_back(c);
	}
	else
	{
		_AI.push_back(c);
	}
}
