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
	bool checkChoice();

	std::string getPrevConsoleOutput() { return sPrevConsoleOutput; }
	std::string getConsoleOutput() { return sConsoleOutput; }

protected:

private:
	Utils* utils;
	Player* player_;
	StateMachine* stateMachine_;

	int iMaxConsoleLen;
	int iRandChoice;

	std::string sConsoleOutput;
	std::string sInputCheckL;
	std::string sInputCheckR;
	std::string sPrevConsoleOutput;

#ifdef _DEBUG
	std::string sInputCheckDie;
	std::string sInputCheckWin;
#endif
};
	


	