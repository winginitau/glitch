# Simple makefile to build glitch for linux and place the binary in the project root folder

# All Target
all: glitch

glitch: 
	cd glitch/Release && $(MAKE) all
	cp glitch/Release/glitch glitch_linux

# Other Targets
clean:
	cd glitch/Release && $(MAKE) clean
	rm glitch_linux
	rm out.h
	rm out.cpp
	rm out_user_code.cpp

.PHONY: all glitch clean 

