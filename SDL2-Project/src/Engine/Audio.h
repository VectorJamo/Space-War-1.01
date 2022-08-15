#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>

class Music
{
private:
	Mix_Music* pMusic;
	const char* pFilePath;
	int pLoopCount;

public:
	bool isPlaying;

public:
	Music(const char* filePath);
	~Music();

	void play(int loopCount);
	void pause();
	void resume();
	void stop();
};

class SoundEffect
{
private:
	Mix_Chunk* pChunk;
	const char* pFilePath;
	int pChannel;
	int pLoopCount;

public:
	SoundEffect(const char* filePath);
	~SoundEffect();

	void play(int channel, int loopCount);
};

