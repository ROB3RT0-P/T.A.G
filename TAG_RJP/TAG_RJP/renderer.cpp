/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#include "renderer.h"

void Renderer::textureRender(SDL_Texture* texture, float x, float y)
{
	SDL_Rect dest;

	dest.x = static_cast<int>(x);
	dest.y = static_cast<int>(y);
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(gRenderer, texture, NULL, &dest);
}