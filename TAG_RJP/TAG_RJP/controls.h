/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include "player.h"
#include <SDL.h>

class Controls
{
	public:
		Controls(Player& player);

		int handleInput(SDL_Event& event, float deltaTime);

	private:
		Player& player;
};