/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#include "controls.h"

char Controls::handleInput( SDL_Event &event )
{
    char input = '\0';

    if ( event.type == SDL_KEYDOWN ) 
    {
        SDL_Keysym keysym = event.key.keysym;

        switch ( keysym.sym ) 
        {
            case SDLK_TAB:
                input = '\a';
                break;
            case SDLK_RETURN:
                input = '\r';
                break;
            case SDLK_ESCAPE:
                input = '\t';
                break;
            case SDLK_BACKSPACE:
                input = '\b';
                break;
            default:
                if ( ( keysym.sym >= SDLK_SPACE && keysym.sym <= SDLK_z ) || ( keysym.sym >= SDLK_0 && keysym.sym <= SDLK_9 ) ) 
                {
                    input = static_cast<char>( keysym.sym );
                }
                break;
            }
    }

    return input;
}

