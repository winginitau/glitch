/******************************************************************
 glitch - Grammar Lexer and Interactive Terminal Command sHell

 Copyright 2018, 2019, Brendan McLearie
 Distributed under MIT license - see LICENSE.txt

 See README.md

 File: glitch_config.h
 - Configurable compile time parameters
********************************************************************/

#ifndef GLITCH_CONFIG_H_
#define GLITCH_CONFIG_H_

#define VERSION "0.1a"
#define PROG_NAME "glitch - Grammar Lexer and Interactive Terminal Command sHell"

/******************************************************************
	USER CONFIGURABLE PARAMETERS
******************************************************************/

// These should all move to command line options in a future version
// They all relate to the final contents of the user code file (default: out_user_code.cpp)

// Moved to config  //#define USER_CODE_HINTS	// Include explanation in user code
// Moved to config  //#define USER_CODE_DEBUG	// Insert a debug output into each user function
// Moved to config  //#define USER_CODE_EXTERNAL_CALL "Reg"
// TODO Move following comments to doc
	// If defined call a function provided by the user
	// of the same name as the user_code_out function names
	// with this string prepended to the function name
	// passing all args plus the callback function pointer.
	// Useful during grammar development, don't have to worry
	// about any additional user code being overwritten. Put
	// all the real action code elsewhere.
// Moved to config  //#define USER_CODE_EXTERNAL_CALL_COMMENTED
	// Write the external calls but comment them out

// Default prefix if defined in the grammar by no prefix specified
#define USER_CODE_CALLTHROUGH_PREFIX "My"

/******************************************************************
	Don't change these unless you are deep diving
******************************************************************/

//#define DEBUG
//#define STDOUT_DEBUG

#define IDENTIFIER_REGEX "^[a-zA-Z0-9_]{1,31}$"

// TODO: This should move to a build directive
#define PLATFORM_LINUX

// Common to itch and glitch. Presently duplicated
// to make build easy for packaging. TODO: move to
// common directory and sort out build scripting.
#include <common_config.h>

// Defining these sorts of sizes is important in
// embedded systems, not so much in a linux host. Silly
// sizes set to allow plenty of space, however warnings
// are thrown when sprintf'ing strings together if the
// input buffers could overflow the target.
// TODO: revisit approach / C vc C++ use etc for
//  strings in glitch.

#ifndef MAX_BUFFER_LENGTH
#define MAX_BUFFER_LENGTH 300
#endif

#define MAX_TEMP_BUFFER_LENGTH 300
#define MAX_OUT_BUFFER_LENGTH 600

#ifndef MAX_BUFFER_WORD_LENGTH
#define MAX_BUFFER_WORD_LENGTH 200
#endif

#ifndef MAX_BUFFER_WORDS_PER_LINE
#define MAX_BUFFER_WORDS_PER_LINE 50
#endif

#define MAX_IDENTIFIERS 300

#ifndef MAX_LABEL_LENGTH
#define MAX_LABEL_LENGTH 200
#endif

typedef struct ENUM_STRING_ARRAY_TYPE {
	// Array of strings relating to enums
	char text[MAX_BUFFER_LENGTH];
} EnumStringArray;

#endif /* GLITCH_CONFIG_H_ */
