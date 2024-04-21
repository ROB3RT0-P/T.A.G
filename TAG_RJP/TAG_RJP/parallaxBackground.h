/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <iostream>

class ParallaxBackground {
public:
    ParallaxBackground(SDL_Renderer* renderer, const std::vector<std::string>& imagePaths, int scrollSpeed);
    ~ParallaxBackground();

    void scroll(int scrollSpeed);
    void zoom(int zoomSpeed, bool zoomInOut);
    void randMovement(int speed);
    void titleRender();
    void render();

private:
    SDL_Renderer* renderer;
    std::vector<SDL_Texture*> layers;

    int iScrollSpeed_;
    int iTotalHeight_;
    int iYOffset_;
    int iScreenHeight_;
};
