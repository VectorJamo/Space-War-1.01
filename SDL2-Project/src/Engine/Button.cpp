#include "Button.h"

SDL_Renderer* Button::pRenderer;

Button::Button(int x, int y, int width, int height, int r, int g, int b, int a)
	:pText(nullptr), pFontPath(nullptr), pFontSize(0), pXpos(x), pYpos(y), pWidth(width), pHeight(height), pR(r), pG(g), pB(b), pA(a), isMouseOver(false)
{
}
Button::Button(int x, int y, int width, int height, int r, int g, int b, int a, const char* fontPath, int fontSize, const char* text, int tR, int tG, int tB, int tA)
	: pText(nullptr), pFontPath(fontPath), pFontSize(fontSize), pXpos(x), pYpos(y), pWidth(width), pHeight(height), pR(r), pG(g), pB(b), pA(a), isMouseOver(false)
{
	pText = new Text(fontPath, fontSize, text, x, y, width, height, tR, tG, tB, tA);
}
Button::~Button()
{
	if (pText != nullptr)
		delete pText;
}

void Button::UseRenderer(SDL_Renderer* renderer)
{
	pRenderer = renderer;
	Text::UseRenderer(renderer);
}

void Button::draw()
{
	// Check if mouse is over the button
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	if ((mx > pXpos && mx < (pXpos + pWidth)) && (my > pYpos && my < (pYpos + pHeight)))
		isMouseOver = true;
	else
		isMouseOver = false;

	// Draw the button rect
	SDL_SetRenderDrawColor(pRenderer, pR, pG, pB, pA);
	SDL_Rect rect = { pXpos, pYpos, pWidth, pHeight };
	SDL_RenderFillRect(pRenderer, &rect);

	// Draw the button text
	if (pText != nullptr)
	{
		pText->draw();
	}
}
void Button::draw(int x, int y)
{
	pXpos = x;
	pYpos = y;
	// Check if mouse is over the button
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	if ((mx > pXpos && mx < (pXpos + pWidth)) && (my > pYpos && my < (pYpos + pHeight)))
		isMouseOver = true;
	else
		isMouseOver = false;

	// Draw the button rect
	SDL_SetRenderDrawColor(pRenderer, pR, pG, pB, pA);
	SDL_Rect rect = { x, y, pWidth, pHeight };
	SDL_RenderFillRect(pRenderer, &rect);

	// Draw the button text
	if (pText != nullptr)
	{
		pText->draw(x, y);
	}
}

void Button::setPosition(int x, int y)
{
	pXpos = x;
	pYpos = y;
	if(pText != nullptr)
		pText->setPosition(x, y);
}
void Button::setSize(int width, int height)
{
	pWidth = width;
	pHeight = height;
	if (pText != nullptr)
		pText->setSize(width, height);
}
void Button::setColor(int r, int g, int b)
{
	pR = r;
	pG = g;
	pB = b;
}
void Button::setAlpha(int a)
{
	pA = a;
}
void Button::setBlendMode(SDL_BlendMode blendMode)
{
	SDL_SetRenderDrawBlendMode(pRenderer, blendMode);
	if (pText != nullptr)
		pText->setBlendMode(blendMode);
}
void Button::addText(const char* fontPath, int fontSize, const char* text, int r, int g, int b, int a)
{
	if (pText != nullptr)
		return;

	pText = new Text(fontPath, fontSize, text, pXpos, pYpos, pWidth, pHeight, r, g, b, a);

}
void Button::setText(const char* text)
{
	if (pText == nullptr)
		return;
	pText->setText(text);
}
void Button::setTextColor(int r, int g, int b)
{
	if (pText == nullptr)
		return;
	pText->setColor(r, g, b);
}
void Button::setTextAlpha(int a)
{
	if (pText == nullptr)
		return;
	pText->setAlpha(a);
}