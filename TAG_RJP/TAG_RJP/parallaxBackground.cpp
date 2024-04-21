/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#include "parallaxBackground.h"

ParallaxBackground::ParallaxBackground(SDL_Renderer* renderer, const std::vector<std::string>& imagePaths, int scrollSpeed)
    : renderer(renderer), iScreenHeight_(0), iYOffset_(0), iScrollSpeed_(0), iTotalHeight_(0)
{
    for (const auto& path : imagePaths) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (surface == nullptr) {
            SDL_Log("Failed to load image: %s", SDL_GetError());
            //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to load image: %s", SDL_GetError(), nullptr);
            continue;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if (texture == nullptr) {
            SDL_Log("Failed to create texture: %s", SDL_GetError());
            continue;
        }
        
        layers.push_back(texture);
        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        iTotalHeight_ += h;
    }
}

ParallaxBackground::~ParallaxBackground() 
{
    for (auto texture : layers) {
        SDL_DestroyTexture(texture);
    }
}

void ParallaxBackground::scroll(int scrollSpeed) 
{
    iYOffset_ += scrollSpeed;
    if (iYOffset_ > iTotalHeight_) {
        iYOffset_ = 0;
    }
}

void ParallaxBackground::zoom(int zoomSpeed, bool zoomInOut)
{
    float zoomFactor = 1.0f;
    if (zoomInOut) 
    {
        zoomFactor += zoomSpeed * 0.01f;
    }
    else 
    {
        zoomFactor -= zoomSpeed * 0.01f;
    }

    for (SDL_Texture* texture : layers) 
    {
        int oldWidth, oldHeight;
        SDL_QueryTexture(texture, nullptr, nullptr, &oldWidth, &oldHeight);

        int newWidth = static_cast<int>(oldWidth * zoomFactor);
        int newHeight = static_cast<int>(oldHeight * zoomFactor);

        SDL_Rect dstRect = { 0, 0, newWidth, newHeight };

        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    }
}

void ParallaxBackground::titleRender()
{
    // RJP - To be changed.
    int y = iYOffset_;
    int bC = 1160; // RJP - backgroundCenterer
    for (SDL_Texture* texture : layers) {
        SDL_Rect dstRect = { 0, y, 700, 1000 };
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
        y += 600;
    }
}

void ParallaxBackground::randMovement(int speed)
{
    // RJP - To be changed.
    iYOffset_ += iScrollSpeed_;
    if (iYOffset_ > iTotalHeight_) {
        iYOffset_ = 0;
    }
}

void ParallaxBackground::render() 
{
    int y = iYOffset_;
    int bC = 1160; // RJP - backgroundCenterer
    for (SDL_Texture* texture : layers) {
        SDL_Rect dstRect = { 0, y - bC, 700, 2500 };
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
        y += 600;
    }
}
