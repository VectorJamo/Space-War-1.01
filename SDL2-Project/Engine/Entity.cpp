#include "Entity.h"

Entity::Entity(const char* filePath)
	:xspeed(0), yspeed(0)
{
	pFilePath = filePath;
	loadTexture(filePath);
}

void Entity::setAnimation(int index, SDL_Rect* clipRects, int frameCount)
{
	animations.push_back(Animation(frameCount, clipRects));
}
void Entity::playAnimation(int index)
{
	animations[index].isActive = true;
}
void Entity::updateAnimation(int index)
{
	currentClipRect = animations[index].play();
}
Animation::Animation(int frameCount, SDL_Rect* clipRects)
	:totalFrames(frameCount), currentFrame(0), clipRects(clipRects), isActive(false)
{

}

SDL_Rect Animation::play()
{
	currentFrame++;
	if ((currentFrame / 4) > totalFrames)
	{
		currentFrame = 0;
		isActive = false;
		SDL_Rect nullRect = { 0, 0, 0, 0 };
		return nullRect;
	}
	return clipRects[currentFrame / 4];
}