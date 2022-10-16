#include "Engine/Engine.h"
#include <vector>
#include <string>

#define SCRWIDTH  1024
#define SCRHEIGHT 768
#define PI 3.14159
#define DEG_TO_RAD(x) x * (PI/180) 

enum GameManager
{
	TITLE_SCREEN,
	CONTROLS, 
	GAME,
	RESTART_SCREEN,
};

SDL_Rect defaultEnemyRect = { 0, 0, 32, 32 };
SDL_Rect enemyAnimations[8] =
{
	{0, 32, 32, 32}, {32, 32, 32, 32}, {32 * 2, 32, 32, 32}, {32 * 3, 32, 32, 32},
	{0, 32 * 2, 32, 32}, {32, 32 * 2, 32, 32}, {32 * 2, 32 * 2, 32, 32}, {32 * 3, 32 * 2, 25, 25}
};
int speedMultiplier = 1;
void rotateVector(int vec_x, int vec_y, int angle, int* vec_xnew, int* vec_ynew)
{
	int x, y;
	x = (cos(DEG_TO_RAD(angle)) * vec_x) +(-sin(DEG_TO_RAD(angle)) * vec_y);
	y = (sin(DEG_TO_RAD(angle)) * vec_x) + (cos(DEG_TO_RAD(angle)) * vec_y);

	*vec_xnew = speedMultiplier * x;
	*vec_ynew = speedMultiplier * y;
}
void addAsteroid(std::vector<Entity*>& asteroids)
{
	const char* filePath1 = "res/images/asteroid.png";
	const char* filePath2 = "res/images/asteroid2.png";
	const char* path;
	if ((rand() % 10) % 2 == 0)
		path = filePath1;
	else
		path = filePath2;

	Entity* a = new Entity(path);
	a->x = rand() % SCRWIDTH - 20;
	a->y = -(rand() % 100) - 40;
	if ((rand() % 10) % 2 == 0) // Just a way to get asteriod's random x-direction motion
		a->xspeed = (rand() % 10);
	else
		a->xspeed = -(rand() % 10);
	a->yspeed = rand() % 15;
	asteroids.push_back(a);
}
void addEnemy(std::vector<Entity*>& enemies)
{
	Entity* e = new Entity("res/images/sprites/enemySprite.png");
	e->currentClipRect = defaultEnemyRect;
	e->setAnimation(0, enemyAnimations, 8);
	e->x = rand() % SCRWIDTH - 20;
	e->y = -(rand() % 100) - 40;
	e->width = 64;
	e->height = 64;
	e->yspeed = (rand() % 10) + 2;
	enemies.push_back(e);
}

bool checkCollision(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
	if (rect1.x + rect1.w < rect2.x || rect1.x > rect2.x + rect2.w || rect1.y + rect1.h < rect2.y || rect1.y > rect2.y + rect2.h)
		return false;

	return true;
}

int main(int argc, char** argv)
{
	srand(time(0));

	Window window(SCRWIDTH, SCRHEIGHT, "Space-War 1.01");
	window.setFrameRateLimit(60);

	Sprite::UseRenderer(window.getRenderer());
	Sprite background("res/images/background.png");
	int scrollingOffset = 0;

	Entity ship("res/images/space-ship.png");
	ship.x = SCRWIDTH / 2 - ship.width/2;
	ship.y = SCRHEIGHT / 2 - ship.height/2;
	ship.xspeed = 0;
	ship.yspeed = -5;
	int angle = 0;

	Entity bullet("res/images/bullet.png");
	bullet.x = ship.x + bullet.width * 2;
	bullet.y = ship.y + bullet.height;
	bullet.xspeed = 0;
	bullet.yspeed = -20;
	bool fired = false;

	std::vector<Entity*> asteroids;
	for (int i = 0; i < 10; i++)
		addAsteroid(asteroids);

	std::vector<Entity*> enemies;
	for (int i = 0; i < 5; i++)
		addEnemy(enemies);
	
	// Powerups
	bool spawnPowerups = false;
	int amod = 255, da = 5;
	Entity healthPickup("res/images/health.png");
	healthPickup.x = rand() % SCRWIDTH;
	healthPickup.y = -80;
	healthPickup.xspeed = 0;
	healthPickup.yspeed = (rand() % 2) + 1;

	// Score text
	UI::UseRenderer(window.getRenderer());
	int score = 0;
	std::string text = "Score: " + std::to_string(score);
	
	Text scoreText("res/fonts/American Captain.ttf", 72);
	scoreText.setText(text.c_str());
	scoreText.setPosition(50, 50);
	scoreText.setSize(150, 50);
	scoreText.setColor(134, 191, 240);
	scoreText.setAlpha(255);

	// Health bar
	int health = 80;
	SDL_Color healthColor = { 0, 255, 0, 255 };
	window.drawFilledRect(ship.x, ship.y + ship.height + 10, health, 10, healthColor.r, healthColor.g, healthColor.b, healthColor.a);
	window.drawRectOutline(ship.x, ship.y + ship.height + 10, 80, 10, 255, 255, 255, 255);

	// Setup Music and SFX
	Music backgroundMusic("res/audio/TitleScreen-music.ogg");
	backgroundMusic.play(-1);
	SoundEffect shoot("res/audio/laser-shoot.wav");
	SoundEffect asteroidExplosion("res/audio/bullet-enemy-explosion.wav");
	SoundEffect enemyExplosion("res/audio/bullet-nuke-explosion.wav");	

	// Game-State Management
	GameManager GAMESTATE = GameManager::TITLE_SCREEN;

	// UI
	SoundEffect hoverMouse("res/audio/hover.wav");
	SoundEffect clickMouse("res/audio/click.wav");

	Sprite gameText("res/images/UI/text.png");
	gameText.width = 500;
	gameText.height = 400;
	gameText.x = SCRWIDTH / 2 - gameText.width / 2;
	gameText.y = 20;

	Sprite shipUI("res/images/UI/ship.png");

	Button playButton("res/images/UI/Button.png", "res/fonts/Pointless.ttf", 72);
	playButton.setText("Play");
	playButton.setSize(120, 50);
	playButton.setPosition(SCRWIDTH/2 - playButton.getWidth()/2, 400);
	playButton.setBorderThickness(2);
	playButton.setBorderColor(150, 80, 30, 255);

	Button controlsButton("res/images/UI/Button.png", "res/fonts/Pointless.ttf", 72);
	controlsButton.setText("Controls");
	controlsButton.setSize(200, 50);
	controlsButton.setPosition(SCRWIDTH / 2 - controlsButton.getWidth() / 2, 470);
	controlsButton.setBorderThickness(2);
	controlsButton.setBorderColor(150, 80, 30, 255);

	Button quitButton("res/images/UI/Button.png", "res/fonts/Pointless.ttf", 72);
	quitButton.setText("Exit");
	quitButton.setSize(120, 50);
	quitButton.setPosition(SCRWIDTH / 2 - quitButton.getWidth() / 2, 545);
	quitButton.setBorderThickness(2);
	quitButton.setBorderColor(150, 80, 30, 255);

	Text copyrightText("res/fonts/Pointless.ttf", 36);
	copyrightText.setText("Made by: Vector99");
	copyrightText.setSize(300, 30);
	copyrightText.setPosition(SCRWIDTH - copyrightText.getWidth() - 25, SCRHEIGHT - copyrightText.getHeight() - 30);
	copyrightText.setColor(225, 255, 255);
	copyrightText.setAlpha(255);

	Text versionText("res/fonts/Pointless.ttf", 36);
	versionText.setText("Version: 1.01");
	versionText.setSize(200, 30);
	versionText.setPosition(25, SCRHEIGHT - copyrightText.getHeight() - 30);
	versionText.setColor(225, 255, 255);
	versionText.setAlpha(255);

	Sprite controlsText("res/images/UI/ControlsText.png");
	controlsText.x = 50;
	controlsText.y = 50;

	Button backButton("res/images/UI/Button.png", "res/fonts/Pointless.ttf", 72);
	backButton.setText("Back");
	backButton.setSize(120, 50);
	backButton.setPosition(SCRWIDTH - quitButton.getWidth() * 3, 600);
	backButton.setBorderThickness(2);
	backButton.setBorderColor(150, 80, 30, 255);

	Button restartButton("res/images/UI/Button.png", "res/fonts/Pointless.ttf", 72);
	restartButton.setText("Restart");
	restartButton.setSize(200, 50);
	restartButton.setPosition(SCRWIDTH/2 - restartButton.getWidth()/2, 400);
	restartButton.setBorderThickness(2);
	restartButton.setBorderColor(150, 80, 30, 255);

	Button menuButton("res/images/UI/Button.png", "res/fonts/Pointless.ttf", 72);
	menuButton.setText("Main Menu");
	menuButton.setSize(200, 50);
	menuButton.setPosition(SCRWIDTH / 2 - menuButton.getWidth() / 2, 480);
	menuButton.setBorderThickness(2);
	menuButton.setBorderColor(150, 80, 30, 255);

	Sprite gameoverText("res/images/UI/Game-over.png");
	gameoverText.width = gameoverText.width + 100;
	gameoverText.height = gameoverText.height + 50;
	gameoverText.x = SCRWIDTH / 2 - gameoverText.width / 2;
	gameoverText.y = SCRHEIGHT / 2 - gameoverText.height / 2 - 200;

	Text finalScoreText("res/fonts/Pointless.ttf", 72);
	finalScoreText.setText("Score: ");
	finalScoreText.setSize(200, 50);
	finalScoreText.setPosition(SCRWIDTH / 2 - finalScoreText.getWidth() / 2, SCRHEIGHT / 2 - finalScoreText.getHeight() / 2 - 100);
	finalScoreText.setColor(225, 255, 255);
	finalScoreText.setAlpha(220);

	Entity powerPowerup("res/images/power.png");
	powerPowerup.x = rand()%(SCRWIDTH - 20);
	powerPowerup.y = -60;
	powerPowerup.width = 30;
	powerPowerup.height = 40;
	powerPowerup.yspeed = 2;
	bool spawnPowerPowerup = false, startPowerPowerup = true, powerUpEffect = false;
	int counter = 0;
	int increment = 1, playerAlphaMod = 255;
	int counter2 = 0;

	Text pausedText("res/fonts/Pointless.ttf", 72);
	pausedText.setText("PAUSED!");
	pausedText.setColor(225, 255, 255);
	pausedText.setAlpha(220);
	pausedText.setSize(400, 100);
	pausedText.setPosition(SCRWIDTH / 2 - pausedText.getWidth() / 2, SCRHEIGHT / 2 - pausedText.getHeight() / 2);
	bool paused = false;

	bool ph = true, ch = true, qh = true, bh = true, rh = true, mh = true;
	// Game loop
	bool running = true;
	while (running)
	{
		if (GAMESTATE == GameManager::TITLE_SCREEN)
		{
			// Clear the back buffer
			window.clear(0, 0, 0, 255);

			// Poll events
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					running = false;
				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					if (event.button.button == SDL_BUTTON_LEFT && playButton.isMouseOver())
					{
						clickMouse.play(1, 0);
						GAMESTATE = GameManager::GAME;
						scrollingOffset = 0;
					}
					if (event.button.button == SDL_BUTTON_LEFT && controlsButton.isMouseOver())
					{
						clickMouse.play(1, 0);
						GAMESTATE = GameManager::CONTROLS;
					}
					if (event.button.button == SDL_BUTTON_LEFT && quitButton.isMouseOver())
					{
						clickMouse.play(1, 0);
						SDL_Delay(40);
						running = false;
					}
				}
			}

			// Draw the current frame
			scrollingOffset--;
			if (scrollingOffset <= -SCRHEIGHT)
				scrollingOffset = 0;

			background.draw(0, scrollingOffset);
			background.draw(0, SCRHEIGHT + scrollingOffset);

			window.drawRectOutline(20, 20, SCRWIDTH - 40, SCRHEIGHT - 40, 255, 255, 255, 255);

			// Draw the UI elements
			gameText.draw();
			// Change the alpha of the button if mouse cursor is over it
			if (playButton.isMouseOver())
			{
				if (ph)
				{
					hoverMouse.play(2, 0);
					ph = false;
				}
				playButton.setAlphaModulation(150);
				shipUI.x = playButton.getXPos() - 40;
				shipUI.y = playButton.getYPos() + 10;
				shipUI.draw();
			}
			else
			{
				playButton.setAlphaModulation(255);
				ph = true;
			}
			if (controlsButton.isMouseOver())
			{
				if (ch)
				{
					hoverMouse.play(2, 0);
					ch = false;
				}
				controlsButton.setAlphaModulation(150);
				shipUI.x = controlsButton.getXPos() - 40;
				shipUI.y = controlsButton.getYPos() + 10;
				shipUI.draw();
			}
			else
			{
				controlsButton.setAlphaModulation(255);
				ch = true;
			}
			if (quitButton.isMouseOver())
			{
				if (qh) 
				{
					hoverMouse.play(2, 0);
					qh = false;
				}
				quitButton.setAlphaModulation(150);
				shipUI.x = quitButton.getXPos() - 40;
				shipUI.y = quitButton.getYPos() + 10;
				shipUI.draw();
			}
			else
			{
				quitButton.setAlphaModulation(255);
				qh = true;
			}

			playButton.draw();
			controlsButton.draw();
			quitButton.draw();
			copyrightText.draw();
			versionText.draw();

			// Swap buffers
			window.display();
		}
		if (GAMESTATE == GameManager::CONTROLS)
		{
			// Clear the back buffer
			window.clear(0, 0, 0, 255);

			// Poll events
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					running = false;
				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					if (event.button.button == SDL_BUTTON_LEFT && backButton.isMouseOver())
					{
						GAMESTATE = GameManager::TITLE_SCREEN;
						clickMouse.play(2, 0);
					}
				}
			}

			// Logic



			// Draw the current frame
			scrollingOffset--;
			if (scrollingOffset <= -SCRHEIGHT)
				scrollingOffset = 0;

			background.draw(0, scrollingOffset);
			background.draw(0, SCRHEIGHT + scrollingOffset);
			window.drawRectOutline(20, 20, SCRWIDTH - 40, SCRHEIGHT - 40, 255, 255, 255, 255);

			controlsText.draw();
			if (backButton.isMouseOver())
			{
				if (bh)
				{
					hoverMouse.play(1, 0);
					bh = false;
				}
				backButton.setAlphaModulation(150);
				shipUI.x = backButton.getXPos() - 40;
				shipUI.y = backButton.getYPos() + 10;
				shipUI.draw();
			}
			else
			{
				backButton.setAlphaModulation(255);
				bh = true;
			}

			backButton.draw();

			// Swap buffers
			window.display();
		} 
		if (GAMESTATE == GameManager::GAME)
		{
			// Clear the back buffer
			window.clear(0, 0, 0, 255);

			// Poll eventsa
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					running = false;
				if (event.type == SDL_KEYDOWN)
				{
					if (event.key.keysym.sym == SDLK_SPACE && fired == false)
					{
						fired = true;
						shoot.play(1, 0);
					}
					if (event.key.keysym.sym == SDLK_p)
					{
						if (paused)
							paused = false;
						else
							paused = true;
					}
				}
			}

			// Keyboard input for player movement
			const unsigned char* keyState = SDL_GetKeyboardState(NULL);
			if (keyState[SDL_SCANCODE_W] && paused == false)
			{
				ship.x += ship.xspeed;
				ship.y += ship.yspeed;
			}
			if (keyState[SDL_SCANCODE_A] && paused == false)
			{
				angle -= 3;
				bullet.rotAngle = angle;
				// Calculate bullet's new velocity as the angle changes
				rotateVector(0, -5, angle, &ship.xspeed, &ship.yspeed);
				rotateVector(0, -20, angle, &bullet.xspeed, &bullet.yspeed); //  Here,  0 and 20 are bullet's initial x and y velocity before any rotation!
			}
			if (keyState[SDL_SCANCODE_D] && paused == false)
			{
				angle += 3;
				bullet.rotAngle = angle;
				// Calculate bullet's new velocity as the angle changes
				rotateVector(0, -5, angle, &ship.xspeed, &ship.yspeed);
				rotateVector(0, -20, angle, &bullet.xspeed, &bullet.yspeed); //  Here,  0 and 20 are bullet's initial x and y velocity before any rotation!
			}

			// Check player-window bounds collision
			if (ship.x <= 20)
				ship.x = 20;
			else if (ship.x + ship.width >= SCRWIDTH - 20)
				ship.x = SCRWIDTH - ship.width - 20;
			if (ship.y <= 20)
				ship.y = 20;
			else if (ship.y + ship.height >= SCRHEIGHT - 20)
				ship.y = SCRHEIGHT - ship.height - 20;

			// Game logic
			if (!fired && paused == false)
			{
				bullet.x = ship.x + bullet.width * 2;
				bullet.y = ship.y + bullet.height;
			}
			else if (fired && paused == false)
			{
				bullet.x += bullet.xspeed;
				bullet.y += bullet.yspeed;

				if (bullet.y <= 0 || bullet.y + bullet.height >= SCRHEIGHT || bullet.x <= 0 || bullet.x + bullet.width >= SCRWIDTH)
				{
					fired = false;
					bullet.x = ship.x + bullet.width * 2;
					bullet.y = ship.y + bullet.height;
				}
			}
			// Update the asteroids
			for (auto& a : asteroids)
			{
				if (paused == false)
				{
					a->x += a->xspeed;
					a->y += a->yspeed;
					if (a->y + a->height - 50 >= SCRHEIGHT || a->x + 50 <= 0 || a->x + a->width - 50 >= SCRWIDTH)
					{
						a->x = rand() % SCRWIDTH;
						a->y = -(rand() % 40) - 40;
						a->width = (rand() % 30) + 30;
						a->height = (rand() % 30) + 30;
						a->xspeed = -(rand() % 10);
						a->yspeed = rand() % 10;
					}

					// Bullet - Asteroid collision
					SDL_Rect bull8 = { bullet.x, bullet.y, bullet.width, bullet.height };
					SDL_Rect asteroid = { a->x, a->y, a->width, a->height };

					if (checkCollision(bull8, asteroid))
					{
						asteroidExplosion.play(1, 0);
						score++;
						a->x = rand() % SCRWIDTH;
						a->y = -(rand() % 40) - 40;
						a->width = (rand() % 30) + 30;
						a->height = (rand() % 30) + 30;
						a->xspeed = -(rand() % 10);
						a->yspeed = rand() % 10;
						fired = false;
					}
					// Asteroid - Player collisiondd
					SDL_Rect player = { ship.x, ship.y, ship.width, ship.height };
					if (checkCollision(asteroid, player))
					{
						asteroidExplosion.play(1, 0);
						health -= 4;
						if (health <= 30)
						{
							healthColor.g = 0;
							healthColor.r = 255;
						}
						if (health <= 0)
							GAMESTATE = GameManager::RESTART_SCREEN;

						a->x = rand() % SCRWIDTH;
						a->y = -(rand() % 40) - 40;
						a->width = (rand() % 30) + 30;
						a->height = (rand() % 30) + 30;
						a->xspeed = -(rand() % 10);
						a->yspeed = rand() % 10;
					}
				}
				
			}
			// Update the enemies
			for (auto& e : enemies)
			{
				if (e->animations[0].isActive == false && paused == false)
				{
					e->y += e->yspeed;
					if (e->y + e->height - 50 >= SCRHEIGHT || e->x + 50 <= 0 || e->x + e->width - 50 >= SCRWIDTH)
					{
						e->x = rand() % SCRWIDTH;
						e->y = -(rand() % 40) - 40;
						e->xspeed = -(rand() % 10);
						e->yspeed = (rand() % 10) + 2;
					}
					// Enemy - Player colision
					SDL_Rect player = { ship.x, ship.y, ship.width, ship.height };
					SDL_Rect enemy = { e->x, e->y, e->width, e->height };

					if (checkCollision(player, enemy))
					{
						enemyExplosion.play(1, 0);
						health -= 10;
						if (health <= 30)
						{
							healthColor.g = 0;
							healthColor.r = 255;
						}
						if (health <= 0)
							GAMESTATE = GameManager::RESTART_SCREEN;

						e->x = rand() % (SCRWIDTH);
						e->y = -(rand() % 40) - 40;
						e->xspeed = -(rand() % 10);
						e->yspeed = (rand() % 10) + 2;
					}
					// Bullet - Enemy collision
					SDL_Rect bull8 = { bullet.x, bullet.y, bullet.width, bullet.height };

					if (checkCollision(bull8, enemy))
					{
						enemyExplosion.play(1, 0);
						score += 2;
						fired = false;

						e->playAnimation(0);
					}
				}
				else {
					if(paused == false)
					{
						e->updateAnimation(0);
						if (e->animations[0].isActive == false)
						{
							e->currentClipRect = defaultEnemyRect;
							e->x = rand() % (SCRWIDTH);
							e->y = -(rand() % 40) - 40;
							e->xspeed = -(rand() % 10);
							e->yspeed = (rand() % 10) + 2;
						}
					}
				}

			}
			if (health <= 50 && spawnPowerups == false && paused == false)
				spawnPowerups = true;

			// Powerups logic
			if (startPowerPowerup && paused == false)
			{
				counter++;
				if (counter > 60 * 10)
				{
					counter = 0;
					spawnPowerPowerup = true;
					startPowerPowerup = false;
				}
			}
			if (spawnPowerPowerup && paused == false)
			{
				powerPowerup.y += powerPowerup.yspeed;
				amod += da;
				if (amod > 255)
					da = -5;
				else if (amod < 100)
					da = 5;
				powerPowerup.setAlphaModulation(amod);
				if (powerPowerup.y + powerPowerup.height > SCRHEIGHT)
				{
					powerPowerup.x = rand() % SCRWIDTH;
					powerPowerup.y = -80;
					startPowerPowerup = true;
					spawnPowerPowerup = false;
				}
			}

			// Collision between power powerup and player ship
			SDL_Rect powerRect = {powerPowerup.x, powerPowerup.y, powerPowerup.width, powerPowerup.height};
			SDL_Rect playerRect = { ship.x, ship.y, ship.width, ship.height };
			if (checkCollision(powerRect, playerRect) && paused == false)
			{
				powerUpEffect = true;
				spawnPowerPowerup = false;
				startPowerPowerup = true;
				counter = -60*5;
				powerPowerup.x = rand() % SCRWIDTH;
				powerPowerup.y = -80;
			}
			if (powerUpEffect && paused == false)
			{
				// Flicker the player ship
				if (playerAlphaMod > 255)
					increment = -1;
				else if (playerAlphaMod < 100)
					increment = 1;

				playerAlphaMod += increment;
				ship.setAlphaModulation(playerAlphaMod);
				health = 80;
				healthColor = { 0, 255, 0, 255 };

				// Increase the speed
				speedMultiplier = 3;

				// Stop the powerup effect after 5 seconds (60*5 frames)
				counter2++;
				if (counter2 > 5 * 60)
				{
					counter2 = 0;
					powerUpEffect = false;
				}
			}
			else{
				if (paused == false)
				{
					speedMultiplier = 1;
					ship.setAlphaModulation(255);
				}
			}

			if (spawnPowerups && paused == false)
			{
				amod += da;
				if (amod > 255)
					da = -5;
				else if (amod < 100)
					da = 5;

				healthPickup.setAlphaModulation(amod);
				healthPickup.y += healthPickup.yspeed;
				if (healthPickup.y + healthPickup.height - 50 >= SCRHEIGHT || healthPickup.x + 50 <= 0 || healthPickup.x + healthPickup.width - 50 >= SCRWIDTH)
				{
					healthPickup.x = rand() % (SCRWIDTH);
					healthPickup.y = -80;
					healthPickup.yspeed = (rand() % 2) + 1;
				}
				// Health pickup - Player collision
				SDL_Rect player = { ship.x, ship.y, ship.width, ship.height };
				SDL_Rect hp = { healthPickup.x, healthPickup.y, healthPickup.width, healthPickup.height };
				if (checkCollision(hp, player) && paused == false)
				{
					health = 80;
					healthColor.r = 0;
					healthColor.g = 255;
					healthPickup.x = rand() % (SCRWIDTH);
					healthPickup.y = -80;
					healthPickup.yspeed = (rand() % 2) + 1;
					spawnPowerups = false;
				}
			}
			text = "Score: " + std::to_string(score);
			scoreText.setText(text.c_str());

			// Draw the current frame
			if (!paused)
			{
				scrollingOffset--;
				if (scrollingOffset <= -SCRHEIGHT)
					scrollingOffset = 0;
			}
	
			background.draw(0, scrollingOffset);
			background.draw(0, SCRHEIGHT + scrollingOffset);

			for (auto& a : asteroids)
				a->draw();
			for (auto& e : enemies)
				e->draw(e->x, e->y, e->currentClipRect);

			if (fired)
				bullet.draw(angle);
			ship.draw(angle);
			// The health bar
			if (health >= 0)
			{
				window.drawFilledRect(ship.x, ship.y + ship.height + 10, health, 10, healthColor.r, healthColor.g, healthColor.b, healthColor.a);
				window.drawRectOutline(ship.x, ship.y + ship.height + 10, 80, 10, 255, 255, 255, 255);
			}
			scoreText.draw();

			// Draw the powerups  
			if (spawnPowerups)
				healthPickup.draw();
			powerPowerup.draw();
			window.drawRectOutline(20, 20, SCRWIDTH - 40, SCRHEIGHT - 40, 255, 255, 255, 255);

			if (paused)
				pausedText.draw();

			// Swap buffers
			window.display();
		}
		if (GAMESTATE == GameManager::RESTART_SCREEN)
		{
			// Clear the back buffer
			window.clear(0, 0, 0, 255);

			// Poll events
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					running = false;
				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					if (event.button.button == SDL_BUTTON_LEFT && quitButton.isMouseOver())
					{
						clickMouse.play(1, 0);
						SDL_Delay(40);
						running = false;
					}
					if (event.button.button == SDL_BUTTON_LEFT && restartButton.isMouseOver())
					{
						clickMouse.play(1, 0);
						GAMESTATE = GameManager::GAME;
						// Reset player's and enemies's states
						score = 0;
						health = 80;
						healthColor = { 0, 255, 0, 255 };

						ship.x = SCRWIDTH / 2 - ship.width / 2;
						ship.y = SCRHEIGHT / 2 - ship.height / 2;
						ship.xspeed = 0;
						ship.yspeed = -5;
						ship.rotAngle = 0;
						angle = 0;

						powerPowerup.x = rand() % SCRWIDTH;
						powerPowerup.y = -60;
						powerUpEffect = false;
						spawnPowerPowerup = false, startPowerPowerup = true, powerUpEffect = false;
						counter = 0;
						increment = 1, playerAlphaMod = 255;
						counter2 = 0;
						
						bullet.x = ship.x + bullet.width * 2;
						bullet.y = ship.y + bullet.height;
						bullet.xspeed = 0;
						bullet.yspeed = -20;
						bullet.rotAngle = 0;

						for (auto& a : asteroids)
							delete a;
						for (auto& e : enemies)
							delete e;
						asteroids.clear();
						enemies.clear();

						for (int i = 0; i < 10; i++)
							addAsteroid(asteroids);
						for (int i = 0; i < 5; i++)
							addEnemy(enemies);

						spawnPowerups = false;
						healthPickup.x = rand() % SCRWIDTH;
						healthPickup.y = -80;
						healthPickup.xspeed = 0;
						healthPickup.yspeed = (rand() % 2) + 1;
					}
					if (event.button.button == SDL_BUTTON_LEFT && menuButton.isMouseOver())
					{
						clickMouse.play(1, 0);
						GAMESTATE = GameManager::TITLE_SCREEN;
						// Reset player's and enemies's states
						score = 0;
						health = 80;
						healthColor = { 0, 255, 0, 255 };

						ship.x = SCRWIDTH / 2 - ship.width / 2;
						ship.y = SCRHEIGHT / 2 - ship.height / 2;
						ship.xspeed = 0;
						ship.yspeed = -5;
						ship.rotAngle = 0;
						angle = 0;

						powerPowerup.x = rand() % SCRWIDTH;
						powerPowerup.y = -60;
						powerUpEffect = false;
						spawnPowerPowerup = false, startPowerPowerup = true, powerUpEffect = false;
						counter = 0;
						increment = 1, playerAlphaMod = 255;
						counter2 = 0;

						bullet.x = ship.x + bullet.width * 2;
						bullet.y = ship.y + bullet.height;
						bullet.xspeed = 0;
						bullet.yspeed = -20;
						bullet.rotAngle = 0;

						for (auto& a : asteroids)
							delete a;
						for (auto& e : enemies)
							delete e;
						asteroids.clear();
						enemies.clear();

						for (int i = 0; i < 10; i++)
							addAsteroid(asteroids);
						for (int i = 0; i < 5; i++)
							addEnemy(enemies);

						spawnPowerups = false;
						healthPickup.x = rand() % SCRWIDTH;
						healthPickup.y = -80;
						healthPickup.xspeed = 0;
						healthPickup.yspeed = (rand() % 2) + 1;
					}
				}
			}

			// Draw the current frame
			scrollingOffset--;
			if (scrollingOffset <= -SCRHEIGHT)
				scrollingOffset = 0;

			background.draw(0, scrollingOffset);
			background.draw(0, SCRHEIGHT + scrollingOffset);
			window.drawRectOutline(20, 20, SCRWIDTH - 40, SCRHEIGHT - 40, 255, 255, 255, 255);

			// Change the alpha of the button if mouse cursor is over it
			if (restartButton.isMouseOver())
			{
				if (rh)
				{
					hoverMouse.play(2, 0);
					rh = false;
				}
				restartButton.setAlphaModulation(150);
				shipUI.x = restartButton.getXPos() - 40;
				shipUI.y = restartButton.getYPos() + 10;
				shipUI.draw();
			}
			else
			{
				restartButton.setAlphaModulation(255);
				rh = true;
			}
			if (quitButton.isMouseOver())
			{
				if (qh)
				{
					hoverMouse.play(2, 0);
					qh = false;
				}
				quitButton.setAlphaModulation(150);
				shipUI.x = quitButton.getXPos() - 40;
				shipUI.y = quitButton.getYPos() + 10;
				shipUI.draw();
			}
			else
			{
				quitButton.setAlphaModulation(255);
				qh = true;
			}
			if (menuButton.isMouseOver())
			{
				if (mh)
				{
					hoverMouse.play(2, 0);
					mh = false;
				}
				menuButton.setAlphaModulation(150);
				shipUI.x = menuButton.getXPos() - 40;
				shipUI.y = menuButton.getYPos() + 10;
				shipUI.draw();
			}
			else
			{
				menuButton.setAlphaModulation(255);
				mh = true;
			}
			gameoverText.draw();
			restartButton.draw();
			quitButton.setPosition(quitButton.getXPos(), 560);
			quitButton.draw();
			menuButton.draw();

			std::string text = "Score: " + std::to_string(score);
			finalScoreText.setText(text.c_str());
			finalScoreText.draw();

			// Swap buffers
			window.display();
			if(GAMESTATE != GameManager::RESTART_SCREEN)
				quitButton.setPosition(quitButton.getXPos(), 545);
		}
	}

	// Clean up the memory allocated for all the asteroids
	for (auto& a : asteroids)
		delete a;
	for (auto& e : enemies)
		delete e;

	return 0;
}