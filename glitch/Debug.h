/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: Debug.h
 - A start at abstracted debug routines for glitch
 - TODO: probably redundant
*******************************************************************/

#ifndef DEBUG_H_
#define DEBUG_H_

#include <glitch_config.h>

class Debug {
public:
	Debug();
	virtual ~Debug();

	char* GetFromStringArray(char* dest, const EnumStringArray* arr, int n);

};

#endif /* DEBUG_H_ */
