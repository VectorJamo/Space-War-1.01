#include "Sprite.h"

SDL_Renderer* Sprite::pRenderer;
Sprite::Sprite()
	:pTexture(nullptr), pFilePath(nullptr), x(0), y(0), width(0), height(0), rotAngle(0)
{
	currentClipRect = { 0, 0, 0, 0 };
}
Sprite::Sprite(const char* filePath)
	:pTexture(nullptr), pFilePath(filePath), x(0), y(0), width(0), height(0), rotAngle(0)
{
	loadTexture(filePath);
	currentClipRect = { 0, 0, 0, 0 };
}
Sprite::~Sprite()
{
	if (pTexture != nullptr)
		SDL_DestroyTexture(pTexture);
}
void Sprite::loadTexture(const char* filePath)
{
	// Load the texture
	pTexture = IMG_LoadTexture(pRenderer, filePath);
	if (pTexture == nullptr)
	{
		std::cout << "Failed to load texture!" << std::endl;
		std::cin.get();
	}

	// Get the texture's dimensions
	SDL_QueryTexture(pTexture, NULL, NULL, &width, &height);
}
void Sprite::UseRenderer(SDL_Renderer* renderer)
{
	pRenderer = renderer;
}
void Sprite::setColorKey(int r, int g, int b)
{
	SDL_Surface* imgSurface = IMG_Load(pFilePath);
	SDL_SetColorKey(imgSurface, SDL_TRUE, SDL_MapRGB(imgSurface->format, r, g, b));
	
	pTexture = SDL_CreateTextureFromSurface(pRenderer, imgSurface);

	SDL_FreeSurface(imgSurface);
}
void Sprite::setColorModulation(int r, int g, int b)
{
	SDL_SetTextureColorMod(pTexture, r, g, b);
}
void Sprite::setAlphaModulation(int a)
{
	SDL_SetTextureBlendMode(pTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(pTexture, a);
}
void Sprite::draw(int rotAngle, SDL_Point* rotPoint, SDL_RendererFlip flipMode)
{
	SDL_Rect destRect = { x, y, width, height };
	SDL_RenderCopyEx(pRenderer, pTexture, NULL, &destRect, rotAngle, rotPoint, flipMode);
}
void Sprite::draw(int x, int y, int rotAngle, SDL_Point* rotPoint, SDL_RendererFlip flipMode)
{
	this->x = x;
	this->y = y;
	SDL_Rect destRect = { x, y, width, height };
	SDL_RenderCopyEx(pRenderer, pTexture, NULL, &destRect, rotAngle, rotPoint, flipMode);
}
void Sprite::draw(int x, int y, SDL_Rect& clipRect, int rotAngle, SDL_Point* rotPoint, SDL_RendererFlip flipMode)
{
	this->x = x;
	this->y = y;
	SDL_Rect destRect = { x, y, width, height };
	SDL_RenderCopyEx(pRenderer, pTexture, &clipRect, &destRect, rotAngle, rotPoint, flipMode);
}