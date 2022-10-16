#include "UI.h"

//// UI
SDL_Renderer* UI::pRenderer;
UI::UI()
	:pX(0), pY(0), pWidth(0), pHeight(0), pR(0), pG(0), pB(0), pA(0) {}

void UI::UseRenderer(SDL_Renderer* renderer)
{
	pRenderer = renderer;
}

void UI::setPosition(int x, int y)
{
	this->pX = x;
	this->pY = y;
}
void UI::setSize(int width, int height)
{
	this->pWidth = width;
	this->pHeight = height;
}

bool UI::isMouseOver()
{
	int mx, my;
	SDL_GetMouseState(&mx, &my);

	if ((mx > pX && mx < pX + pWidth) && (my > pY && my < pY + pHeight))
		return true;
	return false;
}

//// Text
Text::Text(const char* fontPath, int pointSize)
	:pText(nullptr), pFont(nullptr), pTexture(nullptr)
{
	// Initialize all inherited variables to 0
	pX = 0; pY = 0; pWidth = 0; pHeight = 0; pR = 0; pG = 0; pB = 0; pA = 0;

	pFont = TTF_OpenFont(fontPath, pointSize);
	if (pFont == nullptr)
		std::cout << "Failed to load font!" << std::endl;
}

void Text::loadFont(const char* filePath, int pointSize)
{
	if (pFont != nullptr)
		TTF_CloseFont(pFont);

	pFont = TTF_OpenFont(filePath, pointSize);
	if (pFont == nullptr)
		std::cout << "Failed to load font!" << std::endl;
}
void Text::setText(const char* text)
{
	pText = text;
	
	// Load the texture from font and text
	SDL_Color color = { pR, pG, pB, pA };
	SDL_Surface* surface = TTF_RenderText_Blended(pFont, pText, color);

	// Prevent memory leaks
	if (pTexture != nullptr)
		SDL_DestroyTexture(pTexture);

	pTexture = SDL_CreateTextureFromSurface(pRenderer, surface);
	if (pTexture == nullptr)
		std::cout << "Failed to load font texture!" << std::endl;

	SDL_FreeSurface(surface);
}
void Text::setColor(int r, int g, int b) 
{
	pR = r;
	pG = g;
	pB = b;
	
	// Load the texture from font and text
	SDL_Color color = { pR, pG, pB, pA };
	SDL_Surface* surface = TTF_RenderText_Blended(pFont, pText, color);

	// Prevent memory leaks
	if (pTexture != nullptr)
		SDL_DestroyTexture(pTexture);


	pTexture = SDL_CreateTextureFromSurface(pRenderer, surface);
	if (pTexture == nullptr)
		std::cout << "Failed to load font texture!" << std::endl;

	SDL_FreeSurface(surface);
}
void Text::setAlpha(int a)
{
	pA = a;
	// Load the texture from font and text
	SDL_Color color = { pR, pG, pB, pA };
	SDL_Surface* surface = TTF_RenderText_Blended(pFont, pText, color);

	// Prevent memory leaks
	if (pTexture != nullptr)
		SDL_DestroyTexture(pTexture);

	pTexture = SDL_CreateTextureFromSurface(pRenderer, surface);
	if (pTexture == nullptr)
		std::cout << "Failed to load font texture!" << std::endl;

	SDL_FreeSurface(surface);
}
void Text::draw()
{
	SDL_Rect destRect = { pX, pY, pWidth, pHeight };
	SDL_RenderCopy(pRenderer, pTexture, NULL, &destRect);
}

//// Button
Button::Button(const char* texturePath, const char* fontPath, int pointSize)
	:pButtonTexture(nullptr), pButtonText(nullptr), pTextR(0), pTextG(0), pTextB(0), pTextA(0), pBorderThickness(0), pBorderR(0), pBorderG(0), pBorderB(0), pBorderA(0)
{
	// Initialize all inherited variables to 0
	pX = 0; pY = 0; pWidth = 0; pHeight = 0; pR = 0; pG = 0; pB = 0; pA = 0;

	pButtonText = new Text(fontPath, pointSize);
	pButtonTexture = new Sprite(texturePath);
}
Button::~Button()
{
	delete pButtonText;
	delete pButtonTexture;
}
bool Button::isMouseOver()
{
	int mx, my;
	SDL_GetMouseState(&mx, &my);

	if ((mx > pX && mx < pX + pWidth) && (my > pY && my < pY + pHeight))
		return true;
	return false;
}
void Button::setBorderThickness(int pixels)
{
	pBorderThickness = pixels;
}
void Button::setBorderColor(int r, int g, int b, int a)
{
	pBorderR = r;
	pBorderG = g;
	pBorderB = b;
	pBorderA = a;
}
void Button::setPosition(int x, int y)
{
	pX = x;
	pY = y;

	int textPosOffset = 4;
	pButtonText->setPosition(x + textPosOffset, y);
	pButtonTexture->x = x;
	pButtonTexture->y = y;
}
void Button::setSize(int width, int height)
{
	pWidth = width;
	pHeight = height;

	int textWidthOffset = 6;
	pButtonText->setSize(width - textWidthOffset, height);
	pButtonTexture->width = width;
	pButtonTexture->height = height;
}
void Button::setAlphaModulation(int a)
{
	pButtonTexture->setAlphaModulation(a);
}
void Button::setText(const char* text)
{
	pButtonText->setText(text);
}
void Button::setTextColor(int r, int g, int b)
{
	pButtonText->setColor(r, g, b);
}
void Button::setTextAlpha(int a)
{
	pButtonText->setAlpha(a);
}

void Button::draw()
{
	// Draw the border
	for (int i = 1; i <= pBorderThickness; i++)
	{
		SDL_Rect rect = { pX - i, pY - i, pWidth + i*2, pHeight + i*2 };
		SDL_SetRenderDrawColor(pRenderer, pBorderR, pBorderG, pBorderB, pBorderA);
		SDL_RenderDrawRect(pRenderer, &rect);
	}

	// Draw the button first
	pButtonTexture->draw();

	// Then, draw the text over the button
	pButtonText->draw();
}
