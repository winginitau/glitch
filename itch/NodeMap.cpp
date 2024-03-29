/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: NodeMap.cpp
 - Navigation and processing through the syntax tree
 - Called by the parser
*******************************************************************/

#include <NodeMap.h>
#include <itch_config.h>
#include <common_config.h>
#include <stdint.h>
#include <stddef.h>
#include <itch_strings.h>
#include <itch_hal.h>
#include <ctype.h>
#include <itch.h>

#if defined(PLATFORM_LINUX)
#elif defined(PLATFORM_ARDUINO)
#include <Arduino.h>
#else
#error "A PLATFORM_ Directive must be provided"
#endif

/********************************************************************
 * Globals
 ********************************************************************/

#ifdef ITCH_DEBUG
extern void M(char strn[]);
extern char g_debug_message[MAX_OUTPUT_LINE_SIZE];
#endif

M_FLAGS g_mflags;
uint8_t g_map_line_pos;
uint8_t g_map_line_size;
uint8_t g_map_id_current;		// currently matched node
uint8_t g_map_id_walker;		// to walk through the asta array nodes by id

char* g_map_last_target_str = NULL;
uint8_t g_map_last_target_size = 0;

/********************************************************************
 * Functions
 ********************************************************************/

char* strlwr(char* s) {
	// string go lower case
    char* tmp = s;
    for (;*tmp;++tmp) {
        *tmp = tolower((unsigned char) *tmp);
    }
    return s;
}

char* strupr(char* s) {
	// string to upper case
    char* tmp = s;
    for (;*tmp;++tmp) {
        *tmp = toupper((unsigned char) *tmp);
    }
    return s;
}

void MapReset(void) {
	g_mflags.last_matched_id = 0;
	g_mflags.help_active = 0;
	g_mflags.keyword_match = 0;
	g_mflags.error_code = PEME_NO_ERROR;
	g_mflags.lookup_tbd = 0;
	g_mflags.ident_member_tbd = 0;

	g_map_line_pos = 0;
	g_map_line_size = 0;
	g_map_id_current = 0;
}

uint8_t MapGetASTAChildIDByID(uint8_t asta_id) {
	// Return the ASTA ID of the first child of the specified ASTA ID
	// First child is set to 0 in the ASTA if the node has no children

	uint8_t asta_idx = 0;	// index into the asta array

	while (asta_idx < AST_NODE_COUNT) {
		#ifdef PLATFORM_ARDUINO
		/*
			ASTA_Node temp_asta;
			memcpy_P(&temp_asta, &asta[asta_idx], sizeof(ASTA_Node));
			Serial.print("temp_asta.id)): ");
			Serial.println(temp_asta.id);
			if (temp_asta.id == asta_id) {
				Serial.print("returning temp_asta.first_child: ");
				Serial.println(temp_asta.first_child);
				return temp_asta.first_child;
			}
		*/

			if ( ((uint8_t)pgm_read_byte(&(asta[asta_idx].id))) == asta_id) {
				return (uint8_t)pgm_read_byte(&(asta[asta_idx].first_child));
			}
		#endif //PLATFORM_ARDUINO
	#ifdef PLATFORM_LINUX
			if((asta[asta_idx].id) == asta_id) {
				return asta[asta_idx].first_child;
			}
	#endif //PLATFORM_LINUX
		asta_idx++;
	}
	ITCHWriteLineError(ME_GETASTACHILDID_FAIL);
	return PE_ERROR;
}


uint8_t MapGetASTAByID(uint8_t asta_id, ASTA_Node* result) {
	// Populate the provided result ASAT_Node with the contents
	//  of the ASTA asta_id
	uint8_t asta_idx = 0;	// index into the asta array
	uint8_t test;

	// Iterate the ASTA to find the right entry then copy it
	// (can't guarantee the array will be in id order)
	while (asta_idx < AST_NODE_COUNT) {
		memcpy_itch_hal(&test, &(asta[asta_idx].id), sizeof(test));
		if (test == asta_id) {
			#ifdef ARDUINO
				memcpy_P(result, &(asta[asta_idx]), sizeof(ASTA_Node));
			#else
				memcpy_itch_hal(result, &(asta[asta_idx]), sizeof(ASTA_Node));
			#endif

			return PEME_NO_ERROR;
		}
		asta_idx++;
	}
	return ME_GETASTABYID_FAIL;
}

uint8_t MapDetermineTarget(uint8_t* target_size, char** target, char* line) {
	// g_map_line_pos points to current token in target
	*target_size = g_map_line_size - g_map_line_pos;
	if (*target_size == 0) {
		// We've been advanced and line_pos is now pointing
		// past the space delimiter at the end of the line.
		// Don't process
		#ifdef ITCH_DEBUG
			strcpy_itch_debug(g_debug_message, ITCH_DEBUG_MAPDETERMINETARGET_DELIM_SKIP);
			//sprintf(g_debug_message, "DEBUG (MapDetermineTarget): DELIM_SKIP\n\r");
			M(g_debug_message);
		#endif
		return MR_DELIM_SKIP;
	} else {
		// line is \0 terminated.
		*target = line + g_map_line_pos;
		return MR_CONTINUE;
	}
}

uint8_t Compare_N_PARAM_DATE(char* target, uint8_t target_size, ASTA_Node* temp_node) {
	// date format is 2018-02-20
	// sufficient for filtering to just check if it starts with a 2
	(void)target_size;
	if (isdigit(target[0])) {
		uint16_t x = target[0] - '0';
		if (x == 2) {
			// could be
			strcpy_itch_misc(temp_node->label, ITCH_MISC_PARAM_DATE);
			return 1;
		}
	}
	return 0;
}

uint8_t Compare_N_PARAM_TIME(char* target, uint8_t target_size, ASTA_Node* temp_node) {
	// time format is 23:30:45 (hh:mm:ss)
	(void)target_size;
	if (isdigit(target[0])) {
		int x = target[0] - '0';
		if (x < 3) {
			// could be
			strcpy_itch_misc(temp_node->label, ITCH_MISC_PARAM_TIME);
			return 1;
		}
	}
	return 0;
}

uint8_t Compare_N_PARAM_FLOAT(char* target, uint8_t target_size, ASTA_Node* temp_node) {
	// sufficient for filtering to just check if it starts with a digit
	(void)target_size;
	if (isdigit(target[0])) {
		strcpy_itch_misc(temp_node->label, ITCH_MISC_PARAM_FLOAT);
		return 1;
	}
	return 0;
}

uint8_t Compare_N_PARAM_INTEGER(char* target, uint8_t target_size, ASTA_Node* temp_node) {
	// sufficient for filtering to just check if it starts with a digit
	(void)target_size;
	if (isdigit(target[0])) {
		strcpy_itch_misc(temp_node->label, ITCH_MISC_PARAM_INTEGER);
		return 1;
	}
	return 0;
}

uint8_t Compare_N_PARAM_STRING(char* target, uint8_t target_size, ASTA_Node* temp_node) {
	(void)target;
	(void)target_size;
	/*
	if (isdigit(target[0])) {
		// can't be keyword or ident or lookup
		// XXX any string, unless in "" would also likely be subject to
		// identifier regex..... eg filename.....
		return 0;
	}
	*/
	// All input is a string by definition
	// we're looking for a string, so add it
	strcpy_itch_misc(temp_node->label, ITCH_MISC_PARAM_STRING);
	return 1;
}

uint8_t Compare_N_LOOKUP(char* target, uint8_t target_size, ASTA_Node* temp_node) {
	if (isdigit(target[0])) {
		// can't be keyword or ident or lookup
		return 0;
	}
	// Although a lookup is a string it is actually
	// a special case of a reserved word (ie identifier)
	// So lookup needs to be called from here

	// Can't just treat as a string or matchreduce will error
	// because it can't distinguish a string from a lookup
	// in the case where an AST node has valid branching to
	// both a param-string node and a lookup node.

	// A possible match to a lookup can't be determined until
	// the token is complete. Excluding it altogether before
	// then would result in a MR_NO_MATCH and error. Therefore
	// special treatment is required - if a lookup is under
	// evaluation (ie is a possibility in the sibling list) then
	// set the mflags.lookup_tbd flag to indicate.

	uint8_t xlat;
	xlat = LookupLookupMap(temp_node->label);
	int lookup_result;
	lookup_result = LookupLookupMembers(xlat, target, target_size);
	if (lookup_result == 1) {
		g_mflags.lookup_tbd = 0;
		return 1;
	} else {
		g_mflags.lookup_tbd = 1;
		return 0;
	}
}

uint8_t Compare_N_IDENTIFIER(char* target, uint8_t target_size, ASTA_Node* temp_node) {
	uint8_t xlat;
	uint8_t member_id;

	if (isdigit(target[0])) {
		// can't be keyword or ident or lookup
		return 0;
	}

	xlat = LookupIdentMap(temp_node->label);
	member_id = LookupIdentifierMembers(xlat, target, target_size);

#ifdef ITCH_DEBUG
	//sprintf(g_debug_message, "DEBUG (Compare_N_IDENTIFIER): Target: %s  ASTALabel: %s  xlatVal: %d  block_cat_id: %d\n\n\r", target, temp_node->label, xlat, member_id);
	//M(g_debug_message);
#endif

	//See comments in Compare_N_LOOKUP
	if (member_id > 0) {
		g_mflags.ident_member_tbd = 0;
		return member_id;
	} else {
		g_mflags.ident_member_tbd = 1;
		return 0;
	}
}

uint8_t Compare_N_KEYWORD(char* target, uint8_t target_size, ASTA_Node* temp_node) {
	char target_upr[MAX_TOKEN_SIZE];
	char label_upr[MAX_TOKEN_SIZE];

	// even as partial unique, it overrides all other types
	// check if its matching the input string
	if (isdigit(target[0])) {
		// can't be keyword or ident or lookup
		return 0;
	}
	strncpy(target_upr, target, target_size);
	target_upr[target_size] = '\0';
	strcpy(label_upr, temp_node->label);
	strupr(target_upr);
	strupr(label_upr);

	//char* sub_string_start;
	//sub_string_start = strstr(label_upr, target_upr);
	//if (sub_string_start == label_upr) {
	if(strncmp(target_upr, label_upr, target_size) == 0) {
		g_mflags.keyword_match = 1;
		//Serial.println("Compare_N_KEYWORD Match");
		return 1;
	}
	return 0;
}

uint8_t MapMatch(char* line, TokenList* matched_list) {
	// working from the currently matched node, isolate the last token
	// in the line buffer (complete or partial), match it against
	// possible nodes, evaluate the results and return result
	// and a list of matched nodes to the parser

	char* target = NULL;		// for the last bit that we are interested in
	uint8_t target_size;		// buffer is not terminated - need size

	// clear the previous match list
	TLReset(matched_list);

	// if a new line advance to the first node in the ASTA
	if (g_map_id_current == 0) { g_map_id_current = 1; }

	// set the ASTA walker to the current node
	g_map_id_walker = g_map_id_current;

	// Determine the target part of the line buffer
	// ie the last token on the line - complete or partial and get its size
	g_mflags.match_result = MapDetermineTarget(&target_size, &target, line);

	// Check for the MR_DELIM_SKIP flag, if set the last was a space so there
	// 	wont be anything else yet, return.
	if (g_mflags.match_result == MR_DELIM_SKIP) {
		return MR_DELIM_SKIP;
	}

	// save the target to be turned into an action call parameter later (once delim reached)
	g_map_last_target_str = target;
	g_map_last_target_size = target_size;

	// by default help is off
	g_mflags.help_active = 0;
	// except if '?' is the last char (ie target/token of interest)
	if (target[(target_size-1)] == '?') {
		// add sibling nodes to list based on match so far
		// - if advanced past delimiter using space " " then it will be
		//	 siblings of the matched child of the previously matched node
		g_mflags.help_active = 1;
	}
	// reset keyword match if set previously
	g_mflags.keyword_match = 0;

	// reset lookup_tbd and ident_member_tbd if set previously
	g_mflags.lookup_tbd = 0;
	g_mflags.ident_member_tbd = 0;

	// Scan the current node and its siblings for potential or exact matches
	// Returning the result in matched_list
	MapSelectMatchingNodes(target, target_size, matched_list);

	// Evaluate the matched_list and return the result to the parser
	return MapEvaluateMatchedList(matched_list);
}

void MapSelectMatchingNodes(char* token, uint8_t token_size, TokenList* matched_list) {
	ASTA_Node temp_asta;
	uint8_t cr = 0;		// compare result

	do {
		// Iteratively short list possible sibling node matches
		// by comparing the top node at this level and each of its siblings
		// to what we have in the token

		// Get the first candidate node
		MapGetASTAByID(g_map_id_walker, &temp_asta);

		// Node type will determine how we compare it to the token
		switch (temp_asta.type) {
			case AST_PARAM_DATE:	cr = Compare_N_PARAM_DATE(token, token_size, &temp_asta);		break;
			case AST_PARAM_TIME:	cr = Compare_N_PARAM_TIME(token, token_size, &temp_asta);		break;
			case AST_PARAM_FLOAT: 	cr = Compare_N_PARAM_FLOAT(token, token_size, &temp_asta);		break;
			case AST_PARAM_INTEGER:	cr = Compare_N_PARAM_INTEGER(token, token_size, &temp_asta);	break;
			case AST_PARAM_STRING:	cr = Compare_N_PARAM_STRING(token, token_size, &temp_asta);		break;
			case AST_LOOKUP:		cr = Compare_N_LOOKUP(token, token_size, &temp_asta);			break;
			case AST_ENUM_ARRAY:	cr = Compare_N_IDENTIFIER(token, token_size, &temp_asta);		break;
			case AST_KEYWORD:		cr = Compare_N_KEYWORD(token, token_size, &temp_asta);			break;
			default: break;
		};

		if ((cr > 0) && (g_mflags.help_active == 0)) {
			// the node is a possible match to be included
			TLAddASTAToTokenList(matched_list, temp_asta);
		}

		#ifdef ITCH_DEBUG
			TLDumpList(matched_list);
		#endif //ITCH_DEBUG

		if ((g_mflags.help_active == 1)) {
			// help active, send it
			ITCHWriteChar('\t');
			ITCHWriteLine(temp_asta.label);
		}

		//set up for the siblings (if any)
		g_map_id_walker = temp_asta.next_sibling;
	} while (g_map_id_walker != 0); // reached end of sibling list
}

uint8_t MapEvaluateMatchedList(TokenList* matched_list) {
	// filter results based on matching precedence
	// If any of these are present together in the matched
	// list (of entered data, against the available nodes)
	// then the order of precedence is:
	// 	Keyword is partially or fully matched
	// 	Member of a selected identifier list - fully matched
	//  A look up value in full
	//	A time or date param - distinguishable by its format
	//	string, int, float params
	ASTA_Node temp_asta;

	#ifdef ITCH_DEBUG
		Serial.println("MapEvaluateMatchedList, matched_list");
		TLDumpList(matched_list);
	#endif //ITCH_DEBUG

	if (g_mflags.help_active == 1) {
		// do nothing, don't filter - keep everything for help display
		return MR_HELP_ACTIVE;
	} else {
		if (TLIsEmpty(matched_list)) {
			if(g_mflags.lookup_tbd == 1) {
				return MR_LOOKUP_TBD;
			} else {
				if (g_mflags.ident_member_tbd == 1) {
					return MR_IDENT_MEMBER_TBD;
				} else {
					// Dont need to do any more
					return MR_NO_MATCH;
				}
			}
		}
		if ((TLGetSize(matched_list)) == 1) {
			// single entry - no filtering required
			//  its either uniquely matched and can continue to next token
			//	or its unique with an action attached (if EOL is next)
			MapGetASTAByID(TLGetCurrentID(matched_list), &temp_asta);
			g_mflags.last_matched_id = temp_asta.id; // to advance from when there's a unique match
			if (temp_asta.action) {
				return MR_ACTION_POSSIBLE;
			} else {
				return MR_UNIQUE;
			}
		}
		// multi entries - either some precedent matching  to bring it
		// down to 1 or a MR_MULTI returned indicating that it can't be uniquely
		// matched without further input
		// XXX unless its a potential Look-Ahaed case......

		// Call MatchReduce and deal with the result - which is the number of
		// matching nodes left after the reduce
		switch (MapMatchReduce(matched_list)) {
			case 0: return MR_ERROR; break;
			case 1:
				MapGetASTAByID(TLGetCurrentID(matched_list), &temp_asta);
				g_mflags.last_matched_id = temp_asta.id; // to advance from when there's a unique match
				if (temp_asta.action) {
					return MR_ACTION_POSSIBLE;
				} else {
					return MR_UNIQUE;
				}
				break;
			default: return MR_MULTI; break;
		}
	}
}

uint8_t MapMatchReduce(TokenList* list) {
	// filter results based on matching precedence
	// If any of these are present together in the matched
	// list (of entered data, against the available nodes)
	// then the order of precedence is:
	// 	Keyword is partially or fully matched
	// 	Member of a selected identifier list - fully matched
	//  A look up value in full
	//	A time or date param - distinguisable by its format
	//	A string, int, float params

	// algorithm:
	// check keywords
	// if 2 or more keywords, return multi
	// if single keyword - drop rest, return unique
	// else (0 keywords,  check enum-array
	// if 2 or more enum-array, return multi
	// if 1 enum-array drop rest (would have been looked up in matching process), return uinique
	// if 0 enum-array check lookup
	// if 2 or more lookup, return multi
	// if 1 lookup and no others - would have been caught as unique and dealt with in MatchEvaluate
	//		return error XXX (when implemented)
	// if 0 lookup, check rest
	// XXX if more than 1 at this point, then it can't be parsed
	// because it requires a LOOK-RIGHT, on a user parameter to branch
	// TODO how to trigger back an error from here more elegantly?
	// else check for this condition during lexer

	#ifdef ITCH_DEBUG
		Serial.println("MapMatchReduce, list");
		TLDumpList(list);
	#endif //ITCH_DEBUG

	uint8_t count;
	uint8_t type;

	// Work through the types in order of precedence - one by one
	// Starting with the two that have values known to the parser
	for (type = AST_KEYWORD; type <= AST_ENUM_ARRAY; type++) {
		count = TLCountByType(list, type);
		if (count > 1) {
			// return > 1 - still need more chars to determine uniqueness
			return TLGetSize(list);
		} else {
			// count is 0 or 1
			if (count == 1) {
				// count of this type unique
				// drop the rest of list
				for (int i = (type + 1); i < LAST_AST_TYPE; i++) {
					TLDeleteByType(list, i);
				}
				// double check - should now definitely be 1
				// TODO: redundant?
				count = TLGetSize(list);
				if (count == 1) {
					return 1;
				} else {
					// error - count  expected to be 1 but did not achieve a unique match
					#ifdef ITCH_DEBUG
						strcpy_itch_debug(g_debug_message, ITCH_DEBUG_MATCH_REDUCE_FATAL_SIZE_NOT_1);
						M(g_debug_message);
					#endif

					g_mflags.error_code = ME_MATCHREDUCE_EXPECTED_1;

					return 0;
				}
			}
		}
	}

	// TODO: not sure why LOOKUPs can be treated in the previous loop??
	type = AST_LOOKUP;
	count = TLCountByType(list, type);
	if (count > 1) {
		return TLGetSize(list);
	} else {
		if (count == 1) {
			if (TLGetSize(list) == 1) {

				#ifdef ITCH_DEBUG
					strcpy_itch_debug(g_debug_message, ITCH_DEBUG_MATCH_REDUCE_UNIQUE_NODE_PASSED_TO_REDUCER);
					M(g_debug_message);
				#endif

				g_mflags.error_code = PE_LOOKUP_PASSED_TO_REDUCER;

				return 0;
			}
		}
	}

	#ifdef ITCH_DEBUG
		strcpy_itch_debug(g_debug_message, ITCH_DEBUG_MATCH_REDUCE_NEXT_DEP_MULTIPLE_USER_PARAM_NODES);
		M(g_debug_message);
	#endif

	g_mflags.error_code = PE_MULTIPLE_PARAM_LOOKAHEAD;
	//TODO This should really best detected in the lexer....

	return 0;
}

char* MapGetLastTargetString(void) {
	return g_map_last_target_str;
}

uint8_t MapGetLastTargetStringSize(void) {
	return g_map_last_target_size;
}

void MapUpdateLineSize(uint8_t size) {
	g_map_line_size = size;
}

uint8_t MapAdvance(uint8_t in_buf_idx) {
	// Last_matched_id is the uniquely matched node
	//
	// Move to first child so that subsequent matching
	// will occur against the child and its siblings

	//ASTA_Node temp_asta;
	//MapGetASTAByID(g_mflags.last_matched_id, &temp_asta);

	// set the current_id for the next matching iteration to
	// the first child of the last matched.
	//g_map_id_current = temp_asta.first_child;
	g_map_id_current = MapGetASTAChildIDByID(g_mflags.last_matched_id);
	// advance the start pointer (line_pos) in the match
	// buffer so that the next match starts on the next token
	g_map_line_pos = in_buf_idx;
	g_map_line_size = in_buf_idx;

	#ifdef ITCH_DEBUG
		strcpy_itch_debug(g_debug_message, ITCH_DEBUG_MAP_ADVANCE);
		M(g_debug_message);
	#endif

	return g_map_id_current;
}

char* MapCopyActionStringByID(uint8_t asta_id, char* action_str) {
	// Copy the action identifier string of the specified asta_id to action_string

	uint8_t asta_idx = 0;	// index into the asta array

	while (asta_idx < AST_NODE_COUNT) {
		#ifdef PLATFORM_ARDUINO
			if ( ((uint8_t)pgm_read_byte(&(asta[asta_idx].id))) == asta_id) {
				return strcpy_P(action_str, asta[asta_idx].action_identifier);
			}
		#endif //PLATFORM_ARDUINO
	#ifdef PLATFORM_LINUX
			if((asta[asta_idx].id) == asta_id) {
				return strcpy(action_str, asta[asta_idx].action_identifier);
			}
	#endif //PLATFORM_LINUX
		asta_idx++;
	}
	ITCHWriteLineError(ME_COPYASTAACTIONSTRING_FAIL);
	return NULL;
}

char* MapCopyASTALabelByID(uint8_t asta_id, char* label_str) {
	// Copy the label to label_str of the ASTA Label specified by asta_id
	uint8_t asta_idx = 0;	// index into the asta array

	while (asta_idx < AST_NODE_COUNT) {
		#ifdef PLATFORM_ARDUINO
			if ( ((uint8_t)pgm_read_byte(&(asta[asta_idx].id))) == asta_id) {
				return strcpy_P(label_str, asta[asta_idx].label);
			}
		#endif //PLATFORM_ARDUINO
	#ifdef PLATFORM_LINUX
			if((asta[asta_idx].id) == asta_id) {
				return strcpy(label_str, asta[asta_idx].label);
			}
	#endif //PLATFORM_LINUX
		asta_idx++;
	}
	ITCHWriteLineError(ME_COPYASTALABELSTRING_FAIL);
	return NULL;
}


/*
uint8_t MapGetAction(uint8_t asta_id, char* action_str) {
	ASTA_Node temp_asta;
	uint8_t result;
	result = MapGetASTAByID(asta_id, &temp_asta);
	if (result == PEME_NO_ERROR) {
		strcpy(action_str, temp_asta.action_identifier);
	}
	return result;
}
*/

uint8_t MapGetLastMatchedID() {
	return g_mflags.last_matched_id;
}

uint8_t MapGetErrorCode() {
	return g_mflags.error_code;
}

