/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#pragma once

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class Text 
{
public:
    Text(SDL_Renderer* renderer, const std::string& fontPath, int fontSize, SDL_Color textColor);
    ~Text();

    void RenderText(std::string& text, int x, int y);

private:
    SDL_Renderer* renderer_;
    TTF_Font* font_;
    SDL_Color textColor_;
};