/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#pragma once

enum class GameState {
    MENU,
    PLAY,
    PAUSE,
    GAMEOVER,
    DEADEND,
    CONTINUE,
    INTRO,
    ESCAPE,
};

class StateMachine {
public:
    StateMachine() : gsCurrentState(GameState::MENU) {}

    inline void setState(GameState newState) { gsCurrentState = newState; }
    inline GameState getState() const { return gsCurrentState; }

private:
    GameState gsCurrentState;
};
