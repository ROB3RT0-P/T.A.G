/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include <string>
#include "utils.h"

class Console
{
public:
	Console() {};
	~Console();

	bool initConsole();
	int manageInput(char userInput);

	std::string getConsoleOutput() { return consoleOutput; }

protected:
	

private:
	char userInput;
	int maxConsoleLen;
	std::string consoleOutput;
	Utils* utils;
};