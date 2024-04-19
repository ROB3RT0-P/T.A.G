/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#include "console.h"

bool Console::initConsole() 
{
	utils = new Utils();
	maxConsoleLen = 48;
	consoleOutput = "";
	inputCheckL = "left";
	inputCheckR = "right";

#ifdef _DEBUG
	inputCheckDie = "die";
	inputCheckWin = "win";
#endif

	return true;
}

int Console::manageInput(char userInput)
{
	if ( userInput != '\0' )
	{
		if ( userInput == '\b' && !consoleOutput.empty() )
		{
			consoleOutput.pop_back(); // RJP - Backspace - Remove last letter.
			return 0;
		}
		else if ( userInput == '\r' )
		{
			updateGame();
			prevConsoleOutput = consoleOutput;
			consoleOutput.clear(); // RJP - Enter - Reset console value after pressing enter.
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
		else if ( utils->stringLen(consoleOutput) < maxConsoleLen )
		{
			consoleOutput += userInput;
			return 0;
		}
		return 0;
	}
}

void Console::updateGame()
{
	/*
	if (consoleOutput == "left" || "LEFT" || "l" || "L")
	{
		player_->decrementPlayerTurnsRemaining();
		if (checkPlayerState()) stateMachine_->setState(GameState::GAMEOVER);
	}
	else if (consoleOutput == "right" || "RIGHT" || "r" || "R")
	{
		player_->decrementPlayerTurnsRemaining();
		if ( checkPlayerState() ) stateMachine_->setState(GameState::GAMEOVER);
	}
	*/

	if (consoleOutput == inputCheckL )
	{
		// RJP - TODO - Check if input was correct.
		player_->decrementPlayerTurnsRemaining();
		if (checkPlayerState()) stateMachine_->setState(GameState::GAMEOVER);
	}
	else if (consoleOutput == inputCheckR )
	{
		// RJP - TODO - Check if input was correct.
		player_->decrementPlayerTurnsRemaining();
		if ( checkPlayerState() ) stateMachine_->setState(GameState::GAMEOVER);
	}
	
#ifdef _DEBUG
	else if (consoleOutput == "die")
	{
		stateMachine_->setState(GameState::GAMEOVER);
	}
	else if (consoleOutput == "win")
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
		return true;
	}
	return false;
}
