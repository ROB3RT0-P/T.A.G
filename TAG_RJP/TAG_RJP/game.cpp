/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#include "game.h"

bool Game::initialize(int ScreenWidth, int ScreenHeight)
{
	bool success = true;
	SCREEN_WIDTH = ScreenWidth;
	SCREEN_HEIGHT = ScreenHeight;

	// RJP - Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// RJP - Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		// RJP - Create window
		gWindow = SDL_CreateWindow("Text Adventure Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// RJP - Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				// RJP - Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// RJP - Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				global::resourceManager()->initializeLoaderData(gRenderer);
			}
		}
	}

	return success && loadInitialResources();
}

bool Game::loadInitialResources()
{
	loadingProcess.callback = this;
	LoadingProcess::LoadRequest toLoad;
	toLoad.resource = global::Res::Default;
	loadingProcess.dataToLoad.push_back(toLoad);
	toLoad.resource = global::Res::PlayerSprite;
	loadingProcess.dataToLoad.push_back(toLoad);

	global::processManager()->registerProcess(&loadingProcess, raw_enum(global::TaskID::Loading), raw_enum(global::TickOrder::DontCare), raw_enum(global::RenderOrder::DontCare));

	//imagePaths = { "Data/textures/", "", ""};
	background = new ParallaxBackground(gRenderer, imagePaths, 1);
	titleBackground = new ParallaxBackground(gRenderer, imagePaths, 0);

	stateMachine = new StateMachine();

	debugTextSize = 35;
	debugTextSizeInGame = 25;
	textColor = { 255, 255, 255, 255 };
	debugText, consoleText = new Text(gRenderer, "Data/kenney/Fonts/kenneyBlocks.ttf", debugTextSize, textColor);
	audio = new AudioPlayer();
	audio->play("Data/music/titleMusic.mp3");

	console = new Console();
	console->initConsole();

	controls = new Controls();
	utils = new Utils();
	
	return true;
}

void Game::start(const Info& info) {
	playerEntity = static_cast<Player*>(global::entityManager()->createEntity(raw_enum(global::EntityType::Player)));

	fScreenWidth = static_cast<float>( SCREEN_WIDTH );
	fScreenHeight = static_cast<float>( SCREEN_HEIGHT );
	
	consoleRenderPosX = fScreenWidth * 0.1f;
	consoleRenderPosY = fScreenHeight * 0.9f;

	stateMachine->setState( GameState::MENU );
}

void Game::tickLogic( float deltaTime ) 
{
	global::processManager()->tickProcesses();

	switch ( stateMachine->getState() ) 
	{
		case GameState::MENU:
			
			break;

		case GameState::PLAY:
			handleEvents( deltaTime );
			break;

		case GameState::PAUSE:

			break;

		case GameState::GAMEOVER:
	
			break;
		}
}

void Game::blit(SDL_Texture* texture, float x, float y)
{
	SDL_Rect dest;

	dest.x = static_cast<int>(x);
	dest.y = static_cast<int>(y);
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy( gRenderer, texture, NULL, &dest );
}

void Game::renderAndPresent()
{	
	SDL_RenderClear(gRenderer);
	global::processManager()->renderProcesses();
	SDL_RenderPresent(gRenderer);
}

void Game::render(const Info& info)
{
	switch (stateMachine->getState())
	{
	case GameState::MENU:
		break;

	case GameState::PLAY:
		playerTexture = global::resourceManager()->getResourceAsTexture(raw_enum(global::Res::PlayerSprite));

		consoleText->RenderConsoleText(console->getConsoleOutput(), static_cast<int>(consoleRenderPosX), static_cast<int>(consoleRenderPosY));
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		break;
	case GameState::PAUSE:

		break;
	case GameState::GAMEOVER:

		break;
	}
}

void Game::postFrameUpdate()
{
	global::processManager()->endOfFrameCleanup();
}

bool Game::handleEvents(float deltaTime)
{
	userInput = '\0';

	switch (stateMachine->getState())
	{
	case GameState::MENU:
		while (SDL_PollEvent(&event) != 0)
		{
			userInput = controls->handleInput(event);
			if (userInput == '\r' && stateMachine->getState() == GameState::MENU)
			{
				stateMachine->setState(GameState::PLAY);

			}
		}

		if (console->manageInput(userInput)) return 1;

		break;

	case GameState::PLAY:

		while (SDL_PollEvent(&event) != 0)
		{
			userInput = controls->handleInput(event);
			if (userInput == '\r' && stateMachine->getState() == GameState::PLAY) stateMachine->setState(GameState::PAUSE);
			if (userInput != '\0') break;
		}

		if (console->manageInput(userInput)) return 1;

		break;
	case GameState::PAUSE:

		while (SDL_PollEvent(&event) != 0)
		{
			userInput = controls->handleInput(event);
			if (userInput == '\r' && stateMachine->getState() == GameState::PAUSE) stateMachine->setState(GameState::PLAY);
		}

		if (console->manageInput(userInput)) return 1;

		break;
	case GameState::GAMEOVER:
		while (SDL_PollEvent(&event) != 0)
		{
			userInput = controls->handleInput(event);
			if (userInput == '\r' && stateMachine->getState() == GameState::GAMEOVER) stateMachine->setState(GameState::MENU);
		}

		if (console->manageInput(userInput)) return 1;

		break;
	}
	return 0;
}

void Game::close()
{
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;
	font = nullptr;
	debugText = nullptr;
	playerTexture = nullptr;
	audio = nullptr;
	controls = nullptr;
	background = nullptr;
	titleBackground = nullptr;
	
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}


void Game::onLoadComplete(LoadingProcess::LoadRequest* loadedResources, size_t count)
{
	gTexture = global::resourceManager()->getResourceAsTexture(raw_enum(global::Res::Default));
	global::processManager()->registerProcess(this, raw_enum(global::TaskID::DontCare), raw_enum(global::TickOrder::DontCare), raw_enum(global::RenderOrder::DontCare));
}
