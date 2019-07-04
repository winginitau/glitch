/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: Debug.cpp
 - A start at abstracted debug routines for glitch
 - TODO: probably redundant
*******************************************************************/

#include <Debug.h>
#include <string.h>


Debug::Debug() {
	// Auto-generated constructor stub

}

Debug::~Debug() {
	// Auto-generated destructor stub
}

char* Debug::GetFromStringArray(char* dest, const EnumStringArray* arr, int n) {
	return strcpy(dest, &(*arr[n].text));
}
