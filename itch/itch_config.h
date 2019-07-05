/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: itch_config.h
 - Some configurable items presently hand crafted.
 - TODO: PLATFORM directive to be replaced by build system targets
******************************************************************/

#ifndef ITCH_CONFIG_H_
#define ITCH_CONFIG_H_

// Define the target platform
#if not defined PLATFORM_LINUX && not defined PLATFORM_ARDUINO
#define PLATFORM_ARDUINO
// OR
//#define PLATFORM_LINUX
#endif // Not Defined Either

#define PROG_NAME "itch - Interactive Terminal Command sHell"

//#define ITCH_DEBUG

#ifdef MAX_LABEL_LENGTH						// As defined in src/build_config.h
#define MAX_TOKEN_SIZE MAX_LABEL_LENGTH		// MAX_TOKEN_SIZE only used in itch parser
#else
#define MAX_TOKEN_SIZE 48
#endif

#endif /* ITCH_CONFIG_H_ */
