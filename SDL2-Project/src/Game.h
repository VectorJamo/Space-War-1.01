#pragma once
#include "Engine/Window.h"
#include "Engine/Sprite.h"
#include "Engine/Text.h"
#include "Engine/Button.h"
#include "Engine/Timer.h"
#include "Engine/Audio.h"
#include "Engine/GameManager.h"

#define SCRWIDTH 800
#define SCRHEIGHT 600

class Game
{
private:
	bool running;
	Window* window;

	// Gamestate
	GameManager GAMESTATE;

	// Game Objects
	Sprite* background;
	Sprite* player;
	int kills;
	Sprite* bullet;
	bool launched;

	std::vector<Sprite*> enemies;
	std::vector<Sprite*> nukes;

	// UI
	int healthPosX, healthPosY, healthWidth, healthHeight;
	std::string healthText;
	Text* health;
	std::string scoreText;
	Text* score;

	// Audio
	Music* gameMusic;
	SoundEffect* enemyExplosion;
	SoundEffect* nukeExplosion;
	SoundEffect* playerHurt;
	SoundEffect* laserShoot;
	bool isLaserAudioPlayed;
	SoundEffect* buttonClick;

	// Menu
	Sprite* gameText;
	Sprite* menuBackground;
	Button* play;
	Button* controls;
	Button* quit;

	Sprite* controlsImage;
	Button* back;

	Text* endText;
	Button* restart;
	bool wasEnded;

private:
	void handleInput();

public:
	Game();
	~Game();

	void menu();
	void controlsMenu();
	void game();
	void endMenu();

	void freeMemory();
	void init();
	void run();
	void setup();
	void update();

};

