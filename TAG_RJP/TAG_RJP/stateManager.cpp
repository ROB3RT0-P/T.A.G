/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

// RJP - I may switch over to using this later.

#include "stateManager.h"

StateManager::StateManager() : currentState(GameState::START) {}

void StateManager::setGameState(GameState state) {
    currentState = state;
}

GameState StateManager::getGameState() const {
    return currentState;
}
