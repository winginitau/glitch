/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: AST.h
 - Builds an abstract syntax tree of the grammar
 - Writes grammar dependent parser code
*******************************************************************/

#ifndef AST_H_
#define AST_H_

#include <glitch_config.h>
#include <ASTNode.h>
#include <OutputBuffer.h>
#include <Identifiers.h>

#include <stdint.h>

class AST {
public:
	AST();
	virtual ~AST();

	int ast_node_count;
	int grammar_def_count;
	int max_param_count;
	int max_grammar_depth;

	OutputBuffer header_output_queue;
	OutputBuffer code_output_queue;
	OutputBuffer user_output_queue;
	OutputBuffer user_code_output_queue;

    bool user_code_hints;
    bool user_code_debug;
    char user_code_call_through[MAX_BUFFER_WORD_LENGTH];
    bool user_code_call_through_commented;

	int AddSiblingToCurrent(ASTNode* node);
	int AddChildToCurrent(ASTNode* node);
	int ValidateTermTypeStr(const char * type_str);
	bool CheckForExistingSiblingKeywords(ASTNode* start_node, const char* keyword);
	int NewNode(int term_level, const char* term_type);
	int NewNode(int term_level, const char* term_type, const char* term);
	void WriteASTArray(Identifiers* idents); // needs idents as well as AST to write the AST Array

	int GetASTTypeString(char* return_string, int type);

	void AttachActionToCurrent(char* action_identifier);
	void AttachModeChangeToCurrent(char* mode_change_identifier);

	int BuildActionCode(Identifiers& idents);
	int DetermineUnique(void);

private:
	ASTNode* root;
	ASTNode* current;
	uint8_t current_level;
	uint16_t next_id;
	void DT(ASTNode* tree, Identifiers* idents, bool print);

    char output_string[MAX_OUT_BUFFER_LENGTH];

    bool caller_func_preamble_done;
};

#endif /* AST_H_ */
