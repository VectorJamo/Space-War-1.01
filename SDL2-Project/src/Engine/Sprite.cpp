#include "Sprite.h"

SDL_Renderer* Sprite::pRenderer;
Sprite::Sprite()
	:pTexture(nullptr), pFilePath(nullptr), pXpos(0), pYpos(0), pWidth(0), pHeight(0), isEnabled(true)
{
}
Sprite::Sprite(const char* filePath)
	:pTexture(nullptr), pFilePath(filePath), pXpos(0), pYpos(0), pWidth(0), pHeight(0), isEnabled(true)
{
	loadFromFile(filePath);
}
Sprite::~Sprite()
{
	SDL_DestroyTexture(pTexture);
}

void Sprite::UseRenderer(SDL_Renderer* renderer)
{
	pRenderer = renderer;
}

void Sprite::loadFromFile(const char* filePath)
{
	isEnabled = true;
	// Prevent memory leaks
	if (pTexture != nullptr)
	{
		SDL_DestroyTexture(pTexture);
		pTexture = nullptr;
	}
	pTexture = IMG_LoadTexture(pRenderer, filePath);
	if (pTexture == nullptr)
		std::cout << "Failed to load texture!" << std::endl;

	SDL_QueryTexture(pTexture, NULL, NULL, &pWidth, &pHeight);
}
bool Sprite::isCollided(Sprite* sprite)
{
	if (isEnabled == false || sprite->isEnabled == false)
		return false;

	int topA = getYPos();
	int bottomA = getYPos() + getHeight();
	int leftA = getXPos();
	int rightA = getXPos() + getWidth();

	int topB = sprite->getYPos();
	int bottomB = sprite->getYPos() + sprite->getHeight();
	int leftB = sprite->getXPos();
	int rightB = sprite->getXPos() + sprite->getWidth();

	if (topA > bottomB || topB > bottomA || rightA < leftB || rightB < leftA)
		return false;
	return true;
}
void Sprite::setVelocity(int xvel, int yvel)
{
	pVelX = xvel;
	pVelY = yvel;
}
void Sprite::setSize(int width, int height)
{
	pWidth = width;
	pHeight = height;
}
void Sprite::draw()
{
	if (!isEnabled)
		return;
	SDL_Rect destRect = { pXpos, pYpos, pWidth, pHeight };
	SDL_RenderCopy(pRenderer, pTexture, NULL, &destRect);
}
void Sprite::draw(int x, int y)
{
	if (!isEnabled)
		return;
	pXpos = x;
	pYpos = y;
	SDL_Rect destRect = { x, y, pWidth, pHeight };
	SDL_RenderCopy(pRenderer, pTexture, NULL, &destRect);
}
void Sprite::draw(SDL_Rect* clipRect, int x, int y)
{
	if (!isEnabled)
		return;
	pXpos = x;
	pYpos = y;
	SDL_Rect destRect = { x, y, clipRect->w, clipRect->h};
	SDL_RenderCopy(pRenderer, pTexture, clipRect, &destRect);
}
void Sprite::setXPos(int x)
{
	pXpos = x;
}
void Sprite::setYPos(int y)
{
	pYpos = y;
}
void Sprite::setColorKey(int r, int g, int b)
{
	// Prevent memory leaks
	if (pTexture != nullptr)
	{
		SDL_DestroyTexture(pTexture);
		pTexture = nullptr;
	}
	SDL_Surface* surface = IMG_Load(pFilePath);
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, r, g, b));
	pTexture = SDL_CreateTextureFromSurface(pRenderer, surface);

	SDL_FreeSurface(surface);
}
void Sprite::setColorModulation(int r, int g, int b)
{
	SDL_SetTextureColorMod(pTexture, r, g, b);
}
void Sprite::setBlendMode(SDL_BlendMode blendMode)
{
	SDL_SetTextureBlendMode(pTexture, blendMode);
}
void Sprite::setAlphaModulation(int a)
{
	SDL_SetTextureAlphaMod(pTexture, a);
}

void Sprite::setAnimation(int clipRectsCount, SDL_Rect* clipRects)
{
	pAnimations.push_back(Animation(clipRectsCount, clipRects));
}
void Sprite::playAnimation(int animationIndex)
{
	pCurrentClipRect = pAnimations[animationIndex].play();
}

Animation::Animation(int frameCount, SDL_Rect* clipRects)
	:pTotalFrames(frameCount), pCurrentFrame(0), pClipRects(clipRects)
{
}
SDL_Rect Animation::play()
{
	SDL_Rect currentClipRect = pClipRects[pCurrentFrame/5];
	pCurrentFrame++;
	if ((pCurrentFrame / 5) > pTotalFrames - 1)
		pCurrentFrame = 0;

	return currentClipRect;
}
