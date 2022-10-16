#pragma once
#include <iostream>
#include <memory>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Timer.h"

class Window
{
private:
	SDL_Window* pWindow;
	SDL_Renderer* pRenderer;

	const char* pTitle;
	int pWidth, pHeight;
	int pFps;

	Timer timer;
private:
	bool init();

public:
	Window(int width, int height, const char* title);
	~Window();

	void clear(int r, int g, int b, int a);
	
	void drawLine(int x1, int y1, int x2, int y2, int r, int g, int b, int a);
	void drawDot(int x, int y, int r, int g, int b, int a);
	void drawRectOutline(int x, int y, int width, int height, int r, int g, int b, int a);
	void drawFilledRect(int x, int y, int width, int height, int r, int g, int b, int a);

	void display();

	// Setters
	void setRenderViewPort(int x, int y, int width, int height);
	void setDrawBlendMode(SDL_BlendMode blendMode);
	void setFrameRateLimit(int fps);
	
	// Getters
	SDL_Renderer* getRenderer() { return pRenderer; }
};

