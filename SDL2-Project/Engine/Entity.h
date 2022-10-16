#pragma once
#include "Sprite.h"
#include <vector>

struct Animation
{
	int totalFrames;
	int currentFrame;

	SDL_Rect* clipRects;
	bool isActive;

	Animation(int frameCount, SDL_Rect* clipRects);
	SDL_Rect play();
};

struct Entity : public Sprite
{
public:
	int xspeed, yspeed;

	std::vector<Animation> animations;

	Entity(const char* filePath);

	void setAnimation(int index, SDL_Rect* clipRects, int frameCount);
	void playAnimation(int index);
	void updateAnimation(int index);
};

