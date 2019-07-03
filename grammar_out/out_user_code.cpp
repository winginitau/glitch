#include <itch_config.h>
#include <out.h>

#if defined(PLATFORM_LINUX)
#elif defined(PLATFORM_ARDUINO)
#include <Arduino.h>
#else
#error "A PLATFORM_ Directive must be provided"
#endif
#include <itch.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// %code-start content begin
#include <my_funcs.h>
// %code-end content end

void SetLEDStateByNum(int16_t param1_int, uint16_t LED_COMMAND) {
	// >>>
	// >>> INSERT ACTION CODE HERE
	// >>> (Debug and call-through code may follow if specified in the grammar
	// >>>
	// >>>
	// >>> Results Callback via ITCH WriteLineCallback
	// >>>
	MyFuncSetLEDStateByNum(param1_int, LED_COMMAND, WriteLineCallback);
}

void SetLEDStateByName(char* LED_NAME, uint16_t LED_COMMAND) {
	// >>>
	// >>> INSERT ACTION CODE HERE
	// >>> (Debug and call-through code may follow if specified in the grammar
	// >>>
	// >>>
	// >>> Results Callback via ITCH WriteLineCallback
	// >>>
	MyFuncSetLEDStateByName(LED_NAME, LED_COMMAND, WriteLineCallback);
}

void LEDChase(int16_t param1_int, int16_t param2_int) {
	// >>>
	// >>> INSERT ACTION CODE HERE
	// >>> (Debug and call-through code may follow if specified in the grammar
	// >>>
	// >>>
	// >>> Results Callback via ITCH WriteLineCallback
	// >>>
	MyFuncLEDChase(param1_int, param2_int, WriteLineCallback);
}

void Say(char* param1_string) {
	// >>>
	// >>> INSERT ACTION CODE HERE
	// >>> (Debug and call-through code may follow if specified in the grammar
	// >>>
	// >>>
	// >>> Results Callback via ITCH WriteLineCallback
	// >>>
	MyFuncSay(param1_string, WriteLineCallback);
}

void SayWith(uint16_t SAY_HOW, char* param1_string) {
	// >>>
	// >>> INSERT ACTION CODE HERE
	// >>> (Debug and call-through code may follow if specified in the grammar
	// >>>
	// >>>
	// >>> Results Callback via ITCH WriteLineCallback
	// >>>
	MyFuncSayWith(SAY_HOW, param1_string, WriteLineCallback);
}

uint8_t LookupLEDName(char* lookup_string) {
	// Modify to suit. Default: call through to a function with %user-code-call-through
	// prefixed to the same name as this lookup function. Default "My"

	if (MyFuncLookupLEDName(lookup_string) != 0) return 1;
	return 0;
}

