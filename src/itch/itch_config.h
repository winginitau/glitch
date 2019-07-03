/*
 * itch_config.h
 *
 *  Created on: 18 Mar. 2018
 *      Author: brendan
 */

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
