#include "Audio.h"
Music::Music(const char* filePath)
	:pMusic(nullptr), pFilePath(filePath), isPlaying(false), pLoopCount(0)
{
	pMusic = Mix_LoadMUS(filePath);
	if (pMusic == nullptr)
		std::cout << "Failed to load music!" << std::endl;
}
Music::~Music()
{
	Mix_FreeMusic(pMusic);
}

void Music::play(int loopCount)
{
	if (!isPlaying)
	{
		isPlaying = true;
		pLoopCount = loopCount;
		Mix_PlayMusic(pMusic, loopCount);
	}
}

void Music::pause()
{
	if (isPlaying)
	{
		isPlaying = false;
		Mix_PauseMusic();
	}
}
void Music::resume()
{
	if (!isPlaying)
	{
		isPlaying = true;
		Mix_ResumeMusic();
	}
}
void Music::stop()
{
	isPlaying = false;
	Mix_HaltMusic();
}

SoundEffect::SoundEffect(const char* filePath)
	:pChunk(nullptr), pFilePath(filePath), pLoopCount(0)
{
	pChunk = Mix_LoadWAV(filePath);
	if (pChunk == nullptr)
		std::cout << "Failed to load sound effect!" << std::endl;
}
SoundEffect::~SoundEffect()
{
	Mix_FreeChunk(pChunk);
}

void SoundEffect::play(int channel, int loopCount)
{
	pChannel = channel;
	pLoopCount = loopCount;
	Mix_PlayChannel(channel, pChunk, loopCount);
}