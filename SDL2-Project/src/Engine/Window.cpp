#include "Window.h"

Window::Window(int width, int height, const char* title)
	:pWindow(nullptr), pRenderer(nullptr), pTitle(title), pWidth(width), pHeight(height), pFps(60)
{
	if (!init())
	{
		std::cout << "Failed to initialize Window!\n" << std::endl;
		std::cin.get();
	}
}
Window::~Window()
{
	SDL_DestroyWindow(pWindow);
	SDL_DestroyRenderer(pRenderer);

	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Window::init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Failed to initialize SDL! " << SDL_GetError() << std::endl;
		return false;
	}
	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
	{
		std::cout << "Failed to initialize SDL_image! " << IMG_GetError() << std::endl;
		return false;
	}
	if(TTF_Init() != 0)
	{
		std::cout << "Failed to initialize SDL_ttf! " << TTF_GetError() << std::endl;
		return false;
	}
	if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3) == 0)
	{
		std::cout << "Failed to initialize SDL_mixer! " << Mix_GetError() << std::endl;
		return false;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "Failed to initialize SDL_mixer! " << Mix_GetError() << std::endl;
		return false;
	}

	pWindow = SDL_CreateWindow(pTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pWidth, pHeight, SDL_WINDOW_SHOWN);
	if (pWindow == nullptr)
		return false;
	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	if (pRenderer == nullptr)
		return false;
	return true;
}
void Window::setRenderViewPort(int x, int y, int width, int height)
{
	SDL_Rect rect = { x, y, width, height };
	SDL_RenderSetViewport(pRenderer, &rect);
}

void Window::setDrawBlendMode(SDL_BlendMode blendMode)
{
	SDL_SetRenderDrawBlendMode(pRenderer, blendMode);
}
void Window::clear(int r, int g, int b, int a)
{
	timer.start();
	SDL_SetRenderDrawColor(pRenderer, r, g, b, a);
	SDL_RenderClear(pRenderer);
}
void Window::setFrameRateLimit(int fps)
{
	pFps = fps;
}
void Window::drawLine(int x1, int y1, int x2, int y2, int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(pRenderer, r, g, b, a);
	SDL_RenderDrawLine(pRenderer, x1, y1, x2, y2);
}

void Window::drawDot(int x, int y, int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(pRenderer, r, g, b, a);
	SDL_RenderDrawPoint(pRenderer, x, y);
}

void Window::drawRectOutline(int x, int y, int width, int height, int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(pRenderer, r, g, b, a);

	SDL_Rect rect = { x, y, width, height };
	SDL_RenderDrawRect(pRenderer, &rect);
}
void Window::drawFilledRect(int x, int y, int width, int height, int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(pRenderer, r, g, b, a);

	SDL_Rect rect = { x, y, width, height };
	SDL_RenderFillRect(pRenderer, &rect);
}

void Window::display()
{
	SDL_RenderPresent(pRenderer);
	int deltaTime = timer.getPassedTime();
	if (deltaTime < (1000 / pFps))
	{
		SDL_Delay((1000 / pFps) - deltaTime);
		timer.stop();
	}
}
