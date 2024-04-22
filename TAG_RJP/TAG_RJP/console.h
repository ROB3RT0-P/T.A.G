/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include "player.h"
#include "gameState.h"
#include <string>
#include <iostream>
#include <vector>
#include "utils.h"

class Console
{
public:
	Console(StateMachine &stateMachine, Player &player) : stateMachine_(&stateMachine), player_(&player) 
	{
		initConsole();
	};
	~Console();

	bool initConsole();
	int manageInput(char userInput);
	void updateGame();
	bool checkPlayerState();
	void checkChoice();
	inline void setEscapeNum(int iNewEscapeNum) { iEscapeNum_ = iNewEscapeNum; }

	std::string getPrevConsoleOutput() { return sPrevConsoleOutput_; }
	std::string getConsoleOutput() { return sConsoleOutput_; }

protected:

private:
	Player* player_;
	StateMachine* stateMachine_;

	int iMaxConsoleLen_;
	int iRandChoice_;
	Uint32 iEscapeNum_;

	std::vector<std::string> vConsoleOutput_;

	std::string sConsoleOutput_;
	std::string sInputCheckL_;
	std::string sInputCheckR_;
	std::string sPrevConsoleOutput_;

#ifdef _DEBUG
	std::string sInputCheckDie_;
	std::string sInputCheckWin_;
#endif
};
	


	