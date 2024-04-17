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

	debugTextSize = 35;
	debugTextSizeInGame = 25;
	textColor = { 255, 255, 255, 255 };
	debugText, consoleText = new Text(gRenderer, "Data/kenney/Fonts/kenneyBlocks.ttf", debugTextSize, textColor);
	audio = new AudioPlayer();
	audio->play("Data/music/titleMusic.mp3");

	controls = new Controls();
	
	//userInput = "exampleText";
	consoleOutput = "";
	
	return true;
}

void Game::start(const Info& info) {
	playerEntity = static_cast<Player*>(global::entityManager()->createEntity(raw_enum(global::EntityType::Player)));

	fScreenWidth = static_cast<float>(SCREEN_WIDTH);
	fScreenHeight = static_cast<float>(SCREEN_HEIGHT);
}

void Game::tickLogic(float deltaTime) 
{
	global::processManager()->tickProcesses();

	controls->handleInput(event);

	// RJP - This needs to be done in the console class.
	char inputChar = controls->handleInput(event);
	if (inputChar != '\0') 
	{
		char* tempUserInput = nullptr;
		if (userInput) {
			size_t userInputLength = strlen(userInput);

			tempUserInput = new char[userInputLength + 2];

			strcpy_s(tempUserInput, userInputLength + 1, userInput);
		}
		else {
			tempUserInput = new char[2];
		}

		tempUserInput[strlen(tempUserInput)] = inputChar;
		tempUserInput[strlen(tempUserInput) + 1] = '\0';

		delete[] userInput;

		userInput = tempUserInput;

		consoleOutput = userInput;
	}
}

void Game::blit(SDL_Texture* texture, float x, float y)
{
	SDL_Rect dest;

	dest.x = static_cast<int>(x);
	dest.y = static_cast<int>(y);
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(gRenderer, texture, NULL, &dest);
}

void Game::renderAndPresent()
{	
	SDL_RenderClear(gRenderer);
	global::processManager()->renderProcesses();
	SDL_RenderPresent(gRenderer);
}

void Game::render(const Info& info)
{
	playerTexture = global::resourceManager()->getResourceAsTexture(raw_enum(global::Res::PlayerSprite));
	consoleText->RenderText(consoleOutput, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
}

void Game::postFrameUpdate()
{
	global::processManager()->endOfFrameCleanup();
}

void Game::close()
{
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	font = NULL;
	debugText = NULL;
	playerTexture = NULL;
	audio = NULL;
	controls = NULL;
	background = NULL;
	titleBackground = NULL;

	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

bool Game::handleEvents(float deltaTime) 
{
	while (SDL_PollEvent(&event) != 0)
	{
		controls->handleInput(event);
	}

	return 0;
}

void Game::onLoadComplete(LoadingProcess::LoadRequest* loadedResources, size_t count)
{
	gTexture = global::resourceManager()->getResourceAsTexture(raw_enum(global::Res::Default));
	global::processManager()->registerProcess(this, raw_enum(global::TaskID::DontCare), raw_enum(global::TickOrder::DontCare), raw_enum(global::RenderOrder::DontCare));
}
