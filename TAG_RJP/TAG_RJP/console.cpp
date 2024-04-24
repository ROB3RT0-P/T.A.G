/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#include "console.h"

bool Console::initConsole() 
{
	iMaxConsoleLen_ = 48;
	sConsoleOutput_ = "";
	sInputCheckL_ = "left";
	sInputCheckR_ = "right";
	iRandChoice_ = rand();
	iEscapeNum_ = 8;

#ifdef _DEBUG
	sInputCheckDie_ = "die";
	sInputCheckWin_ = "win";
#endif

	return true;
}

int Console::manageInput(char userInput)
{
	if ( userInput != '\0' )
	{
		if ( userInput == '\b' && !sConsoleOutput_.empty() )
		{
			sConsoleOutput_.pop_back(); // RJP - Backspace - Remove last letter.
			return 0;
		}
		else if ( userInput == '\r' )
		{
			updateGame();
			sPrevConsoleOutput_ = sConsoleOutput_;
			vConsoleOutput_.push_back(sConsoleOutput_); // RJP - Adding outputs to a list in case it's needed later.
			sConsoleOutput_.clear(); // RJP - Enter - Reset console value after pressing enter.
			return 0;
		}
		else if ( userInput == '\t' ) 
		{
			return 1; // RJP - Close the game.
		}
		else if ( userInput == '\a')
		{
			return 0; // RJP - Do nothing.
		}
		else if ( Utils::stringLen(sConsoleOutput_) < iMaxConsoleLen_ )
		{
			sConsoleOutput_ += userInput;
			return 0;
		}
		return 0;
	}
	return 0;
}

void Console::updateGame()
{
	if (sConsoleOutput_ == sInputCheckL_ || sConsoleOutput_ == sInputCheckR_ )
	{
		checkChoice();
		checkPlayerState();
	}
	
#ifdef _DEBUG
	else if (sConsoleOutput_ == sInputCheckDie_)
	{
		stateMachine_->setState(GameState::GAMEOVER);
	}
	else if (sConsoleOutput_ == sInputCheckWin_)
	{
		stateMachine_->setState(GameState::ESCAPE);
	}
#endif

	else
	{
		// RJP - Do nothing.
		// RJP - TODO - Print "incorrect command".
	}
}

bool Console::checkPlayerState()
{
	if (player_->getPlayerCorrectTurns() >= iEscapeNum_)
	{
		stateMachine_->setState(GameState::ESCAPE);
		return true;
	}
	if (player_->getPlayerTurnsRemaining() < 1 && stateMachine_->getState() != GameState::ESCAPE)
	{
		stateMachine_->setState(GameState::GAMEOVER);
		return true;
	}
	return false;
}

void Console::checkChoice()
{
	iRandChoice_ = rand();
	player_->decrementPlayerTurnsRemaining();

	if (iRandChoice_ % 2 == 0) 
	{
		player_->incrementPlayerCorrectTurns();
		stateMachine_->setState(GameState::CONTINUE);
	}
	else 
	{
		stateMachine_->setState(GameState::DEADEND);
	}
}

template<typename T>
bool Console::isEqual(const T& a, const T& b) {
	return a == b;
}