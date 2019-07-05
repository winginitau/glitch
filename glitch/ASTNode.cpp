/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: ASTNode.cpp
 - Nodes in the AST
*******************************************************************/

#include <ASTNode.h>
#include <stddef.h>

ASTNode::ASTNode() {
	id = 0;
	term_level = 0;
	label[0] = '\0';
	unique[0] = '\0';
	help[0] = '\0';
	type = AST_UNDEFINED;
	action = false;
	mode_change = false;
	finish = false;
	parent = NULL;
	first_child = NULL;
	next_sibling = NULL;

	action_identifier[0] = '\0';
	mode_change_identifier[0] = '\0';
}

ASTNode::~ASTNode() {
	// Auto-generated destructor stub
}

