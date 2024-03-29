/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: Parser.cpp
 - Parsing of char inputs according to the grammar
 - Leveraging NodeMap and its processing results
 - Forming and calling of actions into project code
*******************************************************************/

#include <Parser.h>
#include <itch_strings.h>
#include <common_config.h>
#include <TokenList.h>
#include <NodeMap.h>
#include <stdlib.h>
#include <itch.h>

#if defined(PLATFORM_LINUX)
#elif defined(PLATFORM_ARDUINO)
#include <Arduino.h>
#else
#error "A PLATFORM_ Directive must be provided"
#endif

#ifdef ITCH_DEBUG
#include <itch_hal.h>
extern void M(char strn[]);
extern char g_debug_message[MAX_OUTPUT_LINE_SIZE];
#endif

/******************************************************************************
 * Globals
 ******************************************************************************/
//extern char g_itch_replay_buf[MAX_INPUT_LINE_SIZE];

extern void ITCHWriteChar(char ch);

char g_parser_parse_buf[MAX_INPUT_LINE_SIZE]; 	// The subject of all parsing functions
uint8_t g_parser_buf_idx;		// Points to the next free char in the buffer and is size
P_FLAGS g_pflags;
uint8_t PFLAGS = 0;
TokenList _possible_list;
TokenList _param_list;
TokenList* g_parser_possible_list = &_possible_list;
TokenList* g_parser_param_list = &_param_list;

/******************************************************************************
 * Functions
 ******************************************************************************/

void ParserInit() {
	TLInitialise(g_parser_possible_list);
	TLInitialise(g_parser_param_list);
	ParserResetLine();
}


void ParserResetLine(void) {
	ParserResetPreserve();
	g_parser_parse_buf[0] = '\0';
}

void ParserResetPreserve(void) {
	g_parser_buf_idx = 0;

	PFLAGS = 0;

	g_pflags.parse_result = R_NONE;
	g_pflags.match_result = MR_NO_RESULT;
	g_pflags.last_error = PEME_NO_ERROR;

	MapReset();
	TLReset(g_parser_possible_list);
	TLReset(g_parser_param_list);
}

uint8_t ParserActionDispatcher(uint8_t action_asta_id) {
	// Lookup the function to call (using func map output from the lexer)
	// Call the caller with an assembled param list.
	// Assumes that  param_list is properly populated
	// with the required params to go with the call.

	char action_ident_str[MAX_AST_ACTION_SIZE];
	char label_str[MAX_AST_LABEL_SIZE];
	//ASTA_Node temp_asta;

	uint8_t func_xlat_call_id;
	ParamUnion param_union_array[MAX_PARAM_COUNT];
	uint8_t param_count = 0;
	uint8_t param_index = 0;

	char* current_tl_label_ptr;
	uint8_t param_type;

	// get the static xlat ID from the lexer output header
	//MapGetAction(action_asta_id, action_ident_str);
	MapCopyActionStringByID(action_asta_id, action_ident_str);
	func_xlat_call_id = LookupFuncMap(action_ident_str);

	// set up for assembling the params
	param_count = TLGetSize(g_parser_param_list);
	TLToTop(g_parser_param_list);

	// Assemble the param array (of type ParamUnion)
	// Iterate down g_parser_param_list to get the params
	while (param_index < param_count) {
		//TLCopyCurrentLabel(g_parser_param_list, temp_str);
		current_tl_label_ptr = TLGetCurrentLabelPtr(g_parser_param_list);
		param_type = TLGetCurrentType(g_parser_param_list);
		switch (param_type) {
			case AST_ENUM_ARRAY:
				uint8_t ident_xlat;
				uint8_t ident_asta_id;
				uint8_t member_id;

				// Get the original asta node that relates to this param
				//  for its label (which is the instance name of the StringArray
				//  indexed by the enum)
				ident_asta_id = TLGetCurrentID(g_parser_param_list);
				//MapGetASTAByID(ident_asta_id, &temp_asta);
				MapCopyASTALabelByID(ident_asta_id, label_str);
				//ident_xlat = LookupIdentMap(temp_asta.label);
				ident_xlat = LookupIdentMap(label_str);
				member_id = LookupIdentifierMembers(ident_xlat, current_tl_label_ptr, strlen(current_tl_label_ptr));
				param_union_array[param_index].param_uint16_t = member_id;
				break;
			case AST_LOOKUP:
			case AST_PARAM_DATE:
			case AST_PARAM_TIME:
			case AST_PARAM_STRING:
				param_union_array[param_index].param_char_star = current_tl_label_ptr;
				break;
			case AST_PARAM_INTEGER:
				param_union_array[param_index].param_int16_t = atoi(current_tl_label_ptr);
				break;
			case AST_PARAM_FLOAT:
				param_union_array[param_index].param_float = atof(current_tl_label_ptr);
				break;
			default:
				return PE_BAD_PARAM_ACTION_DISPATCHER;
		}
		param_index++;
		TLNext(g_parser_param_list);
	}

	// call the caller with the right function id and params
	// Return result (of call matching - callee results come back via a callback)
	return CallFunction(func_xlat_call_id, param_union_array);
}

void ParserBufferInject(char* inject_str) {
	strcpy(g_parser_parse_buf, inject_str);
	g_parser_buf_idx = strlen(g_parser_parse_buf);
}

void P_ESCAPE(char ch) {
	if ( (ch == 0x1B) && ((PFLAGS & PF_ESCAPE) == 0) ) {
		PFLAGS |= PF_ESCAPE;
	}
	if ( (ch == '[') && (PFLAGS & PF_ESCAPE) ) {
		PFLAGS &= ~PF_ESCAPE;
		PFLAGS |= PF_ESCLB;
	}
	#ifdef ITCH_DEBUG
		char temp_str[MAX_OUTPUT_LINE_SIZE];
		strcpy_itch_debug(temp_str, ITCH_DEBUG_PF_ESCAPE_IS);
		sprintf(g_debug_message, "%s 0x%x\n", temp_str, PFLAGS);
		M(g_debug_message);
	#endif
}

uint8_t P_EOL() {
	// Processed when EOL reached

	// If help active, turn it off, reset the map and return R_HELP so results will be displayed
	if (PFLAGS & PF_HELPACTIVE) {
		PFLAGS &= ~PF_HELPACTIVE;

		MapReset();
		TLReset(g_parser_possible_list);
		return R_HELP;
	}
	// If no errors in matching call action or ignore blank lines, or catch other errors
	if ((PFLAGS & PF_ERRORONLINE) == 0) {
		switch (g_pflags.match_result) {
			case MR_ACTION_POSSIBLE:
				// Save the last token to the param list - becuase its EOL rather then DELIM
				// 	the ParseMatch will not have been triggered to save it
				ParserSaveTokenAsParameter();

				#ifdef ITCH_DEBUG
					char temp_str[MAX_OUTPUT_LINE_SIZE];
					char action_ident[MAX_AST_ACTION_SIZE];
					MapCopyActionStringByID(MapGetLastMatchedID(), action_ident);
					strcpy_itch_debug(temp_str, ITCH_DEBUG_P_EOL_ACTION_TO_BE_CALLED_IS);
					sprintf(g_debug_message, "\n%s%s\n", temp_str, action_ident);
					M(g_debug_message);
				#endif

				// Dispatch the action and record any error
				g_pflags.last_error = ParserActionDispatcher(MapGetLastMatchedID());

				MapReset();
				TLReset(g_parser_possible_list);
				if (g_pflags.last_error == PEME_NO_ERROR) {
					return R_COMPLETE;
				} else {
					return R_ERROR;
				}
				break;
			case MR_MULTI:
				g_pflags.last_error = PE_MULTI_NOT_PARSED;
				MapReset();
				TLReset(g_parser_possible_list);
				return R_ERROR;
				break;
			default:
				if (g_parser_buf_idx == 0) { // blank line - do nothing
					MapReset();
					TLReset(g_parser_possible_list);
					return R_COMPLETE;
				}
				g_pflags.last_error = PE_LINE_INCOMPLETE;
				MapReset();
				TLReset(g_parser_possible_list);
				return R_ERROR;
				break;
		}
	}
	// else - must be an error on the line
	if (PFLAGS & PF_ERRORONLINE) {
		switch(g_pflags.match_result) {
			case MR_NO_MATCH:
				g_pflags.last_error = PE_EOL_WITH_NO_MATCH;
				MapReset();
				TLReset(g_parser_possible_list);
				return R_ERROR;
				break;
			default:
				g_pflags.last_error = PE_EOL_WITH_UKNOWN_MR;
				MapReset();
				TLReset(g_parser_possible_list);
				return R_ERROR;
				break;
		}
	}
	MapReset();
	TLReset(g_parser_possible_list);
	return R_ERROR;
}

uint8_t P_DOUBLE_QUOTE() {
	if ((PFLAGS & PF_STRINGLIT) == 0) {
		PFLAGS |= PF_STRINGLIT;
	} else {
		PFLAGS &= ~PF_STRINGLIT;;
	}
	// XXX send " to match buffer?
	// map should probably handle string capture.....
	// handle string literals
	//if (pf.string_litteral == 1) { // already in one
		// XXX stuff into a string literal param buffer as well as
		// 	fall through to main buffer
	//}

	return R_CONTINUE;
}

uint8_t P_SPACE_TAB() {
	if (g_parser_buf_idx == 0) { 			// leading space or tab
		// just ignore it
		return R_IGNORE;
	} else {
		if (PFLAGS & PF_STRINGLIT) {
			// let it fall through into the buffer
			return R_CONTINUE;
		} else {
			if (PFLAGS & PF_DELIMREACHED) { // already had one - extra space or tabs between tokens
				return R_IGNORE;
			} else {
				if ((PFLAGS & PF_ERRORONLINE) == 0) { // must be a token delim
					PFLAGS |= PF_DELIMREACHED;
				} else {
					// already error on line - just fall through
					// for error line play back
				}
				return R_CONTINUE;
			}
		}
	}
}

void P_ADD_TO_PARSE_BUFFER(char ch) {
	g_parser_parse_buf[g_parser_buf_idx] = ch;
	//g_parser_parse_buf[g_parser_buf_idx + 1] = '\0';
	g_parser_buf_idx++;
	MapUpdateLineSize(g_parser_buf_idx);
}

uint8_t ParseProcess(char ch) {
	// Process incoming char in context of parse flags
	// set from processing the previous char and match
	// and results from the node map (if called on previous).
	//char out_str[MAX_OUTPUT_LINE_SIZE];

	#ifdef ITCH_DEBUG
		char temp_str[MAX_OUTPUT_LINE_SIZE];
		strcpy_itch_debug(temp_str, ITCH_DEBUG_PARSE_PROCESSING_CHAR);
		sprintf(g_debug_message, "%s%c\n", temp_str, ch);
		M(g_debug_message);
	#endif

	// First deal with editing and special cases
	switch (ch) {
		case 0x7F:				// Backspace ^H
			return R_BACKSPACE;
			break;
		case 0x1B: 				// ESC - Process ANSI escape sequences (eg "ESC[A" - up arrow)
			P_ESCAPE(ch);		// Track escape sequence build
			return R_DISCARD;	// Drop the ch
			break;
		case '[':				// if it follows ESC, track sequence build otherwise let pass
			if(PFLAGS & PF_ESCAPE) {
				P_ESCAPE(ch);
				return R_DISCARD;
			}
			break;
		case 'A':						// Potentially ESC[A (up arrow)?
			if(PFLAGS & PF_ESCLB) {		// Matched, turn off flags and trigger replay
				PFLAGS &= ~PF_ESCAPE;
				PFLAGS &= ~PF_ESCLB;
				return R_REPLAY;
			}
			break;
		case '\r':   								// deal with \r\n combinations and trigger actions
		case '\n':
			if((PFLAGS & PF_EOLPROCESSED) == 0) { 		// have not just processed EOL
				PFLAGS |= PF_EOLPROCESSED;			// flag that we are now
				ITCHWriteChar('\n');
				return	P_EOL();					// !!! Main parser response trigger
			} else {
				return R_DISCARD;					// redundant extra
			}
			break;
		case '\"':
			// Toggle string_litteral flag, continue
			P_DOUBLE_QUOTE();
			return R_CONTINUE;
			break;
		case ' ':
		case '\t':	// handle space and tab delimiters (and extras of them)
			if (P_SPACE_TAB() == R_IGNORE) {
				return R_IGNORE;				// must have been superfluous
			}
			break; 								// single - drop into buffer
		case '?':	// turn on help flag, drop into parse in help active mode
			ITCHWriteChar('\n');
			ITCHWriteLineMisc(ITCH_MISC_HELP_HEADING);
			PFLAGS |= PF_HELPACTIVE;
			break;
		case 0x03:	// ^C
		case 0x04:	// ^D
		case 0x18:	// ^X
		case 0x1A: 	// ^Z
			return R_EXIT;
			break;
		default:						// catch unhandled escape sequences
			if(PFLAGS & PF_ESCLB) {
				PFLAGS &= ~PF_ESCLB;
				PFLAGS &= ~PF_ESCAPE;
				// TODO left and right arrow editing
				return R_DISCARD;
			}
			break;
	}
	// whatever it is now, throw it in the parse buffer and parse what we have, partial or complete
	P_ADD_TO_PARSE_BUFFER(ch);

	if ((PFLAGS & PF_ERRORONLINE) == 1) {
		// Error already flagged but need to discard buffered chars still coming before finishing
		// so just ignore until EOL. Next iteration will return Error or Complete accordingly
		// once EOL encountered
		#ifdef ITCH_DEBUG
			strcpy_itch_debug(temp_str, ITCH_DEBUG_PARSE_DISCARD_CHAR);
			sprintf(g_debug_message, "%s%c\n", temp_str, ch);
			M(g_debug_message);
		#endif
		return R_IGNORE;
	}

	// if we get to here its just another ordinary char - continue to matching
	g_pflags.match_result = ParserMatch();

	// evaluate the match results for what it means for parsing
	g_pflags.parse_result = ParserMatchEvaluate();

	return g_pflags.parse_result;
}

uint8_t ParserMatch(void) {
	// Setup and call the nodemap token matching process

	// If last match result was unique and input char parsing detected a delimiter,
	// save the token for later action calling and advance the buffer index so that
	// the matcher is looking at the next token. (which will trigger a match dummy iteration
	// with the result being to go back to the calling routine unfinished to get the next ch)

	if ((PFLAGS & PF_DELIMREACHED) && ((g_pflags.match_result == MR_UNIQUE) || (g_pflags.match_result == MR_ACTION_POSSIBLE))) {
		ParserSaveTokenAsParameter();
		// advance the nodemap past the delimiter
		MapAdvance(g_parser_buf_idx);
		PFLAGS &= ~PF_DELIMREACHED;
	}

	// clear out previous matching possibilities
	TLReset(g_parser_possible_list);

	#ifdef ITCH_DEBUG
		TLDumpList(g_parser_possible_list);
	#endif //ITCH_DEBUG

		// run the next match process and return the result
	return MapMatch(g_parser_parse_buf, g_parser_possible_list);
}

uint8_t ParserMatchEvaluate(void) {

	switch (g_pflags.match_result) {
		case MR_NO_MATCH:	// no nodes match the input - error
			#ifdef ITCH_DEBUG
				strcpy_itch_debug(g_debug_message, ITCH_DEBUG_PARSERMATCHEVALUATE_MR_NO_MATCH);
				M(g_debug_message);
			#endif
			PFLAGS |= PF_ERRORONLINE;
			g_pflags.parse_result = R_IGNORE; // still need to clean out incoming buffer
			break;
		case MR_UNIQUE: {
			#ifdef ITCH_DEBUG
				// single unique match - will still need EOL to finsih
				strcpy_itch_debug(g_debug_message, ITCH_DEBUG_PARSERMATCHEVALUATE_MR_UNIQUE);
				M(g_debug_message);
				//pf.unique_match = 1;
				Serial.println("DEBUG: ParserMatchEvaluate, g_parser_possible_list");
				TLDumpList(g_parser_possible_list);
				char temp[MAX_OUTPUT_LINE_SIZE];
				char temp_str[MAX_OUTPUT_LINE_SIZE];
				TLCopyCurrentLabel(g_parser_possible_list, temp);
				strcpy_itch_debug(temp_str, ITCH_DEBUG_PARSERMATCHEVALUATE_UNIQUE_MATCH_IS);
				sprintf(g_debug_message, "%s%s\n", temp_str, temp);
				M(g_debug_message);
			#endif

			// All good - unique match to node on this token,
			// return to continue getting more chars
			g_pflags.parse_result = R_UNFINISHED;
		}
			break;
		case MR_ACTION_POSSIBLE:
			#ifdef ITCH_DEBUG
				// single unique matched with is actionalable - (if EOL is next)
				strcpy_itch_debug(g_debug_message, ITCH_DEBUG_PARSERMATCHEVALUATE_MR_ACTION_POSSIBLE);
				M(g_debug_message);
			#endif
			// All good - unique match to node on this token,
			// and it is actionable at this point (if EOL is next)
			// return to continue getting more chars
			g_pflags.parse_result = R_UNFINISHED;
			break;
		case MR_MULTI: {
			#ifdef ITCH_DEBUG
				char out_str[MAX_OUTPUT_LINE_SIZE];
				char temp_str[MAX_OUTPUT_LINE_SIZE];
				strcpy_itch_debug(g_debug_message, ITCH_DEBUG_PARSERMATCHEVALUATE_MR_MULTI);
				M(g_debug_message);
				TLToTop(g_parser_possible_list);
				for (uint16_t i = 0; i < TLGetSize(g_parser_possible_list); i++) {
					TLCopyCurrentLabel(g_parser_possible_list, temp_str);
					sprintf(out_str, "\t%s\n", temp_str);
					sprintf(g_debug_message, "DEBUG: %s", out_str);
					M(g_debug_message);
					TLNext(g_parser_possible_list);
				}
			#endif

			// no errors so far, but the entered token could match
			// multiple nodes... continue to get more chars
			g_pflags.parse_result = R_UNFINISHED;
		}
			break;
		case MR_DELIM_SKIP:
			#ifdef ITCH_DEBUG
				strcpy_itch_debug(g_debug_message, ITCH_DEBUG_PARSERMATCHEVALUATE_MR_DELIM_SKIP);
				M(g_debug_message);
			#endif
			// good-oh, carry on
			g_pflags.parse_result = R_UNFINISHED;
			break;
		case MR_HELP_ACTIVE: {
			// Moved to immediate write rather than populating the TL which
			// 	is now constrained to grammar depth - help is width and likely
			//  to overrun on the root node where there is the most branching
			#ifdef ITCH_DEBUG
				strcpy_itch_debug(g_debug_message, ITCH_DEBUG_PARSERMATCHEVALUATE_MR_HELP_ACTIVE);
				M(g_debug_message);
			#endif
			g_pflags.parse_result = R_HELP;
		}
			break;
		case MR_LOOKUP_TBD:
			g_pflags.parse_result = R_UNFINISHED;
			break;
		case MR_IDENT_MEMBER_TBD:
			g_pflags.parse_result = R_UNFINISHED;
			break;
		case MR_ERROR:
			#ifdef ITCH_DEBUG
				strcpy_itch_debug(g_debug_message, ITCH_DEBUG_PARSERMATCHEVALUATE_MR_ERROR);
				M(g_debug_message);
			#endif
			g_pflags.last_error = MapGetErrorCode();
			MapReset();
			g_pflags.parse_result = R_ERROR;
			break;
		default:
			#ifdef ITCH_DEBUG
				strcpy_itch_debug(g_debug_message, ITCH_DEBUG_PARSERMATCHEVALUATE_NO_MR_CAUGHT_DEFUALT);
				M(g_debug_message);
			#endif
			g_pflags.last_error = PE_NODEMAP_RESULT_NOT_CAUGHT;
			MapReset();
			g_pflags.parse_result = R_ERROR;
			break;
	}
	return g_pflags.parse_result;
}

void ParserSaveTokenAsParameter(void) {
	// Save the last matched node/token details into the param list
	//
	// Called by ParseMatch when a delimiter is reached and there is
	//	a unique match or an action match.
	//
	// Also called by P_EOL to capture the last token on the line before
	// 	action processing, as this token will not have been isolated in
	// 	the match loop - because the EOL has been reached rather than a
	// 	delimiter.

	char* temp_str;
	uint8_t temp_str_size;
	//TokenNode* new_param;
	//char temp_param[MAX_IDENTIFIER_LABEL_SIZE];
	ASTA_Node temp_asta;
	uint16_t temp_asta_id;

	// get the id of the last matched node
	temp_asta_id = MapGetLastMatchedID();

	// get the matched node from the asta
	MapGetASTAByID(temp_asta_id, &temp_asta);

	// Add to the param list for action calling (but exclude keywords)
	if (temp_asta.type > AST_KEYWORD) {

		// get the last matched param as a string
		// used to be a strcpy function
		temp_str = MapGetLastTargetString();
		temp_str_size = MapGetLastTargetStringSize();

		// Null terminate the string portion (input buffer will have moved on)
		//temp_str[temp_str_size] = '\0';

		// get a new param node and write its details
		//new_param = TLNewTokenNode();
		//new_param->label = (char *)malloc((strlen(temp_str) + 1) * sizeof(char));
		//strcpy(new_param->label, temp_str);

		// just point it
		//new_param->label = temp_str;
		strncpy(temp_asta.label, temp_str, temp_str_size);
		temp_asta.label[temp_str_size] = '\0';
		//new_param->id = temp_asta_id;
		//new_param->type = temp_asta.type;
		TLAddASTAToTokenList(g_parser_param_list, temp_asta);
		//TLAdd(g_parser_param_list, new_param);
	}
}

void ParserWriteLineErrorString(void) {
#ifdef PLATFORM_ARDUINO
	ITCHWriteLine_P(itch_error_strings[g_pflags.last_error].text);
#else
	ITCHWriteLine(itch_error_strings[g_pflags.last_error].text);
#endif
}

void ParserSetError(uint16_t err) {
	g_pflags.last_error = err;
}

