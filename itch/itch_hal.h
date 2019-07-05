/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: itch_hal.h
 - Abstraction for string literal C string operations
 - Platform dependent - on Arduino string literals are in PROGMEM
 *******************************************************************/

#ifndef ITCH_HAL_H_
#define ITCH_HAL_H_

#include <stdint.h>

#if defined(PLATFORM_LINUX)
#elif defined(PLATFORM_ARDUINO)
#include <Arduino.h>
#else
#error "A PLATFORM_ Directive must be provided"
#endif

char *strcpy_itch_misc(char *dest, uint8_t src);
char *strcat_itch_misc(char *dest, uint8_t src);
char *strcpy_itch_hal(char *dest, const char *src);
char *strcat_itch_hal(char *dest, const char *src);
void *memcpy_itch_hal(void *dest, const void *src, size_t sz);

#ifdef ITCH_DEBUG
char *strcpy_itch_debug(char *dest, uint8_t src);
char *strcat_itch_debug(char *dest, uint8_t src);
#endif


#endif /* ITCH_HAL_H_ */
