#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

struct Sprite
{
protected:
	static SDL_Renderer* pRenderer;
	SDL_Texture* pTexture;
	const char* pFilePath;

public:
	float x, y;
	int width, height;
	int rotAngle;
	SDL_Rect currentClipRect;
	
	Sprite();
	Sprite(const char* filePath);
	~Sprite();

	static void UseRenderer(SDL_Renderer* renderer);

	void setColorKey(int r, int g, int b);
	void setColorModulation(int r, int g, int b);
	void setAlphaModulation(int a);

	void draw(int rotAngle = 0, SDL_Point* rotPoint = NULL, SDL_RendererFlip flipMode = SDL_FLIP_NONE);
	void draw(int x, int y, int rotAngle = 0, SDL_Point* rotPoint = NULL,  SDL_RendererFlip flipMode = SDL_FLIP_NONE);
	void draw(int x, int y, SDL_Rect& clipRect, int rotAngle = 0, SDL_Point* rotPoint = NULL, SDL_RendererFlip flipMode = SDL_FLIP_NONE);

protected:
	void loadTexture(const char* filePath);
};

