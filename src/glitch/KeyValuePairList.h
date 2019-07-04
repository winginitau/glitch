/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: KeyValuePairList.h
 - Self describing
********************************************************************/

#ifndef KEYVALUEPAIRLIST_H_
#define KEYVALUEPAIRLIST_H_

#include <StringList.h>

class KeyValuePairList {
public:
	KeyValuePairList();
	virtual ~KeyValuePairList();
	bool Add(char* key_str, char* value_str);
	void WalkKeys(bool (*CallBack)(const char *));
	void WalkValues(bool (*CallBack)(const char *));
	char* GetValue(const char* key, char* value);
	uint16_t GetSize(void);
	uint16_t GetLocation(const char* match_key);
	void GetPairAtLocation(char* key, char* value, uint16_t location);
private:
	StringList key_list;
	StringList value_list;
};

#endif /* KEYVALUEPAIRLIST_H_ */
