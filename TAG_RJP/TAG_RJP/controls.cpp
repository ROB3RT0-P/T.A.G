/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#include "controls.h"

Controls::Controls(Player& playerEntity) : player(playerEntity) {}

int Controls::handleInput(SDL_Event& event, float deltaTime)
{
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_LEFT:
            player.setPlayerVelocityX(-player.getPlayerAcceleration());
            return 0;
            break;
        case SDLK_RIGHT:
            player.setPlayerVelocityX(player.getPlayerAcceleration());
            return 0;
            break;
        case SDLK_UP:
            player.setPlayerVelocityY(-player.getPlayerAcceleration());
            return 0;
            break;
        case SDLK_DOWN:
            player.setPlayerVelocityY(player.getPlayerAcceleration());
            return 0;
            break;
        case SDLK_SPACE:
            //player.shoot(deltaTime);
            return 0;
            break;
        case SDLK_ESCAPE:
            return 1; // RJP - Shutdown game.
            break;
        case SDLK_RETURN:
            return 2;
            break;
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_UP:
        case SDLK_DOWN:
            player.setPlayerVelocityY(0);
            break;
        case SDLK_RIGHT:
        case SDLK_LEFT:
            player.setPlayerVelocityX(0);
            break;
        }
        return 0;
    }

    return 0;
}
