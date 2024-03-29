/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: AST.cpp
 - Builds an abstract syntax tree of the grammar
 - Writes grammar dependent parser code
*******************************************************************/

#include <AST.h>
//#include <glitch_config.h>
#include <glitch_errors.h>
#include <OutputBuffer.h>

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

AST::AST() {
	root = NULL;
	current = NULL;
	current_level = 0;
	next_id = 1;

    output_string[0] = '\0';

    ast_node_count = 0;
	grammar_def_count = 0;
	max_param_count = 0;
	max_grammar_depth = 0;

	caller_func_preamble_done = false;

	user_code_hints = false;
    user_code_debug = false;
    user_code_call_through[0] = '\0';
    user_code_call_through_commented = false;
}

AST::~AST() {
	// Auto-generated destructor stub
}

int AST::AddSiblingToCurrent(ASTNode * node) {
	ASTNode* walker;
	if(root == NULL) {		//first node should always be a child of root
		return E_SIBLING_TO_ROOT_ATTEMPTED;
	}

	if(current->next_sibling == NULL) {
		// check for duplicate keyword entries as we walk
		if ((current->type == AST_KEYWORD) && (node->type == AST_KEYWORD )) {
			if (strcmp(current->label, node->label) == 0) {
				return E_DUPLICATE_KEYWORD_AT_LEVEL;
			}
		}
		current->next_sibling = node;
	} else {
		walker = current->next_sibling;
		while(walker->next_sibling != NULL) {
			// check for duplicate keyword entries as we walk
			if ((walker->type == AST_KEYWORD) && (node->type == AST_KEYWORD )) {
				if (strcmp(walker->label, node->label) == 0) {
					return E_DUPLICATE_KEYWORD_AT_LEVEL;
				}
			}
			walker = walker->next_sibling;
		}
		walker->next_sibling = node;
	}
	node->next_sibling = NULL;
	node->first_child = NULL;
	node->parent = current->parent;
	current = node;
	return E_NO_ERROR;
}

int AST::AddChildToCurrent(ASTNode* node) {
	ASTNode* walker;

	if (root == NULL) {		//special case - first node from level 0
		root = node;
		node->parent = NULL;
	} else {
		if(current->first_child == NULL) {
			current->first_child = node;
		} else {
			walker = current->first_child;
			while(walker->next_sibling != NULL) {
				// check for duplicate keyword entries as we walk
				if ((walker->type == AST_KEYWORD) && (node->type == AST_KEYWORD )) {
					if (strcmp(walker->label, node->label) == 0) {
						return E_DUPLICATE_KEYWORD_AT_LEVEL;
					}
				}
				walker = walker->next_sibling;
			}
			walker->next_sibling = node;
		}
		node->parent = current;
	}

	node->first_child = NULL;
	node->next_sibling = NULL;
	current = node;
	return E_NO_ERROR;
}


int AST::ValidateTermTypeStr(const char* type_str) {
	// check for valid term type and return it
	int i;
	for (i = LAST_AST_TYPE; i != 0; i--) {
		if (strcmp(ast_type_strings[i].text, type_str) == 0) {
			return i;
		}
	}
	return i;
}

bool AST::CheckForExistingSiblingKeywords(ASTNode* start_node, const char* keyword) {
	ASTNode* temp;

	if (start_node == NULL) {
		// empty tree?
		return false;
	}
	// get to the top of the sibling list at this level
	if (start_node->parent == NULL) {
		// must be a level 1 node
		temp = root;
	} else {
		temp = start_node->parent->first_child;
	}
	// now at first member of a sibling group
	if (temp->type == AST_KEYWORD) {
		if (strcmp(temp->label, keyword) == 0) {
			// duplicate found
			return true;
		}
	}
	while (temp->next_sibling != NULL) {
		temp = temp->next_sibling;
		if (temp->type == AST_KEYWORD) {
			if (strcmp(temp->label, keyword) == 0) {
				// duplicate found
				return true;
			}
		}
	}
	return false;
}

int AST::NewNode(int term_level, const char* term_type) {
	return NewNode(term_level, term_type, NULL);
}

int AST::NewNode(int term_level, const char* term_type, const char* term) {

	// increment the count (for export to the parser)
	ast_node_count++;

	// create a new AST node
	ASTNode* n;
	n = new(ASTNode);

	n->id = next_id; 	// next_id - just an incrementing int already set to the next available.
	next_id++;
	if(term != NULL) strcpy(n->label, term);
	n->term_level = term_level;

	// check for valid term type and set it
	n->type = ValidateTermTypeStr(term_type);
	if (n->type == 0) {
		return E_TERM_TYPE_NOT_RECOGNISED;
	}

	// check that term level is the same or incrementing (ie adding a leaf node to the tree)
	if (term_level > current_level + 1) {
		return E_TERM_LEVEL_NOT_INCRMENTAL;
	}

	// Perform some naming rule checks
	// - dont need to here - regex checked when adding to identifier lists

	// Add as a sibling or child as appropriate to level
	if (term_level == current_level +1) {
		current_level = term_level;
		return AddChildToCurrent(n);
	}
	if (term_level == current_level) {
		return AddSiblingToCurrent(n);
	}
	while (term_level < current_level) {
		current_level--;
		current = current->parent;
	}
	return AddSiblingToCurrent(n);
}

void AST::WriteASTArray(Identifiers* idents) {

	grammar_def_count = 0;

	DT(root, idents, false);  // to count the output lines

    sprintf(output_string, "\n// id, type, label, actionable, parent, first_child, next_sibling, action_id");
	header_output_queue.EnQueue(output_string);

	sprintf(output_string, "\n#ifdef PLATFORM_ARDUINO\n");
	header_output_queue.EnQueue(output_string);
	sprintf(output_string, "const ASTA_Node asta [%d] PROGMEM = {\n", grammar_def_count);
	header_output_queue.EnQueue(output_string);
	sprintf(output_string, "#else\n");
	header_output_queue.EnQueue(output_string);
	sprintf(output_string, "const ASTA_Node asta [%d] = {\n", grammar_def_count);
	header_output_queue.EnQueue(output_string);
	sprintf(output_string, "#endif //PLATFORM_ARDUINO\n");
	header_output_queue.EnQueue(output_string);

	DT(root, idents, true);

	sprintf(output_string, "};\n\n");
	header_output_queue.EnQueue(output_string);

	header_output_queue.SetOutputAvailable();
}

void AST::DT(ASTNode* w, Identifiers* idents, bool print) {
	// Dump Tree: asta tree walk, node count, optionally writing it as a vector array.
	char temp[MAX_TEMP_BUFFER_LENGTH];
	char instance_name[MAX_BUFFER_LENGTH];
	char type_string[MAX_BUFFER_LENGTH];

	if (w != NULL) {
//		sprintf(output_string, "\t%u, \"%s\", \"%s\", \"%s\", %u, %u, %d, %d, ", w->id, w->label, w->unique, w->help, w->term_level, w->type, w->action, w->finish);
		sprintf(output_string, "\t%u, %u, ", w->id, w->type);
		switch (w->type) {
			case AST_KEYWORD:
			case AST_LOOKUP:
				sprintf(temp, "\"%s\", ", w->label);
				break;
			case AST_ENUM_ARRAY:

				// output instance name rather than label
				idents->GetInstanceName(w->label, instance_name);

				sprintf(temp, "\"%s\", ", instance_name);
				break;
			case AST_PARAM_DATE:
			case AST_PARAM_TIME:
			case AST_PARAM_INTEGER:
			case AST_PARAM_FLOAT:
			case AST_PARAM_STRING:
				GetASTTypeString(type_string, w->type);
				sprintf(temp, "\"%s\", ", type_string);
				break;
			default:
				break;
		}
		strcat(output_string, temp);
		sprintf(temp, "%u, ", w->action);
		strcat(output_string, temp);
		if (w->parent != NULL) {
			sprintf(temp, "%u, ", w->parent->id);
		} else {
			sprintf(temp, "0, ");
		}
		strcat(output_string, temp);

		if (w->first_child != NULL) {
			sprintf(temp, "%u, ", w->first_child->id);
		} else {
			sprintf(temp, "0, ");
		}
		strcat(output_string, temp);

		if (w->next_sibling != NULL) {
			sprintf(temp, "%u, ", w->next_sibling->id);
		} else {
			sprintf(temp, "0, ");
		}
		strcat(output_string, temp);

		sprintf(temp, "\"%s\",\n", w->action_identifier);
		strcat(output_string, temp);

		grammar_def_count++;

		if (print) header_output_queue.EnQueue(output_string);

		DT(w->first_child, idents, print);
		DT(w->next_sibling, idents, print);
	}
}


int AST::GetASTTypeString(char* return_string, int type) {
	// check for valid term type and return it
	int i;
	for (i = LAST_AST_TYPE; i != 0; i--) {
		if(i == type) {
			strcpy(return_string, ast_type_strings[i].text);
			return i;
		}
	}
	return i;
}

void AST::AttachActionToCurrent(char* action_identifier) {
	current->action = true;
	strcpy(current->action_identifier, action_identifier);
}

void AST::AttachModeChangeToCurrent(char* mode_change_identifier) {
	current->mode_change = true;
	strcpy(current->mode_change_identifier, mode_change_identifier);
}


int AST::BuildActionCode(Identifiers& idents) {
	// While on the AST node that is action-able - "current", walk via
	// 	parent pointers up the tree, building the function parameter
	// 	list that will be included in the function prototype and definition
	//	for this action and send it to the queues for the output files.
	// Also count the depth of the grammar to define max_grammar_depth
	//  which is used in the itch parser to constrain node match list
	//  building to a statically sized array rather than smashing the heap
	//  with malloc and free

	KeyValuePairList params;
	ASTNode* walker;
	int param_count = 0;
	int param_index = 0;
	int grammar_depth = 0;
	char param_name[MAX_BUFFER_LENGTH];
	char param_type[MAX_BUFFER_LENGTH];
	char func_name[MAX_BUFFER_LENGTH];
	int func_xlat_id;
	char temp[MAX_BUFFER_LENGTH];
	char temp2[MAX_BUFFER_LENGTH];

	// ***************************** Check if the action code for this action is already built?
	if (idents.GetActionBuilt(current->action_identifier) == true) {
		return I_ACTION_ALREADY_BUILT;
	}

	// ***************************** Count the Params for the Function Definition and Prototype
	walker = current;
	while (walker != NULL ) {
		// count the num of params that need to be included in the function
		if (walker->type > AST_KEYWORD) {
			param_count++;
		}
		// count the ones that needs to be numerically labelled
		if (walker->type >= AST_LOOKUP) {
			param_index++;
		}
		// count the total grammar depth
		grammar_depth++;
		// walk "up"
		walker = walker->parent;
	}

	// remember the max counts for #defines in the parser
	if (param_count > max_param_count) max_param_count = param_count;
	if (grammar_depth > max_grammar_depth) max_grammar_depth = grammar_depth;

	// ***************************** Assemble param names and types walking up tree

	walker = current;
	while (walker != NULL) {
		switch (walker->type) {
			case AST_UNDEFINED:
				return E_AST_NODE_UNDEFINED_TYPE;
				break;
			case AST_KEYWORD:
				// ignore - it got us to here anyway
				break;
			case AST_ENUM_ARRAY:
				if (idents.Exists(walker->label)) {
					sprintf(param_type, "uint16_t");
					params.Add(walker->label, param_type);
				} else
					return E_BUILDING_ACTION_PROTO;
				break;
			case AST_LOOKUP:
				if (idents.Exists(walker->label)) {
					sprintf(param_type, "char*");
					params.Add(walker->label, param_type);
				} else
					return E_BUILDING_ACTION_PROTO;
				break;
			case AST_PARAM_STRING:
				sprintf(param_type, "char*");
				sprintf(param_name, "param%u_string", param_index);
				params.Add(param_name, param_type);
				param_index--;
				break;
			case AST_PARAM_INTEGER:
				sprintf(param_type, "int16_t");
				sprintf(param_name, "param%u_int", param_index);
				params.Add(param_name, param_type);
				param_index--;
				break;
			case AST_PARAM_FLOAT:
				sprintf(param_type, "float");
				sprintf(param_name, "param%u_float", param_index);
				params.Add(param_name, param_type);
				param_index--;
				break;
			case AST_PARAM_TIME:
				sprintf(param_type, "char*");
				sprintf(param_name, "param%u_time", param_index);
				params.Add(param_name, param_type);
				param_index--;
				break;
			case AST_PARAM_DATE:
				sprintf(param_type, "char*");
				sprintf(param_name, "param%u_date", param_index);
				params.Add(param_name, param_type);
				param_index--;
				break;
		}
		walker = walker->parent;
	}

	// ****************************** Get the function name
	// stored in instance_name of the idents headers
	if (idents.GetInstanceName(current->action_identifier, func_name) != E_NO_ERROR) {
		return E_BUILDING_ACTION_PROTO;
	}

	// ****************************** Get the function's xlat ID
	func_xlat_id = idents.GetXlatID(current->action_identifier);

	// *******************************   Function Caller Preamble - ONCE!
	// write the preamble for the function caller into the code file - Once!
	if(caller_func_preamble_done == false) {
		sprintf(output_string, "uint16_t CallFunction(uint8_t func_xlat, ParamUnion params[]) {\n");
		code_output_queue.EnQueue(output_string);
		sprintf(output_string, "\tswitch (func_xlat) {\n");
		code_output_queue.EnQueue(output_string);
		caller_func_preamble_done = true;
	}

	// ********************************* Header and User_Code function 1st line
	strcpy(output_string, "void ");
	sprintf(temp, "%s(", func_name);
	strcat(output_string, temp);

	// ***************************no params - (void)
	if (param_count == 0) {
		// preserve the line so far
		strcpy(temp2, output_string);

		// Header Proto
		strcat(output_string, "void);\n");
		header_output_queue.EnQueue(output_string);

		// Restore the line preserved earlier
		strcpy(output_string, temp2);

		// User_Code function 1st line
		sprintf(temp, "void) {\n");
		strcat(output_string, temp);
		user_code_output_queue.EnQueue(output_string);

	} else {
		// *************************** param list
		for (int i = param_count - 1; i > 0; i--) {
			params.GetPairAtLocation(param_name, param_type, i);
			sprintf(temp, "%s %s, ", param_type, param_name);
			strcat(output_string, temp);
		}

		// Preserve to far
		strcpy(temp2, output_string);

		// *************************************** last param and close - Prototype
		params.GetPairAtLocation(param_name, param_type, 0);
		sprintf(temp, "%s %s);\n", param_type, param_name);
		strcat(output_string, temp);
		header_output_queue.EnQueue(output_string);

		// Restore the line preserved earlier
		strcpy(output_string, temp2);

		// *************************************** last param and func open - User_Code
		sprintf(temp, "%s %s) {\n", param_type, param_name);
		strcat(output_string, temp);
		user_code_output_queue.EnQueue(output_string);
	}

	// ******************************************* Function caller Case:
	// No params - () call and close case
	if (param_count == 0) {
		sprintf(output_string, "\t\tcase %d:\n", func_xlat_id);
		code_output_queue.EnQueue(output_string);
		sprintf(output_string, "\t\t\t%s();\n", func_name);
		code_output_queue.EnQueue(output_string);
		sprintf(output_string, "\t\t\tbreak;\n");
		code_output_queue.EnQueue(output_string);
	} else {
		// Params to enumerate
		sprintf(output_string, "\t\tcase %d:\n", func_xlat_id);
		code_output_queue.EnQueue(output_string);
		sprintf(output_string, "\t\t\t%s(", func_name);

		// Iterate and enumerate the params
		int e = 0;
		for (int i = param_count - 1; i > 0; i--) {
			params.GetPairAtLocation(param_name, param_type, i);

			//remove the *
			char* star;
			star = strrchr(param_type, '*');
			if (star) {
				*star = '\0';
				strcat(param_type, "_star");
			}

			sprintf(temp, "params[%d].param_%s, ", e, param_type);
			strcat(output_string, temp);
			e++;
		}

		// Last param and close the call and case
		params.GetPairAtLocation(param_name, param_type, 0);

		//remove the *
		char* star;
		star = strrchr(param_type, '*');
		if (star) {
			*star = '\0';
			strcat(param_type, "_star");
		}

		// form the last param and write the close and case break;
		sprintf(temp, "params[%d].param_%s);\n", param_count -1 , param_type);
		strcat(output_string, temp);
		code_output_queue.EnQueue(output_string);
		sprintf(output_string, "\t\t\tbreak;\n");
		code_output_queue.EnQueue(output_string);
	}


	//ShowBlockByCategory(params[0].param_int16);
	//char** ShowBlockCatN(int block_category, int param1_int);
	//ShowBlockCatN(params[0].param_int16, params[1].param_int16);

	// *************************************** User_Code function body

	// include user code hints if specified
	if (user_code_hints) {
		sprintf(output_string, "\t// >>>\n");
		user_code_output_queue.EnQueue(output_string);
		sprintf(output_string, "\t// >>> INSERT ACTION CODE HERE\n");
		user_code_output_queue.EnQueue(output_string);
		sprintf(output_string, "\t// >>> (Debug and call-through code may follow if specified in the grammar\n");
		user_code_output_queue.EnQueue(output_string);
		sprintf(output_string, "\t// >>>\n");
		user_code_output_queue.EnQueue(output_string);
	}

	// have the example code print debug info (func and param types / value)
	//	storing the strings in PROGMEM on Arduino to save memory
	if (user_code_debug) {
		sprintf(output_string, "\tchar temp[MAX_OUTPUT_LINE_SIZE];\n");
		user_code_output_queue.EnQueue(output_string);

		char arduino_output[MAX_BUFFER_LENGTH];
		char linux_output[MAX_BUFFER_LENGTH];
		char arduino_temp[MAX_BUFFER_LENGTH];
		char linux_temp[MAX_BUFFER_LENGTH];


		sprintf(linux_output, "\t\tsprintf(temp, \"Call: %s(...) (", func_name);
		sprintf(arduino_output, "\t\tsprintf_P(temp, PSTR(\"Call: %s(...) (", func_name);

		if (param_count == 0) {
			sprintf(linux_temp, "void)\\n\\n\\r\");\n");
			sprintf(arduino_temp, "void)\\n\\n\\r\"));\n");
			strcat(linux_output, linux_temp);
			strcat(arduino_output, arduino_temp);
		} else {
			// write the param format specifiers - except the last one
			for (int i = param_count - 1; i > 0; i--) {
				params.GetPairAtLocation(param_name, param_type, i);
				if ( (strcmp(param_type, "uint16_t") == 0) || (strcmp(param_type, "int16_t") == 0) ) {
					sprintf(temp, "%s %s(%%d), ", param_type, param_name);
				} else {
					if (strcmp(param_type, "char*") == 0) {
						sprintf(temp, "%s %s(%%s), ", param_type, param_name);
					} else {
						if (strcmp(param_type, "float") == 0) {
							sprintf(temp, "%s %s(%%f), ", param_type, param_name);
						} else {
							printf("ERROR Param Type not recognised building User Function code example\n\n");
							exit(-1);
						}
					}
				}
				strcat(linux_output, temp);
				strcat(arduino_output, temp);
			}
			// now the last param format specifier - closing the string to print
			params.GetPairAtLocation(param_name, param_type, 0);
			if ( (strcmp(param_type, "uint16_t") == 0) || (strcmp(param_type, "int16_t") == 0) ) {
				sprintf(linux_temp, "%s %s(%%d))\\n\\r\", ", param_type, param_name);
				sprintf(arduino_temp, "%s %s(%%d))\\n\\r\"), ", param_type, param_name);
			} else {
				if (strcmp(param_type, "char*") == 0) {
					sprintf(linux_temp, "%s %s(%%s))\\n\\r\", ", param_type, param_name);
					sprintf(arduino_temp, "%s %s(%%s))\\n\\r\"), ", param_type, param_name);
				} else {
					if (strcmp(param_type, "float") == 0) {
						sprintf(linux_temp, "%s %s(%%f))\\n\\r\", ", param_type, param_name);
						sprintf(arduino_temp, "%s %s(%%f))\\n\\r\"), ", param_type, param_name);
					} else {
						printf("ERROR Param Type not recognised building User Function code example\n\n");
						exit(-1);
					}
				}
			}
			strcat(linux_output, linux_temp);
			strcat(arduino_output, arduino_temp);

			// Write variable names that relate the param format specifiers
			for (int i = param_count - 1; i > 0; i--) {
				params.GetPairAtLocation(param_name, param_type, i);
				sprintf(temp, "%s, ", param_name);
				strcat(linux_output, temp);
				strcat(arduino_output, temp);
			}

			// And close the call
			params.GetPairAtLocation(param_name, param_type, 0);
			sprintf(temp, "%s);\n", param_name);
			strcat(linux_output, temp);
			strcat(arduino_output, temp);
		}

		// write out the arduino and linux versions with the pre-proc logic
		sprintf(output_string, "\t#ifdef PLATFORM_ARDUINO\n");
		user_code_output_queue.EnQueue(output_string);

		user_code_output_queue.EnQueue(arduino_output);

		sprintf(output_string, "\t#else\n");
		user_code_output_queue.EnQueue(output_string);

		user_code_output_queue.EnQueue(linux_output);

		sprintf(output_string, "\t#endif //PLATFORM_ARDUINO\n");
		user_code_output_queue.EnQueue(output_string);
	}

	// rest of the user_code function body and closures
	if(user_code_hints) {
		sprintf(output_string, "\t// >>>\n");
		user_code_output_queue.EnQueue(output_string);
		sprintf(output_string, "\t// >>> Results Callback via ITCH WriteLineCallback\n");
		user_code_output_queue.EnQueue(output_string);
		sprintf(output_string, "\t// >>>\n");
		user_code_output_queue.EnQueue(output_string);
	}

	if(user_code_debug) {
		sprintf(output_string, "\tWriteLineCallback(temp);\n");
		user_code_output_queue.EnQueue(output_string);
	}

	// Write a function call to an external function, adding the call back pointer
	if (user_code_call_through[0] != '\0') { //it is either default "My" or user set

		if(user_code_call_through_commented) {
			sprintf(output_string, "\t//");
		} else {
			sprintf(output_string, "\t");
		}

		strcat(output_string, user_code_call_through);
		sprintf(temp, "%s(", func_name);
		strcat(output_string, temp);

		if (param_count == 0) {
			// Just write the callback and close it
			sprintf(temp, "WriteLineCallback);\n");
			strcat(output_string, temp);
		} else {
			// write the param list into the external call - including the last one
			for (int i = param_count - 1; i >= 0; i--) {
				params.GetPairAtLocation(param_name, param_type, i);
				sprintf(temp, "%s, ", param_name);
				strcat(output_string, temp);
			}
			// then the callback and close
			sprintf(temp, "WriteLineCallback);\n");
			strcat(output_string, temp);
		}
		user_code_output_queue.EnQueue(output_string);
	}

	// Close the the whole function
	sprintf(output_string, "}\n\n");
	user_code_output_queue.EnQueue(output_string);

	header_output_queue.SetOutputAvailable();
	user_code_output_queue.SetOutputAvailable();
	code_output_queue.SetOutputAvailable();

	// Flag that this identifier's action code has now been built
	idents.SetActionBuilt(current->action_identifier, true);

	return E_NO_ERROR;
}

int AST::DetermineUnique(void) {
	return 0;
}
