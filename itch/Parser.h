/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: Parser.h
 - Parsing of char inputs according to the grammar
 - Leveraging NodeMap and its processing results
 - Forming and calling of actions into project code
*******************************************************************/

#ifndef PARSER_H_
#define PARSER_H_

#include <out.h>

#if defined(PLATFORM_LINUX)
#elif defined(PLATFORM_ARDUINO)
#include <Arduino.h>
#else
#error "A PLATFORM_ Directive must be provided"
#endif

/******************************************************************************
 * Forward declarations for globals defined elsewhere
 ******************************************************************************/

#ifdef ITCH_DEBUG
extern void M(char * strn);
#endif

extern char g_itch_replay_buff[MAX_INPUT_LINE_SIZE];

/******************************************************************************
 * Structs and typdefs
 ******************************************************************************/
#define PF_ERRORONLINE 	0b00000001
#define PF_DELIMREACHED 0b00000010
#define PF_STRINGLIT	0b00000100
#define PF_EOLPROCESSED	0b00001000
#define PF_ESCAPE		0b00010000
#define PF_ESCLB		0b00100000
#define PF_HELPACTIVE	0b01000000

typedef struct PARSER_FLAGS {
	uint8_t parse_result;
	uint8_t match_result;
	uint8_t last_error;
} P_FLAGS;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

	void ParserInit();
	uint8_t ParseProcess(char ch);
	uint8_t ParserMatch(void);
	uint8_t ParserMatchEvaluate(void);
	void ParserSaveTokenAsParameter(void);

	void ResetPossibleList(void);
	void ParserWriteLineErrorString(void);
	void ParserSetError(uint16_t err);
	void ParserResetLine(void);
	void ParserResetPreserve(void);
	void ParserBufferInject(char* inject_str);
	uint8_t ParserActionDispatcher(uint8_t asta_id);

	void P_ESCAPE(char ch);
	uint8_t P_EOL();
	uint8_t P_DOUBLE_QUOTE();
	uint8_t P_SPACE_TAB();
	void P_ADD_TO_PARSE_BUFFER(char ch);

#endif /* PARSER_H_ */
