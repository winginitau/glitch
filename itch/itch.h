/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: itch.h
 - Singleton class for inclusion of itch in other projects
 - Static output writing functions
*******************************************************************/

#ifndef ITCH_H_
#define ITCH_H_

#include <itch_config.h>
#include <stdio.h>
#include <stdint.h>

#if defined(PLATFORM_LINUX)
#include <termios.h>
#elif defined(PLATFORM_ARDUINO)
#include <Arduino.h>
#else
#error "A PLATFORM_ Directive must be provided"
#endif

enum ITCH_SESSION_FLAGS {
	ITCH_INIT,
	ITCH_TEXT_DATA,		// Normal operating - just sending log data
	ITCH_TEXT_CCC,		// Command Configure Control
	ITCH_TERMINAL,		// Interactive terminal with basic emulation
	ITCH_BUFFER_STUFF,	// Parsing from the stuff buffer
};


void WriteLineCallback(const char* string);
void ITCHWriteLine(const char* string);
void ITCHWrite(const char* string);
void ITCHWriteChar(const char ch);

void PreserveReplay(void);
void TrimReStuffBuffer(void);
void TrimBuffer(void);

#ifdef PLATFORM_ARDUINO
	size_t ITCHWrite_P(const char *string);
	void ITCHWriteLine_P(const char *string);
	void ITCHWriteLine(const __FlashStringHelper *string);
	void ITCHWrite(const __FlashStringHelper *string);
#endif //PLATFORM_ARDUINO

#ifdef PLATFORM_LINUX
	// handle signals
	//static void INTExitHandler(int sig);
#endif //PLATFORM_LINUX

class ITCH {
public:
	ITCH();
	//virtual ~ITCH();

	#ifdef PLATFORM_ARDUINO
		void Begin(HardwareSerial& port,uint32_t baud);
	#endif //PLATFORM_ARDUINO
	#ifdef PLATFORM_LINUX
		void Begin(FILE* input_stream, FILE* output_stream);
		void RestoreTerm();
	#endif//PLATFORM_LINUX

	void SetMode(uint8_t mode);
	uint8_t StuffAndProcess(char* str);
	void Poll();

private:
	uint8_t i_mode;				// Itch Mode - Data, terminal, command etc
	uint8_t i_replay;			// Replaying on or off
	uint8_t i_term_echo;		// Echo received chars back to the terminal?

	char term_esc_seq[4];
	uint8_t term_esc_idx;
	char ccc_esc_seq[4];
	uint8_t ccc_esc_idx;
};

#endif /* ITCH_H_ */

