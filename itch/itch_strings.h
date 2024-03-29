/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: itch_strings.h
 - enum indexes and string literals used by itch
 - Platform dependent - on Arduino string literals are in PROGMEM
 *******************************************************************/

#ifndef PARSER_ERRORS_H_
#define PARSER_ERRORS_H_


#include <itch_config.h>
#include <stdint.h>

#if defined(PLATFORM_LINUX)
#elif defined(PLATFORM_ARDUINO)
#include <Arduino.h>
#else
#error "A PLATFORM_ Directive must be provided"
#endif

// Manual calculation based on the length of strings in this file
// TODO: could be optimised but since its in PROGMEM on arduino its been low priority
// TODO: trim strings and sizes to use less rom on devices smaller than mega2560
#define MAX_ITCH_STRING_LENGTH 114

typedef struct ENUM_STRING_ARRAY_TYPE {
	// Array of strings relating to enums
	const char text[MAX_ITCH_STRING_LENGTH];
} EnumStringArray;

// PE - Parse Error, ME - Map/Match Error
enum {
	PE_ERROR = 0,
	PEME_NO_ERROR,
	PE_LINE_INCOMPLETE,
	PE_MULTI_NOT_PARSED,
	PE_EOL_WITH_UKNOWN_MR,
	PE_EOL_WITH_NO_MATCH,
	PE_NODEMAP_RESULT_NOT_CAUGHT,
	ME_MATCHREDUCE_EXPECTED_1,
	PE_LOOKUP_PASSED_TO_REDUCER,
	PE_MULTIPLE_PARAM_LOOKAHEAD,
	PE_FUNC_XLAT_NOT_MATCHED_IN_CALLFUNCTION,
	PE_BAD_PARAM_ACTION_DISPATCHER,
	ME_GETASTABYID_FAIL,
	PE_NO_PARSE_RESULT,
	TL_EXCEED_GRAMMAR_DEPTH,
	ME_GETASTACHILDID_FAIL,
	ME_COPYASTAACTIONSTRING_FAIL,
	ME_COPYASTALABELSTRING_FAIL,
	LAST_PARSE_ERROR
};

#ifdef PLATFORM_ARDUINO
const EnumStringArray itch_error_strings[LAST_PARSE_ERROR] PROGMEM = {
#else
const EnumStringArray itch_error_strings[LAST_PARSE_ERROR] = {
#endif
		"PE_ERROR - This shouldn't happen",
		"Unknown error",
		"Last token invalid or command incomplete",
		"Multiple non-unique matches were found for some or all terms and could not be parsed",
		"EOL reached and the MR result from NodeMap is unknown",
		"Term or terms could not be matched",
		"NodeMap result was not caught by the parser",
		"Fatal error in MatchReduce: size != 1 after iterative delete to unique node",
		"Fatal error in MatchReduce: unique node of type AST_LOOKUP passed to reducer",
		"In MatchReduce - Next parse step dependent on multiple user-param nodes which can't be distinguished",
		"func_xlat not matched in CallFunction - no matching function?",
		"ActionDispatcher: Unknown or incorrect parameter type found in param_list while assembling ParamUnion- keyword?",
		"GetASTAByID: ASTA ID Not Found or array index going out of bounds",
		"Parser did not return a result at all. Error in Parser?",
		"Fatal Error: TokenList attempted to exceed MAX_GRAMMAR_DEPTH",
		"MapGetASTAChildIDByID: ASTA ID Not Found or array index going out of bounds",
		"MapCopyActionStringByID ASTA ID Not Found or array index going out of bounds",
		"MapCopyASTALabelByID ASTA ID Not Found or array index going out of bounds",
};


enum {
	ITCH_MISC_ERROR = 0,
	ITCH_MISC_HELP_HEADING,
	ITCH_MISC_ERROR_PROMPT,
	ITCH_MISC_ERROR_HEADER,
	ITCH_MISC_PROMPT,
	ITCH_MISC_CRNL,
	ITCH_MISC_PARAM_DATE,
	ITCH_MISC_PARAM_TIME,
	ITCH_MISC_PARAM_FLOAT,
	ITCH_MISC_PARAM_INTEGER,
	ITCH_MISC_PARAM_STRING,
	ITCH_TERMINAL_WELCOME,
	ITCH_TERMINAL_GOODBYE,
	ITCH_BUFFER_STUFFING_MODE,
	ITCH_BUFFER_STUFFING_ERROR,
	ITCH_CCC_OK,
	ITCH_CCC_ERROR,
	LAST_ITCH_MISC_STRING
};

#define ITCH_PROMPT_SIZE 7

#ifdef PLATFORM_ARDUINO
const EnumStringArray misc_itch_strings[LAST_ITCH_MISC_STRING] PROGMEM = {
#else
const EnumStringArray misc_itch_strings[LAST_ITCH_MISC_STRING] = {
#endif
	"MISC_ERROR",
	"Command Help:",
	"Try \"?\" or <command> ? for help.",
	">>> Error: ",
	"itch:$ ",
	"\n",
	"<param-date>",
	"<param-time>",
	"<param-float>",
	"<param-integer>",
	"<param-string>",
	"ITCH - Interactive Terminal Command sHell",
	"ITCH Interactive Terminal Session Ended",
	"ITCH Entering Buffer Stuffing Mode. Will return to TEXT_DATA mode on completion.",
	"ITCH Line being stuffed when error occurred:",
	"OK",
	"ERROR",
};

void ITCHWriteLineError(uint8_t error_enum);
void ITCHWriteLineMisc(uint8_t misc_string_enum);
void ITCHWriteMisc(uint8_t misc_string_enum);

#ifdef ITCH_DEBUG

enum {
	ITCH_DEBUG_ERROR = 0,
	ITCH_DEBUG_R_COMPLETE,
	ITCH_DEBUG_POLL_CASE_R_REPLAY,
	ITCH_DEBUG_MAPDETERMINETARGET_DELIM_SKIP,
	ITCH_DEBUG_MAPSELECTMATCHINGNODES_ID_LABEL_ACTION,
	ITCH_DEBUG_MATCH_REDUCE_FATAL_SIZE_NOT_1,
	ITCH_DEBUG_MATCH_REDUCE_UNIQUE_NODE_PASSED_TO_REDUCER,
	ITCH_DEBUG_MATCH_REDUCE_NEXT_DEP_MULTIPLE_USER_PARAM_NODES,
	ITCH_DEBUG_MAP_ADVANCE,
	ITCH_DEBUG_PF_ESCAPE_IS,
	ITCH_DEBUG_P_EOL_ACTION_TO_BE_CALLED_IS,
	ITCH_DEBUG_PARSE_PROCESSING_CHAR,
	ITCH_DEBUG_PARSE_R_REPLAY,
	ITCH_DEBUG_PARSE_DISCARD_CHAR,
	ITCH_DEBUG_PARSERMATCHEVALUATE_MR_NO_MATCH,
	ITCH_DEBUG_PARSERMATCHEVALUATE_MR_UNIQUE,
	ITCH_DEBUG_PARSERMATCHEVALUATE_UNIQUE_MATCH_IS,
	ITCH_DEBUG_PARSERMATCHEVALUATE_MR_ACTION_POSSIBLE,
	ITCH_DEBUG_PARSERMATCHEVALUATE_MR_MULTI,
	ITCH_DEBUG_PARSERMATCHEVALUATE_MR_DELIM_SKIP,
	ITCH_DEBUG_PARSERMATCHEVALUATE_MR_HELP_ACTIVE,
	ITCH_DEBUG_PARSERMATCHEVALUATE_MR_ERROR,
	ITCH_DEBUG_PARSERMATCHEVALUATE_NO_MR_CAUGHT_DEFUALT,
	LAST_ITCH_DEBUG_STRING
};

#ifdef PLATFORM_ARDUINO
const EnumStringArray itch_debug_strings[LAST_ITCH_DEBUG_STRING] PROGMEM = {
#else
const EnumStringArray itch_debug_strings[LAST_ITCH_DEBUG_STRING] = {
#endif
	"ITCH_DEBUG_ERROR",
	"DEBUG ITCH: R_COMPLETE\n",
	"DEBUG (Poll) case R_REPLAY. replay_buf: ",
	"DEBUG (MapDetermineTarget): DELIM_SKIP\n",
	"DEBUG (MapSelectMatchingNodes) Adding Possible Match: ID/Label/Action: ",
	"DEBUG Fatal in (MatchReduce): size != 1 after iterative delete to unique node\n",
	"DEBUG Fatal Error in (MatchReduce): unique lookup node passed to reducer\n",
	"DEBUG (MatchReduce) Error Next parse step dependent on multiple, indistinguishable param nodes\n",
	"DEBUG (MapAdvance)\n",
	"DEBUG pf.escape is: ",
	"DEBUG (P_EOL) **** Action matched and to be called is: ",
	"DEBUG (Parse) *************** Processing Character: ",
	"DEBUG (Parse) Returning R_REPLAY\n",
	"DEBUG (Parse) Discarding: ",
	"DEBUG (ParserMatchEvaluate): MR_NO_MATCH\n",
	"DEBUG (ParserMatchEvaluate): MR_UNIQUE\n",
	"DEBUG (ParserMatchEvaluate): Uniquely matched: ",
	"DEBUG (ParserMatchEvaluate): MR_ACTION_POSSIBLE\n",
	"DEBUG (ParserMatchEvaluate): MR_MULTI\n",
	"DEBUG (ParserMatchEvaluate): MR_DELIM_SKIP\n",
	"DEBUG (ParserMatchEvaluate): MR_HELP_ACTIVE\n",
	"DEBUG (ParserMatchEvaluate): MR_ERROR\n",
	"DEBUG (ParserMatchEvaluate): NO MR Caught - default case\n",
};

#endif


#endif /* PARSER_ERRORS_H_ */
