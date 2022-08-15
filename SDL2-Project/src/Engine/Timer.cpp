#include "Timer.h"

Timer::Timer()
	:pStartTime(0), pPassedTime(0), pTimePassedBeforePause(0), isPasued(false)
{
}

void Timer::start()
{
	pStartTime = SDL_GetTicks();
}
void Timer::stop()
{
	pStartTime = 0;
	pPassedTime = 0;
	pTimePassedBeforePause = 0;
}
void Timer::pause()
{
	if (!isPasued)
	{
		isPasued = true;
		pTimePassedBeforePause += SDL_GetTicks() - pStartTime;
		pStartTime = 0;
	}
}
void Timer::resume()
{
	if (isPasued)
	{
		isPasued = false;
		pStartTime = SDL_GetTicks();
	}
}
int Timer::getPassedTime()
{
	if (isPasued)
		return pTimePassedBeforePause;
	else
		return (pTimePassedBeforePause + (SDL_GetTicks() - pStartTime));
}
