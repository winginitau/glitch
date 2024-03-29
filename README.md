# GLITCH/ITCH

### Grammar, Lexer, Interactive Terminal Command sHell

- Terminal / command host for inclusion in Arduino and embedded projects.
- Simple definition of an arbitrary command grammar.
- Interactive command and control with basic terminal emulation.
- "Plug in" design - does not require fundamental project code changes.
- Command help generated automatically from the command grammar.
- Management of enums and associated string arrays - for use in code and the grammar.
- Helps extension and maintenance of firmware code and configuration schemas by providing a single source, "master" meta config (if such functionality is relevant to the project).
 - glitch compiles the grammar and produces out.h and associated files.
 - itch.h is the parser and terminal - gets included with out.h in embedded projects.

Successfully decoupled from its originating project. Its now an independent tool for use in other projects that need an interactive command / protocol shell and/or enum, string list management.

### Features include:

- Simple definition language for the creation of any arbitrary token-based command and control grammar that is Left-Right parse-able and does not need lookahead.
- Definition and management of enum and associated string arrays used in the grammar (or required by firmware generally).
- On a linux box, GLITCH compiles the definition and produces a C header and source files which are the grammar dependent portions of the parser.
- ITCH is a combination of the compiled grammar, the base parser code and the terminal host code.
- Easily included into any Arduino project without needing fundamental change to the project design – a couple of #includes, Begin() in setup, and Poll() in the loop.
- Successfully parsed commands call through to the project main code to carry out actions.
- The terminal component provides: 
  - Simple line editing
  - Last command recall
  - Minimum unique command abbreviation (eg &quot;show sensors&quot; could be abbreviated to &quot;s s&quot; if there are no other keywords starting with &quot;s&quot; at each level)
  - Integrated help – &quot; ?&quot; displays next available keywords and data types in the grammar.
  - Switchable modes using definable "escape" sequences:
    - Data / passthrough (terminal and parser are silent) - start up mode
    - Text interactive (example program uses "^^^" to enter this mode - eg similar to Hayes AT "+++")
    - Text protocol - no echo, "OK" and "ERROR" responses only (example uses "%%%" to enter this mode)
    - CTRL-D to exit back to data/passthrough.
- Static RAM efficient making extensive use of PROGMEM and other flash segments.
- Total static RAM usage is 221 bytes (of which 150 bytes is the default configurable input buffer)
- Includes an example grammar and project to illustrate usage

### Overview diagram is [here](Overview.png).

### [Example Project Recipe](USAGE.md)

### Known limitations:

- High PROGMEM usage – currently includes extensive error strings that could be trimmed back significantly. (~20k for a useful grammar with a few enum string arrays, most of which is error strings).
- Line editing and help functions don't work nicely with line bufferred terminals (eg Arduino IDE serial moniotr). It is a char by char parser to support various interactive features and to reduce Poll() loop time.
- Designed to be used with terminal programs without local echo and that are able to translate '\n' into crlf.
  - Try: picocom --imap lfcrlf -b 9600 /dev/ttyACM0
- Data / passthrough does not yet provide read() functions to yield the data / buffer to the main program. 
- Only tested on an Arduino mega2560
- Linux components are made using an Eclipse CDT managed build environment - needs work on a proper makefile
- Build directives are presenly hand crafted in a header file (rather than being target based using an external make/build utility). though defaults should be fine.
- Glitch usage documentation still evolving
- Line editing currently only supports \<backspace\>, last command recall \<up-arrow\>, and " ?" help. \<left\> \<right\> \<del\> to be added.
- Windows port / build to be done and tested – currently the glitch portion has only been built and tested on linux

### Grammar links:
 - [example_grammar.gf](grammar/example_grammar.gf) for a simple grammar used in the example project.
 - [This](grammar/grammar.gf) is a more sophisticated grammar used in the [FodderFactory](https://github.com/winginitau/FodderFactory)

### An example self documenting program is included:
 - [example.ino](example/example.ino)
 - [my_funcs.h](example/my_funcs.h)
 - [my_funcs.cpp](example/my_funcs.cpp)

Released under an MIT license – [see LICEENSE](LICENSE)
