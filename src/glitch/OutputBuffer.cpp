/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt
 See also README.txt

 File: OutputBuffer.cpp
 - Derived from StringList
 - Used as a line staging queue for file outputs as they are
   built up by the lexer and parser-code generation routines.
******************************************************************/

#include <OutputBuffer.h>
//#include <glitch_config.h>



OutputBuffer::OutputBuffer()
	: StringList() {
    output_available = false;;

}

OutputBuffer::~OutputBuffer() {
	// Auto-generated destructor stub
}

bool OutputBuffer::OutputAvailable() {
    return output_available;
}

char* OutputBuffer::GetOutputAsString(char* output_str) {

    DeQueue(output_str);
    if (GetSize() == 0) {
        output_available = false;
    }
    return output_str;
}

void OutputBuffer::SetOutputAvailable(void) {
    if (GetSize() > 0) {
        output_available = true;
    } else {
    	output_available = false;
    }
}
