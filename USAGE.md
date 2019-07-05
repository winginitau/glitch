*GLITCH/ITCH Usage Example*

This example works under linux.

Prerequisites:
 - git
 - c/c++ build environment (developed with gcc)
 - The Arduino IDE (or an IDE and AVR toolchain of your choice if you know what you're doing)

Major steps include:
 - Clone and compile the package for linux
 - Use glitch to process an example grammar
 - Install itch as an Arduino library
 - Set up the example project 
 - Open the project in the Arduino IDE
 - Build it, upload it and play with it 

Clone the github repository and compile glitch. This will place the binary glitch_linux in the top glitch directory:
```bash
git clone https://github.com/winginitau/glitch.git
cd glitch
make
```
A quick tour of the project structure:
 - glitch/glitch - glitch sources and build files
 - glitch/glitch_linux - the compiled binary once 'make' has been used
 - glitch/itch - the core parser to be installed as an Arduino library
 - glitch/grammar/example_grammar.gf - a simple grammar LED grammar for the example project
 - glitch/grammar/grammar.gf - a more elaborate grammar used on a fodder growing control system
 - glitch/example/* - the example arduino project used herein
 - glitch/example/my_funcs.* - self documenting example of itch integration 
 
Compile the example grammar. This will place 3x out* files in the glitch top - to be later incorporated into the Arduino project / library.
```bash
./glitch_linux -g grammar/example_grammar.gf
```
Ideally, the different parts of this project could be managed separatley. However, the sketch, its source files, the compiled grammar, and the itch library need to find each other to allow "call-throughs" and callbacks. 

With a sophisticated IDE / environment this can be accomplished with source locations and include paths. However for the Arduino IDE, the simplist (though not very pretty) solution is to lump together everything together into an Arduino library - except the example.ino file.

Assuming that Arduino sketches are kept in $HOME/Arduino:
```bash
mkdir $HOME/Arduino/example
mkdir $HOME/Arduino/libraries/ITCH
cp example/example.ino $HOME/Arduino/example/
cp example/my_funcs.* $HOME/Arduino/libraries/ITCH/
cp out* $HOME/Arduino/libraries/ITCH/
cp itch/* $HOME/Arduino/libraries/ITCH/
```
Finally, open the Arduino IDE. 
```
File/Open: Arduino/example/example.ino
Verify
Upload
```





 




