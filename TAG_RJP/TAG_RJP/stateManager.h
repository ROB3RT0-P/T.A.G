/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#pragma once
#include "gameState.h"

class StateManager {
public:
    StateManager();

    void setGameState(GameState state);
    GameState getGameState() const;

private:
    GameState currentState;
};