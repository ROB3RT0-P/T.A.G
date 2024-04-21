/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>

class Renderer
{
public:
	Renderer(SDL_Renderer* renderer) : gRenderer(renderer) {};
	~Renderer();

	void textureRender(SDL_Texture* texture, float x, float y, float scaleX, float scaleY);
	void fadeInOut(SDL_Renderer* renderer, int duration, bool fadeIn, float screenWidth, float screenHeight);

private:
	SDL_Renderer* gRenderer = NULL;
};