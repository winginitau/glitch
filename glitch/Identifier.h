/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: identifier.h
 - Multivariate node and sub-list / pairs in the name space of
   identifiers that gets defined by a grammar
********************************************************************/

#ifndef IDENTIFIER_H_
#define IDENTIFIER_H_

#include <glitch_config.h>
#include <KeyValuePairList.h>

enum {
	ID_TYPE_UNDEFINED = 0,
	ID_ENUM_ARRAY_PAIR,
	ID_ENUM_LIST,
	ID_LOOKUP_LIST,
	ID_ACTION_PAIR,
	LAST_IDENTIFIER_TYPE,
};

const EnumStringArray identifier_types[LAST_IDENTIFIER_TYPE] = {
		"ID_TYPE_UNDEFINED",
		"ID_ENUM_ARRAY_PAIR",
		"ID_ENUM_LIST",
		"ID_LOOKUP_LIST",
		"ID_ACTION_PAIR",
};


class Identifier {
public:

	Identifier();
	virtual ~Identifier();

	char IdentifierName[MAX_BUFFER_LENGTH];
	char InstanceName[MAX_BUFFER_LENGTH];

	int Type;
	int func_xlat;
	bool action_built;

	KeyValuePairList KeyValueList;
	StringList SimpleList;
};

#endif /* IDENTIFIER_H_ */
