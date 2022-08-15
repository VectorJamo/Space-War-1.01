#include "Game.h"

Game::Game()
	:running(true), window(nullptr)
{

	// Initialize the window
	window = new Window(SCRWIDTH, SCRHEIGHT, "Space Maina");
	window->setFrameRateLimit(60);

	GAMESTATE = GameManager::MAIN_MENU;
}
Game::~Game()
{
	freeMemory();

	// Always delete the window last so that all loaded audio, textures, fonts get freed before closing their respective subsystems
	delete window;

}
void Game::freeMemory()
{
	delete background;
	background = nullptr;

	delete player;
	player = nullptr;
	delete bullet;
	bullet = nullptr;

	for (auto& e : enemies)
	{
		delete e;
		e = nullptr;
	}
	enemies.clear();
	for (auto& eb : nukes)
	{
		delete eb;
		eb = nullptr;
	}
	nukes.clear();

	delete health;
	health = nullptr;
	delete score;
	score = nullptr;

	delete gameMusic;
	gameMusic = nullptr;
	delete enemyExplosion;
	enemyExplosion = nullptr;
	delete nukeExplosion;
	nukeExplosion = nullptr;
	delete playerHurt;
	playerHurt = nullptr;
	delete buttonClick;
	buttonClick = nullptr;

	delete menuBackground;
	menuBackground = nullptr;
	delete gameText;
	gameText = nullptr;
	delete play;
	play = nullptr;
	delete controls;
	controls = nullptr;
	delete quit;
	quit = nullptr;

	delete controlsImage;
	controlsImage = nullptr;
	delete back;
	back = nullptr;
	delete endText;
	endText = nullptr;
	delete restart;
	restart = nullptr;
}

void Game::init()
{

}
void Game::setup()
{
	srand(time(0));

	// Intialize sprites
	Sprite::UseRenderer(window->getRenderer());
	background = new Sprite("res/images/textures/space-background.png");
	
	// Player sprite
	player = new Sprite("res/images/textures/player.png");
	player->setSize(100, 75);
	player->setXPos((SCRWIDTH / 2) - (player->getWidth() / 2));
	player->setYPos(SCRHEIGHT - (player->getHeight()));
	player->setVelocity(5, 5);
	kills = 0;

	// Bullet sprite
	bullet = new Sprite("res/images/textures/bullet.png");
	bullet->setSize(20, 30);
	bullet->setXPos(player->getXPos() + 40); 
	bullet->setYPos(player->getYPos());
	bullet->setVelocity(0, 10);
	launched = false;

	// Enemies
	for (int i = 0; i < 5; i++)
	{
		Sprite* enemy = new Sprite("res/images/textures/ufo.png");
		enemy->setSize(80, 30);
		enemy->setXPos(abs(rand() % SCRWIDTH-enemy->getWidth()));
		enemy->setYPos(abs(rand() % SCRHEIGHT/2));
		enemy->setVelocity(5, 50);
		enemies.push_back(enemy);
	}

	// Nukes
	for (int i = 0; i < 3; i++)
	{
		Sprite* nuke = new Sprite("res/images/textures/nuke.png");
		nuke->setSize(45, 60);
		nuke->setXPos(abs(rand() % SCRWIDTH - nuke->getWidth()));
		nuke->setYPos(abs(rand() % SCRHEIGHT / 2) - 500);
		nuke->setVelocity((rand()%10)-(rand()%15), 10);
		nukes.push_back(nuke);
	}

	// Initialize the UI 
	Text::UseRenderer(window->getRenderer());
	// Health bar and text 
	healthPosX = 30;
	healthPosY = 20;
	healthWidth = 150;
	healthHeight = 30;
	healthText = "Ship Health";
	health = new Text("res/fonts/Pointless.ttf", 48, healthText.c_str(), 30, 60, 180, 30, 220, 210, 215, 255);

	// Score
	scoreText = "Score: " + std::to_string(kills);
	score = new Text("res/fonts/Pointless.ttf", 48, scoreText.c_str(), SCRWIDTH-200, 50, 120, 30, 220, 210, 215, 255);

	// Music
	gameMusic = new Music("res/audio/Game-music.ogg");
	gameMusic->play(-1);

	enemyExplosion = new SoundEffect("res/audio/bullet-enemy-explosion.wav");
	nukeExplosion = new SoundEffect("res/audio/bullet-nuke-explosion.wav");
	playerHurt = new SoundEffect("res/audio/player-hurt.wav");
	laserShoot = new SoundEffect("res/audio/laser-shoot.wav");
	isLaserAudioPlayed = false;
	buttonClick = new SoundEffect("res/audio/button-click.wav");

	// Menu
	Button::UseRenderer(window->getRenderer());
	menuBackground = new Sprite("res/images/textures/space-background-menu.png");
	gameText = new Sprite("res/images/textures/space-mania.png");

	play = new Button((SCRWIDTH / 2) - (100 / 2), 200, 100, 50, 90, 209, 219, 255, "res/fonts/Pointless.ttf", 24, "Play", 255, 255, 255, 150);
	play->setBlendMode(SDL_BLENDMODE_BLEND);
	controls = new Button((SCRWIDTH / 2) - (150/ 2), 300, 150, 50, 90, 209, 219, 255, "res/fonts/Pointless.ttf", 24, "Controls", 255, 255, 255, 150);
	quit = new Button((SCRWIDTH / 2) - (100 / 2), 400, 100, 40, 90, 209, 219, 255, "res/fonts/Pointless.ttf", 24, "Quit", 255, 255, 255, 150);

	controlsImage = new Sprite("res/images/textures/controls.png");
	controlsImage->setColorModulation(180, 255, 255);
	back = new Button((SCRWIDTH / 2) - (130 / 2), 450, 130, 40, 90, 209, 219, 255, "res/fonts/Pointless.ttf", 24, "Back", 255, 255, 255, 150);

	endText = new Text("res/fonts/Pointless.ttf", 48, "You died! Score:", (SCRWIDTH/2) - 400/2, 200, 400, 60, 220, 210, 215, 255);
	restart = new Button((SCRWIDTH / 2) - (150 / 2), 300, 150, 50, 90, 209, 219, 255, "res/fonts/Pointless.ttf", 24, "Restart", 255, 255, 255, 150);
	wasEnded = false;
}

void Game::update()
{
	menu();
	controlsMenu();
	game();
	endMenu();
	
}

void Game::handleInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			running = false;
	}
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_A] && player->getXPos() > 0)
		player->setXPos(player->getXPos() - player->getVelX());
	if (keyState[SDL_SCANCODE_D] && (player->getXPos() + player->getWidth() < SCRWIDTH))
		player->setXPos(player->getXPos() + player->getVelX());
	if (keyState[SDL_SCANCODE_W] && (player->getYPos() > 0))
		player->setYPos(player->getYPos() - player->getVelY());
	if (keyState[SDL_SCANCODE_S] && (player->getYPos() + player->getHeight() < SCRHEIGHT))
		player->setYPos(player->getYPos() + player->getVelY());

	if (keyState[SDL_SCANCODE_SPACE])
		launched = true;
}
void Game::menu()
{
	if (GAMESTATE == GameManager::MAIN_MENU)
	{
		// Main Menu 
		while (running)
		{
			window->clear(0, 0, 0, 0);
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					running = false;

				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					if (event.button.button == SDL_BUTTON_LEFT && play->isMouseOver)
					{
						buttonClick->play(-1, 0);
						SDL_Delay(100);
						GAMESTATE = GameManager::GAME;
					}
					if (event.button.button == SDL_BUTTON_LEFT && controls->isMouseOver)
					{
						GAMESTATE = GameManager::CONTROLS;
						buttonClick->play(-1, 0);
					}
					if (event.button.button == SDL_BUTTON_LEFT && quit->isMouseOver)
					{
						buttonClick->play(-1, 0);
						SDL_Delay(100);
						running = false;
					}

				}
			}

			if (play->isMouseOver)
				play->setAlpha(100);
			else
				play->setAlpha(220);

			if (controls->isMouseOver)
				controls->setAlpha(100);
			else
				controls->setAlpha(220);

			if (quit->isMouseOver)
				quit->setAlpha(100);
			else
				quit->setAlpha(220);


			// Draw
			menuBackground->draw(0, 0);
			gameText->draw(200, 0);
			play->draw();
			controls->draw();
			quit->draw();

			window->display();
			if (GAMESTATE != GameManager::MAIN_MENU)
				update();
		}
	}
}
void Game::controlsMenu()
{
	if (GAMESTATE == GameManager::CONTROLS)
	{

		// Controls
		while (running)
		{
			window->clear(0, 0, 0, 0);

			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					running = false;
				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					if (event.button.button == SDL_BUTTON_LEFT && back->isMouseOver)
					{
						GAMESTATE = GameManager::MAIN_MENU;
						buttonClick->play(-1, 0);
					}
				}
			}
			 
			if (back->isMouseOver)
				back->setAlpha(100);
			else
				back->setAlpha(220);
			

			//Draw
			menuBackground->draw(0, 0);
			controlsImage->draw(0, 0);
			back->draw();

			window->display();
			if (GAMESTATE != GameManager::CONTROLS)
				update();
		}
	}
}
void Game::game()
{
	if (wasEnded)
	{
		freeMemory();
		setup();
		wasEnded = false;
	}
	if (GAMESTATE == GameManager::GAME)
	{
		while (running)
		{
			// Clear previous frame ->
			window->clear(0, 0, 0, 0);

			// Handle user inputs ->
			handleInput();

			// Move ->
			if ((launched == true) && (bullet->getYPos() > 0))
			{
				if (!isLaserAudioPlayed)
					laserShoot->play(-1, 0);
				isLaserAudioPlayed = true;
				bullet->isEnabled = true;
				bullet->setYPos(bullet->getYPos() - bullet->getVelY());
			}
			else {
				launched = false;
				bullet->isEnabled = false;
				isLaserAudioPlayed = false;
				bullet->setXPos(player->getXPos() + 40);
				bullet->setYPos(player->getYPos());
			}
			for (auto& e : enemies)
			{
				e->setXPos(e->getXPos() + e->getVelX());

				// Check if enemy goes outside the bounds of the window
				if (e->getXPos() + e->getWidth() >= SCRWIDTH)
				{
					e->setVelocity(-abs(e->getVelX()), 50);
					e->setYPos(e->getYPos() + e->getVelY());
				}
				else if (e->getXPos() < 0)
				{
					e->setVelocity(abs(e->getVelX()), 50);
					e->setYPos(e->getYPos() + e->getVelY());
				}
				// Check if enemy goes below the window
				if (e->getYPos() + e->getHeight() >= SCRHEIGHT)
				{
					// Respawn the ones which have gone off the window
					e->setYPos(rand() % 100);
					e->setXPos(rand() % 600);
				}
				// Check for collision
				if (e->isCollided(bullet))
				{
					enemyExplosion->play(-1, 0);
					kills++;
					launched = false;
					// Respawn the hitted ones with extra velocity
					e->setYPos(rand() % 100);
					e->setXPos(rand() % 600);
					if (abs(e->getVelX()) <= 8)
						e->setVelocity((abs(e->getVelX()) + 2), e->getVelY());
				}
				// Player enemy collision
				if (e->isCollided(player))
				{
					playerHurt->play(-1, 0);
					e->setYPos(rand() % 100);
					e->setXPos(rand() % 600);
					if (abs(e->getVelX()) <= 8)
						e->setVelocity((abs(e->getVelX()) + 2), e->getVelY());
					// Decrease the health bar width
					healthWidth -= 30;
					if (healthWidth <= 0)
						GAMESTATE = GameManager::END;
				}
			}

			if (kills > 5)
			{
				for (auto& eb : nukes)
				{
					eb->setXPos(eb->getXPos() + eb->getVelX());
					eb->setYPos(eb->getYPos() + eb->getVelY());

					// Check if the boss goes outside of the window
					if ((eb->getYPos() + eb->getHeight()) >= SCRHEIGHT)
					{
						// Spawn the boss in a random position
						eb->setXPos(abs(rand() % SCRWIDTH - eb->getWidth()));
						eb->setYPos(abs(rand() % SCRHEIGHT / 2) - 500);
						eb->setVelocity((rand() % 10) - (rand() % 15), 10);
					}
					// Check for collision ->
					if (eb->isCollided(player))
					{
						playerHurt->play(-1, 0);
						// Spawn the boss in a random position
						eb->setXPos(abs(rand() % SCRWIDTH - eb->getWidth()));
						eb->setYPos(abs(rand() % SCRHEIGHT / 2) - 500);
						eb->setVelocity((rand() % 10) - (rand() % 15), 10);

						// Decrease the heath bar width
						healthWidth -= 15;
						if (healthWidth <= 0)
							GAMESTATE = GameManager::END;
					}
					

					if (eb->isCollided(bullet))
					{
						nukeExplosion->play(-1, 0);
						launched = false;
						kills++;
						// Spawn the boss in a random position
						eb->setXPos(abs(rand() % SCRWIDTH - eb->getWidth()));
						eb->setYPos(abs(rand() % SCRHEIGHT / 2) - 500);
						eb->setVelocity((rand() % 10) - (rand() % 15), 10);
					}
				}
			}

			// Update the UI ->
			scoreText = "Score: " + std::to_string(kills);
			score->setText(scoreText.c_str());

			// Draw ->
			background->draw(0, 0);

			player->draw();
			for (auto& e : enemies)
				e->draw();

			bullet->draw();
			// Draw the nukes after certain number of kills
			if (kills > 5)
			{
				for (auto& eb : nukes)
					eb->draw();
			}

			// Draw the UI
			health->draw();
			score->draw();

			window->drawRectOutline(30, 20, 150, 30, 255, 255, 255, 255);
			window->drawFilledRect(healthPosX, healthPosY, healthWidth, healthHeight, 89, 227, 91, 255);

			// Swap buffers ->
			window->display();
			if (GAMESTATE != GameManager::GAME)
				update();

		}
	}
}

void Game::endMenu()
{
	back->setPosition((SCRWIDTH / 2) - (130 / 2), 400);
	if (GAMESTATE == GameManager::END)
	{
		while (running)
		{
			window->clear(0, 0, 0, 0);
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					running = false;

				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					if (event.button.button == SDL_BUTTON_LEFT && back->isMouseOver)
					{
						GAMESTATE = GameManager::MAIN_MENU;
						buttonClick->play(-1, 0);
						wasEnded = true;
					}
					if (event.button.button == SDL_BUTTON_LEFT && restart->isMouseOver)
					{
						buttonClick->play(-1, 0);
						SDL_Delay(100);
						freeMemory();
						setup();
						GAMESTATE = GameManager::GAME;
					}
				}
			}
			if (GAMESTATE == GameManager::END)
			{
				if (restart->isMouseOver)
					restart->setAlpha(100);
				else
					restart->setAlpha(220);

				if (back->isMouseOver)
					back->setAlpha(100);
				else
					back->setAlpha(220);

				//Draw
				menuBackground->draw(0, 0);

				std::string text = "You Died! Final Score: " + std::to_string(kills);
				endText->setText(text.c_str());
				endText->draw();
				restart->draw();
				back->draw();

			}
			window->display();

			if (GAMESTATE != GameManager::END)
				update();
		}
	}
		
}
void Game::run()
{
	setup();
	update();
}