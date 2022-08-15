#pragma once
#include <SDL.h>

class Timer
{
private:
	int pStartTime;
	int pPassedTime;
	int pTimePassedBeforePause;

public:
	bool isPasued;
public:
	Timer();

	void start();
	void stop();
	void pause();
	void resume();

	int getPassedTime();
};

