/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt
 See also README.txt

 File: OutputBuffer.h
 - Derived from StringList
 - Used as a line staging queue for file outputs as they are
   built up by the lexer and parser-code generation routines.
******************************************************************/

#ifndef OUTPUTBUFFER_H_
#define OUTPUTBUFFER_H_

#include <StringList.h>

class OutputBuffer: public StringList {
public:
	OutputBuffer();
	virtual ~OutputBuffer();

	bool OutputAvailable();
	char* GetOutputAsString(char* output_str);
	void SetOutputAvailable();
private:
    bool output_available;
};


#endif /* OUTPUTBUFFER_H_ */
