/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: ASTNode.h
 - Nodes in the AST
*******************************************************************/

#ifndef ASTNODE_H_
#define ASTNODE_H_

#include <glitch_config.h>
#include <stdint.h>

const EnumStringArray ast_type_strings[LAST_AST_TYPE] = {
		"undefined",
		"keyword",
		"enum-array",
		"lookup",
		"param-date",
		"param-time",
		"param-integer",
		"param-uint8",
		"param-uint16",
		"param-uint32",
		"param-int16",
		"param-int32",
		"param-float",
		"param-string",

};

enum {
	AST_END_YES,
	AST_END_NO,
	AST_END_MAYBE,
};


class ASTNode {
public:
	uint16_t id;
	char label[MAX_BUFFER_WORD_LENGTH];
	char unique[MAX_BUFFER_WORD_LENGTH];
	char help[MAX_BUFFER_LENGTH];
	int term_level;
	int type;
	bool action;
	bool mode_change;
	bool finish;
	ASTNode* parent;
	ASTNode* next_sibling;
	ASTNode* first_child;

	char action_identifier[MAX_BUFFER_WORD_LENGTH];
	char mode_change_identifier[MAX_BUFFER_WORD_LENGTH];

	ASTNode();
	virtual ~ASTNode();

private:

};


#endif /* ASTNODE_H_ */
