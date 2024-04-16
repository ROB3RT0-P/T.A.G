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
	Renderer(SDL_Renderer* renderer)
	{
		gRenderer = renderer;
	}
	~Renderer();

	void textureRender(SDL_Texture* texture, float x, float y)
	{
		SDL_Rect dest;

		dest.x = static_cast<int>(x);
		dest.y = static_cast<int>(y);
		SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

		SDL_RenderCopy(gRenderer, texture, NULL, &dest);
	}

private:
	SDL_Renderer* gRenderer = NULL;
};