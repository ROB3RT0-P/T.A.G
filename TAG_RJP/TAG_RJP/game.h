/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include "loadingprocess.h"
#include "player.h"
#include "controls.h"
#include "parallaxBackground.h"
#include "text.h"
#include "audio.h"
#include "global.h"
#include "processmanager.h"
#include "resourcemanager.h"
#include "entitymanager.h"
#include "renderer.h"
#include "gameState.h"
#include "utils.h"
#include "console.h"

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>

// Forwards
struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;

class Game : 
	public LoadingProcess::ILoadingProcessCallback, 
	public IProcess
{
public:
	Game() : quit_(false), playerEntity() {}

	bool initialize(int ScreenWidth, int ScreenHeight);
	void tickLogic(float deltaTime);
	void blit(SDL_Texture* texture, float x, float y);
	void renderAndPresent();
	void postFrameUpdate();
	void close();
	bool handleEvents(float deltaTime);

private:
	bool loadInitialResources();

	virtual void start(const Info& info);;
	virtual void tick(const Info& info) {};
	virtual void render(const Info& info);
	virtual void endOfFrameUpdate(const Info& info) {};
	virtual void completed(const Info& info) {};

	virtual void onLoadComplete(LoadingProcess::LoadRequest* loadedResources, size_t count);

	LoadingProcess loadingProcess;

	Player* playerEntity;

	SDL_Window*				gWindow = NULL;
	SDL_Renderer*			gRenderer = NULL;
	SDL_Texture*			gTexture = NULL;

	TTF_Font*				font;
	Text*					debugText;
	Text*					gameText;
	Text*					consoleText;

	SDL_Texture*			playerTexture_;
	SDL_Texture*			backgroundTexture_;
	SDL_Texture*			mainMenuTexture_;
	SDL_Texture*			gameOverTexture_;
	SDL_Texture*			deadEndTexture_;
	SDL_Texture*			continueTexture_;
	SDL_Texture*			escapeTexture_;

	SDL_Color				textColor_;
	SDL_Color				debugTextColor_;
	SDL_Event				event_;

	AudioPlayer*			audio;
	Controls*				controls;
	ParallaxBackground*		background;
	ParallaxBackground*		titleBackground;
	Renderer*				textureRenderer;
	StateMachine*			stateMachine;
	Utils*					utils;
	Console*				console;

	std::string sPlayerScore_;
	std::string sPlayerEntityHealth_;

	std::vector<std::string> vsImagePaths_;
	std::vector<std::string> vsTitleImagePaths_;

	Uint32 uInitialTime_;
	Uint32 uCurrentTime_;
	Uint32 uStateTimer_;

	int SCREEN_WIDTH_ = 0;
	int SCREEN_HEIGHT_ = 0;
	int scrollSpeed_ = 1;
	int prevTime_;
	int gameTime_ = 0;
	int debugTextSize_;
	int textSize_;
	int iInputReturn_;

	float fConsoleRenderPosX_;
	float fConsoleRenderPosY_;
	float fScreenWidth_;
	float fScreenHeight_;

	bool bQuit_;
	bool bMusicPlaying_;
	bool bStateSwitch_;

	char cUserInput_;
};