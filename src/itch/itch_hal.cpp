/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: itch_hal.cpp
 - Abstraction for string literal C string operations
 - Platform dependent - on Arduino string literals are in PROGMEM
 *******************************************************************/

#include <itch_config.h>
#include <itch_strings.h>
#include <stdint.h>
#include <string.h>

#if defined(PLATFORM_LINUX)
#elif defined(PLATFORM_ARDUINO)
#include <Arduino.h>
#else
#error "A PLATFORM_ Directive must be provided"
#endif

char *strcpy_itch_misc(char *dest, uint8_t src) {
	#ifdef ARDUINO
		return strcpy_P(dest, misc_itch_strings[src].text);
	#else
		return strcpy(dest, misc_itch_strings[src].text);
	#endif
}

char *strcat_itch_misc(char *dest, uint8_t src) {
	#ifdef ARDUINO
		return strcat_P(dest, misc_itch_strings[src].text);
	#else
		return strcat(dest, misc_itch_strings[src].text);
	#endif
}

char *strcpy_itch_hal(char *dest, const char *src) {
	#ifdef ARDUINO
		return strcpy_P(dest, src);
	#else
		return strcpy(dest, src);
	#endif
}

char *strcat_itch_hal(char *dest, const char *src) {
#ifdef ARDUINO
	return strcat_P(dest, src);
#else
	return strcat(dest, src);
#endif
}

void *memcpy_itch_hal(void *dest, const void *src, size_t sz) {
#ifdef ARDUINO
	return memcpy_P(dest, src, sz);
#else
	return memcpy(dest, src, sz);
#endif
}

#ifdef ITCH_DEBUG
char *strcpy_itch_debug(char *dest, uint8_t src) {
	#ifdef ARDUINO
		return strcpy_P(dest, itch_debug_strings[src].text);
	#else
		return strcpy(dest, itch_debug_strings[src].text);
	#endif
}

char *strcat_itch_debug(char *dest, uint8_t src) {
	#ifdef ARDUINO
		return strcat_P(dest, itch_debug_strings[src].text);
	#else
		return strcat(dest, itch_debug_strings[src].text);
	#endif
}

#endif //ITCH_DEBUG



