/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt
 See also README.txt

 File: StringList.h
 - Linked list of C strings
******************************************************************/

#ifndef STRINGLIST_H_
#define STRINGLIST_H_

#include <stdint.h>

typedef struct STRING_NODE {
	char* str;
	struct STRING_NODE* next;
} StringNode;

class StringList {
public:
	StringList();
	virtual ~StringList();

	bool AddString(const char * str);
	bool IsIn(const char * str);
	void WalkList(bool (*CallBack)(const char *));
	bool EnQueue(const char * str);
	char* DeQueue(char* str);
	uint16_t GetSize(void);
	uint16_t GetLocation(const char* match_str);
	char * GetStringAtLocation(char* result, uint16_t location);
	void Reset();
private:
	void ResetFromHead(StringNode* s);
	StringNode* head;
};

#endif /* STRINGLIST_H_ */
