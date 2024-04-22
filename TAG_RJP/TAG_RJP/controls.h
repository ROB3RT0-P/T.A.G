/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include <SDL.h>

class Controls
{
public:
	Controls() {};
	static char handleInput(SDL_Event& event);

private:
};