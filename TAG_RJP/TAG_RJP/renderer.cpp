/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#include "renderer.h"

void Renderer::textureRender( SDL_Texture* texture, float x, float y , float scaleX, float scaleY)
{
	if (texture)
	{
		SDL_Rect dest;

		dest.x = static_cast<int>(x);
		dest.y = static_cast<int>(y);
		SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

		dest.w *= scaleX;
		dest.h *= scaleY;

		SDL_RenderCopy(gRenderer, texture, NULL, &dest);
	}
	else 
	{
		SDL_Log("Failed to create texture: %s", SDL_GetError());
	}
}

void Renderer::fadeInOut(SDL_Renderer* renderer, int duration, bool fadeIn, float screenWidth, float screenHeight) 
{

}
