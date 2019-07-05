/******************************************************************
 Example program illustrating basic Arduino usage of glitch / itch

 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt
 See also README.txt

 File: itch_arduino.ino
 - Example Arduino program
******************************************************************/

#include "Arduino.h"
#include <stdio.h>

// Include the itch header file
#include <itch.h>

// Globally declare an itch singleton
// Note: It's possible to use it as a local, though since it is designed
// 	to be inserted between your program code and all "console" / Serial
//  communications it is likely to be instantiated regularly. On resource
//  constrained devices this means a hit to the stack when ever it comes
//  into scope (particularly the input buffer). Better to declare it global
//  so that it is allocated in .bss (if on Arduino) and its buffer usage
//  is visible at build time.
ITCH itch;

void setup() {
	// Call Begin() passing the HardwareSerial to use, and the speed
	itch.Begin(Serial, 9600);
}

void loop() {
	// Do some work

	// Call Poll() in the loop
	itch.Poll();
}


