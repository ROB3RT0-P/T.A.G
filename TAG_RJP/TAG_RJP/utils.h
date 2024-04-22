/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include <string>

class Utils 
{
public:

	static int stringLen(std::string str)
	{
        int length = 0;
        for (int i = 0; str[i] != '\0'; ++i) {
            ++length;
        }
        return length;
	}

private:

};