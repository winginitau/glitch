/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: TokenList.h
 - List handling of NodeMap and Parser tokens and ASTA nodes
*******************************************************************/

#ifndef TOKENLIST_H_
#define TOKENLIST_H_

#include <itch_config.h>
#include <out.h>
#include <stdint.h>

#if defined(PLATFORM_LINUX)
#elif defined(PLATFORM_ARDUINO)
#include <Arduino.h>
#else
#error "A PLATFORM_ Directive must be provided"
#endif

typedef struct TOKEN_NODE {
	char* label;
	uint8_t type;
	uint8_t id;
} TokenNode;

typedef struct TOKEN_LIST {
	TokenNode list[MAX_GRAMMAR_DEPTH];
	uint8_t current;
	uint8_t size;
} TokenList;

void TLInitialise(TokenList* tl);
TokenNode* TLAddASTAToTokenList(TokenList* tl, ASTA_Node node); //MapSelectMatchingNodes()
void TLReset(TokenList* tl);									// quite a few

char* TLCopyCurrentLabel(TokenList* tl, char* result); 	//ParserMatchEvaluate
char* TLGetCurrentLabelPtr(TokenList* tl);				//ParserActionDispatcher
uint8_t TLGetCurrentType(TokenList* tl);				//ParserActionDispatcher
uint8_t TLGetCurrentID(TokenList* tl); 	//MapEvaluateMatchedList x2, ParserActionDispatcher

void TLToTop(TokenList* tl);			//ParserActionDispatcher, ParserMatchEvaluate
TokenNode* TLNext(TokenList* tl);		//ParserActionDispatcher, ParserMatchEvaluate
uint8_t TLGetSize(TokenList* tl);		// Lots
uint8_t TLIsEmpty(TokenList* tl);		//MapEvaluateMatchedList

void TLDeleteByType(TokenList* tl, int8_t dtype);		//MapMatchReduce
uint8_t TLCountByType(TokenList* tl, uint8_t type);	//MapMatchReduce

#ifdef ITCH_DEBUG
void TLDumpList(TokenList* tl);
#endif //ITCH_DEBUG

#endif /* TOKENLIST_H_ */
