#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

class Text
{
private:
	SDL_Texture* pTextTexture;
	TTF_Font* pFont;
	static SDL_Renderer* pRenderer;

	const char* pFontPath;
	int pFontSize;

	const char* pText;
	int pXpos, pYpos, pWidth, pHeight;
	int pR, pG, pB, pA;

public:
	Text(const char* fontPath, int fontSize);
	Text(const char* fontPath, int fontSize, const char* text = "", int xpos = 0, int ypos = 0, int width = 0, int height = 0, int r = 0, int g = 0, int b = 0, int a = 0);
	~Text();

	static void UseRenderer(SDL_Renderer* renderer);
	void loadFont(const char* fontPath, int fontSize);
	void loadTextFromFont(const char* text);

	void draw();
	void draw(int x, int y);

	// Setters
	void setFontSize(int fontSize);
	void setText(const char* text);
	void setPosition(int x, int y);
	void setSize(int width, int height);
	void setColor(int r, int g, int b);
	void setAlpha(int a);
	void setBlendMode(SDL_BlendMode blendMode);

	// Getters
	inline SDL_Texture* getTextTexture() { return pTextTexture; }
	inline int getXPos() { return pXpos; }
	inline int getYPos() { return pYpos; }
	inline int getWidth() { return pWidth; }
	inline int getHeight() { return pHeight; }

};

