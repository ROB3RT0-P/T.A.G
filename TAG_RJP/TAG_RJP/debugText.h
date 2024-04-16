/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#pragma once

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class DebugText {
public:
    DebugText(SDL_Renderer* renderer, const std::string& fontPath, int fontSize, SDL_Color textColor) {
        if (TTF_Init() == -1) {
            std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
            // Handle error appropriately
        }

        font_ = TTF_OpenFont(fontPath.c_str(), fontSize);
        if (!font_) {
            std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << std::endl;
            // Handle error appropriately
        }

        renderer_ = renderer;
        textColor_ = textColor;
    }

    ~DebugText() {
        TTF_CloseFont(font_);
        TTF_Quit();
    }

    void RenderText(const std::string& text, int x, int y) {
        SDL_Surface* surface = TTF_RenderText_Solid(font_, text.c_str(), textColor_);
        if (!surface) {
            std::cerr << "TTF_RenderText_Solid failed: " << TTF_GetError() << std::endl;
            // Handle error appropriately
            return;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
        if (!texture) {
            std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
            // Handle error appropriately
            SDL_FreeSurface(surface);
            return;
        }

        SDL_Rect destRect = { x, y, surface->w, surface->h };
        SDL_RenderCopy(renderer_, texture, NULL, &destRect);

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

private:
    SDL_Renderer* renderer_;
    TTF_Font* font_;
    SDL_Color textColor_;
};