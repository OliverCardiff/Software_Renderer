#pragma once
#include <ctime>
#include "StdAfx.h"
#include <cstdio>

namespace TakeOne
{
	class GameTime
	{
	public:
		GameTime(void);
		~GameTime(void);

	public:
		void Begin();
		float GetElapsedMS();
		float GetTotalMS();
		void Update();
		void Pause();
		void Restart();

	private://member functions
		float ToMS(float);

	private://member variables
		std::clock_t _start;
		float _elapsed;
		float _lastElapsed;
		float _total;
		bool _paused;
	};
}
