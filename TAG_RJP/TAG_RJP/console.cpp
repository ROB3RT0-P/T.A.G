/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#include "console.h"

bool Console::initConsole() 
{
	utils = new Utils();
	maxConsoleLen = 25;
	consoleOutput = "";
	return true;
}

int Console::manageInput(char userInput)
{
	if (userInput != '\0')
	{
		if (userInput == '\b' && !consoleOutput.empty())
		{
			consoleOutput.pop_back(); // RJP - Backspace - Remove last letter.
			return 0;
		}
		else if (userInput == '\r')
		{
			// RJP - Use console value.
			consoleOutput = ""; // RJP - Reset console value after pressing enter.
			return 0;
		}
		else if (userInput == '\t') 
		{
			return 1; // RJP - Close the game.
		}
		else if (utils->stringLen(consoleOutput) < maxConsoleLen)
		{
			consoleOutput += userInput;
			return 0;
		}
	}
}
