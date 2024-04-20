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
    StateMachine() : currentState(GameState::MENU) {}

    void setState(GameState newState) {
        currentState = newState;
    }

    GameState getState() const {
        return currentState;
    }

private:
    GameState currentState;
};
