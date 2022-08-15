#pragma once
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>

class Animation
{
private:
	int pTotalFrames;
	int pCurrentFrame;

	SDL_Rect* pClipRects;

public:
	Animation(int frameCount, SDL_Rect* clipRects);
	SDL_Rect play();
};
class Sprite
{
private:
	SDL_Texture* pTexture;
	static SDL_Renderer* pRenderer;

	const char* pFilePath;
	int pXpos, pYpos, pWidth, pHeight, pVelX, pVelY;
	
	SDL_Rect pCurrentClipRect;

public:
	bool isEnabled;

public:
	Sprite();
	Sprite(const char* filePath);
	~Sprite();

	static void UseRenderer(SDL_Renderer* renderer);

	void loadFromFile(const char* filePath);
	
	void draw();
	void draw(int x, int y);
	void draw(SDL_Rect* clipRect, int x, int y);

	bool isCollided(Sprite* sprite);

	// Setters
	void setXPos(int x);
	void setYPos(int y);
	void setVelocity(int xvel, int yvel);
	void setSize(int width, int height);
	void setColorKey(int r, int g, int b);
	void setColorModulation(int r, int g, int b);
	void setBlendMode(SDL_BlendMode blendMode);
	void setAlphaModulation(int a);

	void setAnimation(int clipRectsCount, SDL_Rect* clipRects);
	void playAnimation(int animationIndex);

	// Getters
	inline int getXPos() { return pXpos; }
	inline int getYPos() { return pYpos; }
	inline int getVelX() { return pVelX; }
	inline int getVelY() { return pVelY; }
	inline int getWidth() { return pWidth; }
	inline int getHeight() { return pHeight; }
	inline SDL_Rect getCurrentClipRect() { return pCurrentClipRect; }

private:
	std::vector<Animation> pAnimations;
};

