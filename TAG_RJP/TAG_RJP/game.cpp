/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#include "game.h"

bool Game::initialize(int ScreenWidth, int ScreenHeight)
{
	bool success = true;
	SCREEN_WIDTH_ = ScreenWidth;
	SCREEN_HEIGHT_ = ScreenHeight;

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
		gWindow = SDL_CreateWindow("Text Adventure Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH_, SCREEN_HEIGHT_, SDL_WINDOW_SHOWN);
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
	playerTexture_ = global::resourceManager()->getResourceAsTexture(raw_enum(global::Res::PlayerSprite));
	backgroundTexture_ = global::resourceManager()->getResourceAsTexture(raw_enum(global::Res::BackgroundSprite));

	// RJP - Temporary texture loading - This is to be moved over to the Resource Manager.
	SDL_Surface* surface = IMG_Load( "Data/textures/main_menu.png" );
	mainMenuTexture_ = SDL_CreateTextureFromSurface(gRenderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load( "Data/textures/pathways.png" );
	backgroundTexture_ = SDL_CreateTextureFromSurface( gRenderer, surface );
	SDL_FreeSurface(surface);

	surface = IMG_Load( "Data/textures/game_over.png");
	gameOverTexture_ = SDL_CreateTextureFromSurface(gRenderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load( "Data/textures/dead_end.png" );
	deadEndTexture_ = SDL_CreateTextureFromSurface(gRenderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("Data/textures/continue.png");
	continueTexture_ = SDL_CreateTextureFromSurface(gRenderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("Data/textures/escape.png");
	escapeTexture_ = SDL_CreateTextureFromSurface(gRenderer, surface);
	SDL_FreeSurface(surface);

	//playerEntity = static_cast<Player*>(global::entityManager()->createEntity(raw_enum(global::EntityType::Player)));
	playerEntity = new Player(); // RJP - For testing demo. To be added to Entity Manager later.
	playerEntity->init();

	stateMachine = new StateMachine();

	audio = new AudioPlayer();
	textureRenderer = new Renderer( gRenderer );
	console = new Console(*stateMachine, *playerEntity);
	console->initConsole();

	debugTextSize_ = 20;
	textSize_ = 35;
	textColor_ = { 255, 255, 255, 255 };
	debugTextColor_ = { 0, 255, 0, 255 };
	debugText = new Text(gRenderer, "Data/kenney/Fonts/kenneyPixel.ttf", debugTextSize_, debugTextColor_);
	gameText = new Text(gRenderer, "Data/kenney/Fonts/kenneyPixel.ttf", textSize_, textColor_);
	consoleText = new Text(gRenderer, "Data/kenney/Fonts/kenneyPixel.ttf", textSize_, textColor_);

	controls = new Controls();
	utils = new Utils();
	
	bMusicPlaying_ = false;
	bStateSwitch_ = false;
	uCurrentTime = 0;
	uInitialTime = 0;
	uStateTimer = 4000;

	fScreenWidth_ = static_cast<float>( SCREEN_WIDTH_ );
	fScreenHeight_ = static_cast<float>( SCREEN_HEIGHT_ );
	
	consoleRenderPosX_ = fScreenWidth_ * 0.1f;
	consoleRenderPosY_ = fScreenHeight_ * 0.9f;

	return true;
}

void Game::start(const Info& info) 
{
	stateMachine->setState( GameState::MENU );

#ifdef _DEBUG
	//stateMachine->setState(GameState::ESCAPE);
#endif

}

void Game::tickLogic( float deltaTime ) 
{
	global::processManager()->tickProcesses();
	
	switch ( stateMachine->getState() ) 
	{
		case GameState::MENU:
			if (!bMusicPlaying_)
			{
				audio->play("Data/music/Ice_Demon.mp3");
				bMusicPlaying_ = true;
			}
			break;

		case GameState::PLAY:
			bStateSwitch_ = false;
			if (!bMusicPlaying_)
			{
				audio->play("Data/music/SCP-x2x.mp3");
				bMusicPlaying_ = true;
			}

			if (playerEntity->getPlayerTurnsRemaining() < 1)
			{
				audio->stop();
				bMusicPlaying_ = false;
			}

			break;

		case GameState::PAUSE:

			break;

		case GameState::GAMEOVER:
			if (!bMusicPlaying_)
			{
				audio->play("Data/music/Gathering_Darkness.mp3");
				bMusicPlaying_ = true;
			}
			break;
		case GameState::DEADEND:
			if (!bStateSwitch_)
			{
				uInitialTime = SDL_GetTicks();
				bStateSwitch_ = true;
			}
			uCurrentTime = SDL_GetTicks();

			if ((uCurrentTime - uInitialTime) > uStateTimer)
			{
				stateMachine->setState(GameState::PLAY);
			}

			break;

		case GameState::CONTINUE:
			if (!bStateSwitch_)
			{
				uInitialTime = SDL_GetTicks();
				bStateSwitch_ = true;
			}
			uCurrentTime = SDL_GetTicks();

			if ((uCurrentTime - uInitialTime) > uStateTimer)
			{
				stateMachine->setState(GameState::PLAY);
			}

			break;
		case GameState::ESCAPE:
			if (!bMusicPlaying_)
			{
				audio->play("Data/music/Night_of_Chaos.mp3");
				bMusicPlaying_ = true;
			}

			break;

		}
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
		SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 255 ); // RJP - Default background.
		textureRenderer->textureRender( mainMenuTexture_, fScreenWidth_ * 0.0f, fScreenHeight_ * 0.1f, 0.28f, 0.28f ); // RJP - offsetting values needs reworking - this is not scalable.
		gameText->RenderConsoleText( "Peril's Passage", static_cast<int>( fScreenWidth_ * 0.41f ), static_cast<int>( fScreenHeight_ * 0.05f ) );
		gameText->RenderConsoleText( "Seeketh thou liberation? - [ TAB ]", static_cast<int>( fScreenWidth_ * 0.29f ), static_cast<int>( fScreenHeight_ * 0.9f ) );
#ifdef _DEBUG
		debugText->RenderDebugText("MAIN_MENU", 10, 10);
#endif
		break;

	case GameState::PLAY:
		// RJP - TODO - Fade to go here eventually.
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); // RJP - Default background.
#ifdef _DEBUG
		debugText->RenderDebugText("PLAYING", 10, 10);
#endif
		textureRenderer->textureRender(backgroundTexture_, fScreenWidth_ * 0.0f, fScreenHeight_ * 0.1f, 0.8f, 0.75f);
		gameText->RenderGameText("In sooth, there be twain paths afore thee...", static_cast<int>(fScreenWidth_ * 0.05f), static_cast<int>(fScreenHeight_ * 0.05f));
		gameText->RenderGameText("Choices remaining: " + std::to_string(playerEntity->getPlayerTurnsRemaining()), static_cast<int>(fScreenWidth_ * 0.4f), static_cast<int>(fScreenHeight_ * 0.9f));
		// RJP - Render multiple previous lines - Add prevConsoleOutput to a list and loop through the list with an offset on the y axis.
		gameText->RenderGameText(console->getPrevConsoleOutput(), static_cast<int>(consoleRenderPosX_), static_cast<int>(consoleRenderPosY_ - 30 ) );
		consoleText->RenderConsoleText(console->getConsoleOutput(), static_cast<int>(consoleRenderPosX_), static_cast<int>(consoleRenderPosY_));
		
		if( console )

		break;
	case GameState::PAUSE:
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
#ifdef _DEBUG
		debugText->RenderConsoleText("PAUSE_MENU", 10, 10);
#endif

		gameText->RenderConsoleText("Input - [ A ] ~ [ Z ]", static_cast<int>(fScreenWidth_ * 0.1f), static_cast<int>(fScreenHeight_ * 0.4f));
		gameText->RenderConsoleText("Confirm - [ RETURN ]", static_cast<int>(fScreenWidth_ * 0.1f), static_cast<int>(fScreenHeight_ * 0.5f));
		gameText->RenderConsoleText("Pause / Unpause - [ TAB ]", static_cast<int>(fScreenWidth_ * 0.1f), static_cast<int>(fScreenHeight_ * 0.6f));
		gameText->RenderConsoleText("Quit - [ ESC ]", static_cast<int>(fScreenWidth_ * 0.1f), static_cast<int>(fScreenHeight_ * 0.7f));
		gameText->RenderConsoleText("Make your way to the exit before your luck runs out...", static_cast<int>(fScreenWidth_ * 0.1f), static_cast<int>(fScreenHeight_ * 0.8f));

		break;
	case GameState::GAMEOVER:
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		textureRenderer->textureRender(gameOverTexture_, fScreenWidth_ * 0.0f, fScreenHeight_ * 0.1f, 1.35f, 1.45f);
#ifdef _DEBUG
		debugText->RenderConsoleText("GAME_OVER", 10, 10);
#endif
		gameText->RenderConsoleText("Thy fortune hath forsaken thee...", static_cast<int>(fScreenWidth_ * 0.29f), static_cast<int>(fScreenHeight_ * 0.9f));
		break;

	case GameState::DEADEND:
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
#ifdef _DEBUG
		debugText->RenderDebugText("DEAD_END", 10, 10);
#endif
		textureRenderer->textureRender(deadEndTexture_, fScreenWidth_ * 0.0f, fScreenHeight_ * 0.1f, 0.75f, 0.65f);
		gameText->RenderGameText("Dead end...", static_cast<int>(fScreenWidth_ * 0.05f), static_cast<int>(fScreenHeight_ * 0.05f));
		gameText->RenderGameText("Choices remaining: " + std::to_string(playerEntity->getPlayerTurnsRemaining()), static_cast<int>(fScreenWidth_ * 0.4f), static_cast<int>(fScreenHeight_ * 0.9f));
		break;

	case GameState::CONTINUE:
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
#ifdef _DEBUG
		debugText->RenderDebugText("CONTINUE", 10, 10);
#endif
		textureRenderer->textureRender(continueTexture_, fScreenWidth_ * 0.0f, fScreenHeight_ * 0.1f, 0.8f, 0.65f);
		gameText->RenderGameText("Fortune is in thy favour...", static_cast<int>(fScreenWidth_ * 0.05f), static_cast<int>(fScreenHeight_ * 0.05f));
		gameText->RenderGameText("Choices remaining: " + std::to_string(playerEntity->getPlayerTurnsRemaining()), static_cast<int>(fScreenWidth_ * 0.4f), static_cast<int>(fScreenHeight_ * 0.9f));

		break;

	case GameState::INTRO:

		break;
	
	case GameState::ESCAPE:
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
#ifdef _DEBUG
		debugText->RenderDebugText("ESCAPE", 10, 10);
#endif
		textureRenderer->textureRender(escapeTexture_, fScreenWidth_ * 0.0f, fScreenHeight_ * 0.1f, 0.85f, 0.65f);
		gameText->RenderGameText("Fate hath spared thee for another day...", static_cast<int>(fScreenWidth_ * 0.25f), static_cast<int>(fScreenHeight_ * 0.85f));
		break;
	}
}

void Game::postFrameUpdate()
{
	global::processManager()->endOfFrameCleanup();
}

bool Game::handleEvents(float deltaTime)
{
	userInput_ = '\0';

	switch (stateMachine->getState())
	{
	case GameState::MENU:
		while (SDL_PollEvent(&event_) != 0)
		{
			userInput_ = controls->handleInput(event_);
			if (userInput_ == '\a' && stateMachine->getState() == GameState::MENU)
			{
				stateMachine->setState(GameState::PAUSE);
				audio->stop();
				bMusicPlaying_ = false;
			}
		}

		if (console->manageInput(userInput_)) return 1;

		break;

	case GameState::PLAY:

		while (SDL_PollEvent(&event_) != 0)
		{
			userInput_ = controls->handleInput(event_);
			if (userInput_ == '\a' && stateMachine->getState() == GameState::PLAY) stateMachine->setState(GameState::PAUSE);
			if (userInput_ != '\0') break;
		}

		if (console->manageInput(userInput_)) return 1;

#ifdef _DEBUG

		if (console->getPrevConsoleOutput() == "die")
		{
			audio->stop();
			bMusicPlaying_ = false;
		}

		if (console->getPrevConsoleOutput() == "r")
		{
			stateMachine->setState(GameState::CONTINUE);
		}
		
		if (console->getPrevConsoleOutput() == "l")
		{
			stateMachine->setState(GameState::DEADEND);
		}
#endif

		break;
	case GameState::PAUSE:

		while (SDL_PollEvent(&event_) != 0)
		{
			userInput_ = controls->handleInput(event_);
			if (userInput_ == '\a' && stateMachine->getState() == GameState::PAUSE) stateMachine->setState(GameState::PLAY);
		}

		if (console->manageInput(userInput_)) return 1;

		break;
	case GameState::GAMEOVER:
		while (SDL_PollEvent(&event_) != 0)
		{
			userInput_ = controls->handleInput(event_);
			if (userInput_ == '\a' && stateMachine->getState() == GameState::GAMEOVER) stateMachine->setState(GameState::MENU);
		}

		if (console->manageInput(userInput_)) return 1;

		break;
	case GameState::INTRO:

		break;
	case GameState::DEADEND:

		break;
	case GameState::CONTINUE:

		break;
	case GameState::ESCAPE:

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
	playerTexture_ = nullptr;
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
