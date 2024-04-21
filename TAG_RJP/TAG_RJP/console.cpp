/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#include "console.h"

bool Console::initConsole() 
{
	utils = new Utils();

	iMaxConsoleLen = 48;
	sConsoleOutput = "";
	sInputCheckL = "left";
	sInputCheckR = "right";
	iRandChoice = rand();

#ifdef _DEBUG
	sInputCheckDie = "die";
	sInputCheckWin = "win";
#endif

	return true;
}

int Console::manageInput(char userInput)
{
	if ( userInput != '\0' )
	{
		if ( userInput == '\b' && !sConsoleOutput.empty() )
		{
			sConsoleOutput.pop_back(); // RJP - Backspace - Remove last letter.
			return 0;
		}
		else if ( userInput == '\r' )
		{
			updateGame();
			sPrevConsoleOutput = sConsoleOutput;
			sConsoleOutput.clear(); // RJP - Enter - Reset console value after pressing enter.
			return 0;
		}
		else if ( userInput == '\t' ) 
		{
			return 1; // RJP - Close the game.
		}
		else if ( userInput == '\a')
		{
			// RJP - Do nothing. // RJP - Tab - Change state.
			return 0;
		}
		else if ( utils->stringLen(sConsoleOutput) < iMaxConsoleLen )
		{
			sConsoleOutput += userInput;
			return 0;
		}
		return 0;
	}
	return 0;
}

void Console::updateGame()
{
	if (sConsoleOutput == sInputCheckL )
	{
		player_->decrementPlayerTurnsRemaining();
		checkChoice() ? stateMachine_->setState(GameState::CONTINUE) : stateMachine_->setState(GameState::DEADEND);
		if (checkPlayerState()) stateMachine_->setState(GameState::GAMEOVER);
	}
	else if (sConsoleOutput == sInputCheckR )
	{	
		player_->decrementPlayerTurnsRemaining();
		checkChoice() ? stateMachine_->setState(GameState::CONTINUE) : stateMachine_->setState(GameState::DEADEND);
		if ( checkPlayerState() ) stateMachine_->setState(GameState::GAMEOVER);
	}
	
#ifdef _DEBUG
	else if (sConsoleOutput == sInputCheckDie)
	{
		stateMachine_->setState(GameState::GAMEOVER);
	}
	else if (sConsoleOutput == sInputCheckWin)
	{
		stateMachine_->setState(GameState::MENU);
	}
#endif

	else
	{
		// RJP - TODO - Print "incorrect command".
	}
}

bool Console::checkPlayerState()
{
	if (player_->getPlayerTurnsRemaining() < 1)
	{
		return true; // RJP - Player is dead.
	}
	return false;
}

bool Console::checkChoice()
{
	iRandChoice = rand();

	if (iRandChoice % 2 == 0) {
		return true; // RJP - Number is even.
	}
	else {
		return false; // RJP - Number is odd.
	}
}