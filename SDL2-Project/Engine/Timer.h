#pragma once
#include <SDL.h>

class Timer
{
private:
	int pStartTime;
	int pPassedTime;
	int pTimePassedBeforePause;
	
	// For delta time
	static int StartTime;
	static int DeltaTime;

public:
	bool isPasued;
public:
	Timer();

	void start();
	void stop();
	void pause();
	void resume();

	int getPassedTime();

	// For delta time
	static void Init();
	static void Update();
	static float GetDeltaTime();
};

