#include "Text.h"

SDL_Renderer* Text::pRenderer;
Text::Text(const char* fontPath, int fontSize)
	: pTextTexture(nullptr), pFont(nullptr), pFontPath(nullptr), pFontSize(fontSize), pText(nullptr),
	pXpos(0), pYpos(0), pWidth(0), pHeight(0), pR(0), pG(0), pB(0), pA(0)
{

}
Text::Text(const char* fontPath, int fontSize, const char* text, int xpos, int ypos, int width, int height, int r, int g, int b, int a)
	:pTextTexture(nullptr), pFont(nullptr), pFontPath(fontPath), pFontSize(fontSize), pText(text),
	pXpos(xpos), pYpos(ypos), pWidth(width), pHeight(height), pR(r), pG(g), pB(b), pA(a)
{
	loadFont(fontPath, fontSize);
	loadTextFromFont(text);
}
Text::~Text()
{
	SDL_DestroyTexture(pTextTexture);
}

void Text::UseRenderer(SDL_Renderer* renderer)
{
	pRenderer = renderer;
}

void Text::loadFont(const char* fontPath, int fontSize)
{
	pFont = TTF_OpenFont(fontPath, fontSize);
	if (pFont == nullptr)
		std::cout << "Failed to load font!" << std::endl;

}
void Text::loadTextFromFont(const char* text)
{
	// Prevent memory leaks
	if (pTextTexture != nullptr)
	{
		SDL_DestroyTexture(pTextTexture);
		pTextTexture = nullptr;
	}

	pText = text;
	
	SDL_Color color = { pR, pG, pB, pA};

	SDL_Surface* surface = TTF_RenderText_Solid(pFont, text, color);
	pTextTexture = SDL_CreateTextureFromSurface(pRenderer, surface);
	if (pTextTexture == nullptr)
		std::cout << "Failed to load texture from font!" << std::endl;

	SDL_FreeSurface(surface);

}
void Text::draw()
{
	SDL_Rect destRect = { pXpos, pYpos, pWidth, pHeight };
	SDL_RenderCopy(pRenderer, pTextTexture, NULL, &destRect);
}
void Text::draw(int x, int y)
{
	pXpos = x;
	pYpos = y;
	SDL_Rect destRect = { x, y, pWidth, pHeight };
	SDL_RenderCopy(pRenderer, pTextTexture, NULL, &destRect);
}

void Text::setFontSize(int fontSize)
{
	pFontSize = fontSize;
	loadFont(pFontPath, fontSize);
	loadTextFromFont(pText);
}
void Text::setText(const char* text)
{
	pText = text;
	loadTextFromFont(text);
}
void Text::setPosition(int x, int y)
{
	pXpos = x;
	pYpos = y;
}
void Text::setSize(int width, int height)
{
	pWidth = width;
	pHeight = height;
}
void Text::setColor(int r, int g, int b)
{
	pR = r;
	pG = g;
	pB = b;
	loadTextFromFont(pText);
}
void Text::setAlpha(int a)
{
	pA = a;
	loadTextFromFont(pText);
}
void Text::setBlendMode(SDL_BlendMode blendMode)
{
	SDL_SetTextureBlendMode(pTextTexture, blendMode);
}