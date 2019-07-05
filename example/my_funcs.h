/******************************************************************
 Example program illustrating basic Arduino usage of itch

 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt
 See also README.txt

 File: my_funcs.h
 - Illustrative header that maps action and lookup calls from
   itch into the main project code
 - Is included by the parser (defined in the grammar definition)
   to provide the integration
******************************************************************/

#ifndef MY_FUNCS_H_
#define MY_FUNCS_H_

// Place functions that itch will call on action events into
// a code and header file that gets included by itch - this file.

// Standard integer specifiers used throughout to be precise on memory usage
#include <stdint.h>

// Define anything else that makes sense for your program and grammar
// - These could be included in the grammar file too!
// - Included here to illustrate %lookup-list functionality from itch

struct MY_LEDS {
	const char *led_name;
	uint8_t pin_num;
};

// Action functions declarations called "through" by itch:
// - Are called when the parser parses an input arriving at an %action directive
// - Are all present in out_user_code.cpp (though its risky to maintain your code there)
// - If %user-code-call-through is specified, the out_user_code.cpp functions are
//   written by glitch to call through to your code (ie. here) with a name prefix
// - Same base name as the %action-define function names in the grammar
// - Prepended with "My" (default) or the string defined by %user-code-call-through-prefix
// - In this case "MyFunc" is specified in the grammar
// - The function parameters are the same as in out_user_code.cpp
//	- The out_user_code.cpp function called through to here  if %user-code-call-through
//	  is specified. This is the most useful mode.
//	- Parameter list is defined by any non-keyword grammar directives "up" the
//	  lexical tree from the calling %action
//	- Needs to include the callback function pointer which has the form:
//	  void WriteLineCallback(const char* string);
//	- If your C code is not up to void/function pointers just copy and paste from here

void MyFuncSetLEDStateByNum(int16_t param1_int, uint16_t LED_COMMAND, void(*Callback)(const char*));
void MyFuncSetLEDStateByName(char* LED_NAME, uint16_t LED_COMMAND, void(*Callback)(const char*));
void MyFuncSay(char* param1_string, void(*Callback)(const char*));
void MyFuncSayWith(uint16_t SAY_HOW, char* param1_string, void(*Callback)(const char*));
void MyFuncLEDChase(int16_t param1_int, int16_t param2_int, void(*Callback)(const char*));

// String lookup functions called by itch:
//	- The directive %lookup-list LED_NAME LookupLEDName tells itch to call your
//	  code for an answer on the validity of a string entered for parsing
//	- In the example grammar "%2 lookup LED_NAME" gives the lexical context
//  - The code therefore needs to return a true (1) or false (0) answer
//	- In this example case: is the lookup_string a valid LEDName?

uint8_t MyFuncLookupLEDName(char *lookup_string);

#endif /* MY_FUNCS_H_ */
