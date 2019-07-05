/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt
 See also README.txt

 File: LineBuffer.h
 - General buffer built up char by char from glitch
 - Various functions to assist the lexical processing
 - TODO: move the defines to config?
******************************************************************/

#ifndef LINEBUFFER_H_
#define LINEBUFFER_H_

#include <stdint.h>

#ifndef MAX_BUFFER_LENGTH
#define MAX_BUFFER_LENGTH 200
#endif

#ifndef MAX_BUFFER_WORD_LENGTH
#define MAX_BUFFER_WORD_LENGTH 200
#endif

#ifndef MAX_BUFFER_WORDS_PER_LINE
#define MAX_BUFFER_WORDS_PER_LINE 50
#endif

//#define MAX_IDENTIFIERS 50
#ifndef TOKEN_DELIM
#define TOKEN_DELIM " "
#endif

class LineBuffer {
public:
    LineBuffer();
    virtual ~LineBuffer();
    void Init();
    int AddChar(char ch);
    int GetLine(char* line);
    int Tokenise();
    //void PrintTokens();
    char* GetTokenStr(char* token, uint16_t idx);
    //void PrintRawBuffer();
    char* GetRawBuffer(char* ret_buf);
    bool Available();
    bool IsEmptyLine();
    void Reset();
    char* GetToEOL(char* result, int after_token);
private:
    char buf[MAX_BUFFER_LENGTH];
    char buf_preserve[MAX_BUFFER_LENGTH];
    uint16_t char_idx;
    uint16_t word_idx;
    bool available;
    bool empty_line;
    char* word_list[MAX_BUFFER_WORDS_PER_LINE];
    uint16_t word_count;

};


#endif /* LINEBUFFER_H_ */
