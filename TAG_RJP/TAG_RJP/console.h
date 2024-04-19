/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include "player.h"
#include "gameState.h"
#include <string>
#include "utils.h"

class Console
{
public:
	Console( StateMachine &stateMachine, Player &player ) : stateMachine_(&stateMachine), player_(&player) {};
	~Console();

	bool initConsole();
	int manageInput(char userInput);
	void updateGame();
	bool checkPlayerState();

	std::string getPrevConsoleOutput() { return prevConsoleOutput; }
	std::string getConsoleOutput() { return consoleOutput; }

protected:

private:
	char userInput;
	int maxConsoleLen;
	std::string consoleOutput;
	std::string inputCheckL;
	std::string inputCheckR;

#ifdef _DEBUG
	std::string inputCheckDie;
	std::string inputCheckWin;
#endif

	std::string prevConsoleOutput;
	Utils* utils;

	Player* player_;
	StateMachine* stateMachine_;
};