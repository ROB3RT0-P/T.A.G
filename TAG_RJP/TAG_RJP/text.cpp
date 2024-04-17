/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#include "text.h"

Text::Text(SDL_Renderer* renderer, const std::string& fontPath, int fontSize, SDL_Color textColor) 
{
    if (TTF_Init() == -1) 
    {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
    }

    font_ = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font_) 
    {
        std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << std::endl;
    }

    renderer_ = renderer;
    textColor_ = textColor;
}

Text::~Text() 
{
    TTF_CloseFont(font_);
    TTF_Quit();
}

void Text::RenderText(std::string& text, int x, int y)
{
    int xOffset = 0;
    for (char ch : text) 
    {
        SDL_Surface* surface = TTF_RenderGlyph_Solid(font_, ch, textColor_);
        if (!surface) 
        {
            std::cerr << "TTF_RenderGlyph_Solid failed: " << TTF_GetError() << std::endl;
            continue;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
        if (!texture) 
        {
            std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            continue;
        }

        SDL_Rect destRect = { x + xOffset, y, surface->w, surface->h };
        SDL_RenderCopy(renderer_, texture, NULL, &destRect);

        xOffset += surface->w;

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}