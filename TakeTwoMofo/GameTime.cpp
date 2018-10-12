#include "StdAfx.h"
#include "GameTime.h"

using namespace TakeOne;

GameTime::GameTime(void)
{
	_paused = false;
	_start = 0;
	_lastElapsed = 0;
	_elapsed = 0;
	_total = 0;
}

GameTime::~GameTime(void)
{
}

//public functions
void GameTime::Begin()
{
	_start = std::clock();
	_lastElapsed = (float)_start;
}

float GameTime::GetTotalMS()
{
	return ToMS((float) (std::clock() - _start));
}

void GameTime::Update()
{
	_elapsed = std::clock() - _lastElapsed;
	if(!_paused)
	{
		_lastElapsed = (float)std::clock();
	}
}

float GameTime::GetElapsedMS()
{
	return ToMS(_elapsed);
	
}

void GameTime::Pause()
{
	if(!_paused)
	{
		_paused = true;
	}
	else 
	{
		_paused = false;
		_start += (clock_t)_elapsed;
	}
}

void GameTime::Restart()
{
	Begin();
}

//Private Functions
float GameTime::ToMS(float arg)
{
	return (arg / CLOCKS_PER_SEC) * 1000;
}
