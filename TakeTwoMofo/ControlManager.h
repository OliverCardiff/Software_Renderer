#pragma once
#include "InputHandler.h"
#include "Controller.h"

namespace TakeOne
{
	class ControlManager
	{
	public:
		ControlManager(void);
		~ControlManager(void);
	public:
		void Update(float time, InputHandler * IH);
		std::list<Controller*> _players;
		std::list<Controller*> _AI;
		void Submit(Controller* c, bool player);
	};
}