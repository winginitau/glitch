/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: NodeMap.h
 - Navigation and processing through the syntax tree
 - Called by the parser
*******************************************************************/

#ifndef NODEMAP_H_
#define NODEMAP_H_

#include <TokenList.h>
#include <stdint.h>
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

/******************************************************************************
 * Structs and typdefs
 ******************************************************************************/

// Possible Match Results
enum {
	MR_ERROR = 0,
	MR_NO_RESULT,
	MR_NO_MATCH,
	MR_MULTI,
	//MR_UNIQUE_KEYWORD,
	//MR_UNIQUE_IDENT,
	//MR_PARAM,
	//MR_LOOK_RIGHT,
	//MR_INCOMPLETE,
	MR_ACTION_POSSIBLE,
	MR_DELIM_SKIP,
	MR_CONTINUE,
	MR_UNIQUE,
	MR_HELP_ACTIVE,
	MR_LOOKUP_TBD,
	MR_IDENT_MEMBER_TBD,
};

typedef struct MATCH_FLAGS {
	uint8_t help_active;
	uint8_t keyword_match;
	uint8_t lookup_tbd;
	uint8_t ident_member_tbd;

	uint8_t last_matched_id;
	uint8_t match_result;
	uint8_t error_code;
} M_FLAGS;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

void MapReset(void);
uint8_t MapGetASTAByID(uint8_t ASTA_ID, ASTA_Node* result);
uint8_t MapGetASTAChildIDByID(uint8_t asta_id);
uint8_t MapGetLastMatchedID();
uint8_t MapMatch(char* target, TokenList* match_list);
char* MapGetLastTargetString(void);
uint8_t MapGetLastTargetStringSize(void);
void MapUpdateLineSize(uint8_t size);
uint8_t MapAdvance(uint8_t in_buf_idx);
char* MapCopyActionStringByID(uint8_t asta_id, char* action_str);
char* MapCopyASTALabelByID(uint8_t asta_id, char* label_str);
//uint8_t MapGetAction(uint8_t asta_id, char* action_str);
uint8_t MapMatchReduce(TokenList* list);
uint8_t MapGetErrorCode();

uint8_t MapDetermineTarget(uint8_t* target_size, char* target, char* line);
void MapSelectMatchingNodes(char* target, uint8_t token_size, TokenList* matched_list);
uint8_t MapEvaluateMatchedList(TokenList* matched_list);
uint8_t Compare_N_PARAM_DATE(char* target, uint8_t target_size, ASTA_Node* temp_node);
uint8_t Compare_N_PARAM_TIME(char* target, uint8_t target_size, ASTA_Node* temp_node);
uint8_t Compare_N_PARAM_FLOAT(char* target, uint8_t target_size, ASTA_Node* temp_node);
uint8_t Compare_N_PARAM_INTEGER(char* target, uint8_t target_size, ASTA_Node* temp_node);
uint8_t Compare_N_PARAM_STRING(char* target, uint8_t target_size, ASTA_Node* temp_node);
uint8_t Compare_N_LOOKUP(char* target, uint8_t target_size, ASTA_Node* temp_node);
uint8_t Compare_N_IDENTIFIER(char* target, uint8_t target_size, ASTA_Node* temp_node);
uint8_t Compare_N_KEYWORD(char* target, uint8_t target_size, ASTA_Node* temp_node);

#endif /* NODEMAP_H_ */
