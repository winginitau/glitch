/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: common_config.h
 - Shared enum symbols used in glitch and itch
*******************************************************************/

#ifndef COMMON_CONFIG_H_
#define COMMON_CONFIG_H_

#if defined(PLATFORM_LINUX)
#elif defined(PLATFORM_ARDUINO)
#include <Arduino.h>
#else
#error "A PLATFORM_ Directive must be provided"
#endif

// parser result codes

enum {
	R_ERROR = 0,
	R_NONE,
	R_UNFINISHED,
	R_COMPLETE,
	R_IGNORE,
	R_DISCARD,
	R_HELP,
	R_CONTINUE,
	R_REPLAY,
	R_BACKSPACE,
	R_EXIT,
};

// AST types shared across grammar processor and parser
enum {
	AST_UNDEFINED = 0,
	AST_KEYWORD,
	AST_ENUM_ARRAY,
	AST_LOOKUP,
	AST_PARAM_DATE,
	AST_PARAM_TIME,
	AST_PARAM_INTEGER,
	AST_PARAM_UINT8,
	AST_PARAM_UINT16,
	AST_PARAM_UINT32,
	AST_PARAM_INT16,
	AST_PARAM_INT32,
	AST_PARAM_FLOAT,
	AST_PARAM_STRING,
	LAST_AST_TYPE,
};


#endif /* COMMON_CONFIG_H_ */
