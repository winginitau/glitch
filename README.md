**GLITCH/ITCH**

Grammar, Lexer, Interactive Terminal Command sHell

It serves multiple purposes (for which no equivalent singular or multi solution could be found that was fit for size and purpose). They are:

- Simple definition and extension of an arbitrary grammar that can be used to interactively communicate with, command and control an Arduino.
- Supports basic terminal emulation and command help, coupled with definable enums and string arrays.
- Allow extension and maintenance of base firmware code and configuration schemas to support new functions, device drivers, operational data etc from a single source &quot;master&quot; meta config (if such functionality is relevant to the project)

It has successfully been decoupled from its originating project and is now an independent, (hopefully) useful tool for other projects that need enum and string list management and an interactive command / protocol shell for Arduinos and other embedded devices.

Features include:

- Simple definition language for the creation of any arbitrary token-based command and control grammar that is Left-Right parse-able and does not need lookahead.
- Definition and management of enum and associated string arrays used in the grammar (or required by firmware generally).
- On a linux box, GLITCH compiles the definition and produces a C header and source files which are the core of the parser.
- ITCH is a combination of the generated parser and the terminal emulation code.
- Easily included into any Arduino project without needing fundamental change to the project design – a couple of #includes, Begin() in setup, and Poll() in the loop.
- Successfully parsed commands call through to the project main code to carry out actions.
- The terminal component provides: simple line editing; last command recall; minimum unique command abbreviation (eg &quot;show sensors&quot; could be abbreviated to &quot;s s&quot; if there are no other keywords starting with &quot;s&quot; at each level); integrated help – &quot; ?&quot; displays next available keywords and data types in the grammar.
- Static RAM efficient making extensive use of PROGMEM and other flash segments.
- Total static RAM usage is 221 bytes (of which 150 bytes is the default configurable input buffer)
- Example grammar and project to illustrate usage

Known limitations:

- High PROGMEM usage – currently includes extensive error strings that could be trimmed back significantly. (~30k for a useful grammar with a few enum string arrays, most of which is error strings).
- Only tested on an Arduino mega2560
- Not yet packaged as an Arduino library
- Linux components are made using an Eclipse CDT managed build environment
- Arduino components rely on the Sloeber Arduino Eclipse plugin to manage Arduino libraries and the AVR tool chain.
- Build directives are hand crafted in a header file (rather than being target based using an external make/build utility).
- Glitch usage documentation to be done
- Windows port / build to be done and tested – currently the glitch portion has only been built and tested on linux

See grammar\_out/ for example grammars and the resultant outputs from running example\_grammar.gf through glitch.

See itch\_arduino/itch\_arduino.ino for a self documenting example program.

Released under an MIT license – see LICEENSE.TXT.
