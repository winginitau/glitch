/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: itch_strings.cpp
 - String literal output write functions
 - Platform dependent - on Arduino string literals are in PROGMEM
 - Provides abstraction for PROGMEM strings to be written to output
 *******************************************************************/

#include <itch_config.h>
#include <itch_strings.h>
#include <itch.h>

#if defined(PLATFORM_LINUX)
#elif defined(PLATFORM_ARDUINO)
#include <Arduino.h>
#else
#error "A PLATFORM_ Directive must be provided"
#endif

void ITCHWriteLineError(uint8_t error_enum) {
	#ifdef PLATFORM_ARDUINO
		ITCHWriteLine_P(itch_error_strings[error_enum].text);
	#endif
	#ifdef PLATFORM_LINUX
		ITCHWriteLine(itch_error_strings[error_enum].text);
	#endif
}

void ITCHWriteLineMisc(uint8_t misc_string_enum) {
	#ifdef PLATFORM_ARDUINO
		ITCHWriteLine_P(misc_itch_strings[misc_string_enum].text);
	#endif
	#ifdef PLATFORM_LINUX
		ITCHWriteLine(misc_itch_strings[misc_string_enum].text);
	#endif
}

void ITCHWriteMisc(uint8_t misc_string_enum) {
	#ifdef PLATFORM_ARDUINO
		ITCHWrite_P(misc_itch_strings[misc_string_enum].text);
	#endif
	#ifdef PLATFORM_LINUX
		ITCHWrite(misc_itch_strings[misc_string_enum].text);
	#endif
}
