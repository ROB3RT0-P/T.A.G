/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#include "audio.h"

AudioPlayer::AudioPlayer() 
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0) 
    {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) 
    {
        SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }

    music = nullptr;
}

AudioPlayer::~AudioPlayer() 
{
    if (music != nullptr) 
    {
        Mix_FreeMusic(music);
    }

    Mix_CloseAudio();

    SDL_Quit();
}

void AudioPlayer::play(const char* filePath) 
{
    music = Mix_LoadMUS(filePath);
    if (music == nullptr) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Music missing.", SDL_GetError(), nullptr);
        return;
    }

    Mix_PlayMusic(music, 0);
}

void AudioPlayer::pause() 
{
    Mix_PauseMusic();
}

void AudioPlayer::resume() 
{
    Mix_ResumeMusic();
}

void AudioPlayer::stop() 
{
    Mix_HaltMusic();
}
