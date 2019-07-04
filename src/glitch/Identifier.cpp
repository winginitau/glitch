/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: identifier.cpp
 - Multivariate node and sub-list / pairs in the name space of
   identifiers that gets defined by a grammar
********************************************************************/

#include <Identifier.h>


Identifier::Identifier() {
	IdentifierName[0] = '\0';
	Type = ID_TYPE_UNDEFINED;
	func_xlat = 0;
	action_built = false;
}

Identifier::~Identifier() {
	// Auto-generated destructor stub
}

