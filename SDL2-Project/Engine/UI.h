#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Sprite.h"
#include <iostream>

class UI
{
protected:
	static SDL_Renderer* pRenderer;
	int pX, pY, pWidth, pHeight;
	int pR, pG, pB, pA;

public:
	UI();
	static void UseRenderer(SDL_Renderer* renderer);
	
	bool isMouseOver();
	virtual void draw() = 0;

	// Setters
	virtual void setPosition(int x, int y);
	virtual void setSize(int width, int height);
	
	// Getters
	inline int getXPos() { return pX; }
	inline int getYPos() { return pY; }
	inline int getWidth() { return pWidth; }
	inline int getHeight() { return pHeight; }
};

class Text : public UI
{
private:
	const char* pText;
	TTF_Font* pFont;

	SDL_Texture* pTexture;

public:
	Text(const char* fontPath, int pointSize);

	void loadFont(const char* fontPath, int pointSize);

	// Setters
	void setColor(int r, int g, int b);
	void setAlpha(int a);
	void setText(const char* text);

	void draw() override;
};
class Button : public UI
{
private:
	Sprite* pButtonTexture;
	Text* pButtonText;
	int pTextR, pTextG, pTextB, pTextA, pBorderThickness, pBorderR, pBorderG, pBorderB, pBorderA;

public:
	Button(const char* texturePath, const char* fontPath, int pointSize);
	~Button();

	bool isMouseOver();

	// Setters
	void setPosition(int x, int y) override;
	void setSize(int width, int height) override;
	void setAlphaModulation(int a);
	void setText(const char* text);
	void setTextColor(int r, int g, int b);
	void setTextAlpha(int a);
	void setBorderThickness(int pixels);
	void setBorderColor(int r, int g, int b, int a);

	void draw() override;
};
