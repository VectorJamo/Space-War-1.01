#pragma once
#include "Text.h"

class Button
{
private:
	Text* pText;
	static SDL_Renderer* pRenderer;

	const char* pFontPath;
	int pFontSize;

	int pXpos, pYpos, pWidth, pHeight;
	int pR, pG, pB, pA;
public:
	bool isMouseOver;

public:
	Button(int x, int y, int width, int height, int r, int g, int b, int a);
	Button(int x, int y, int width, int height, int r, int g, int b, int a, const char* fontPath, int fontSize, const char* text, int tR, int tG, int tB, int tA);
	~Button();

	static void UseRenderer(SDL_Renderer* renderer);

	void draw();
	void draw(int x, int y);

	// Setters
	void setPosition(int x, int y);
	void setSize(int width, int height);
	void setColor(int r, int g, int b);
	void setAlpha(int a);
	void setBlendMode(SDL_BlendMode blendMode);

	void addText(const char* fontPath, int fontSize, const char* text, int r, int g, int b, int a);
	void setText(const char* text);
	void setTextColor(int r, int g, int b);
	void setTextAlpha(int a);

	// Getters
	inline Text* getTextObject() { return pText; }
	inline int getXPos() { return pXpos; }
	inline int getYPos() { return pYpos; }
	inline int getWidth() { return pWidth; }
	inline int getHeight() { return pHeight; }
};

