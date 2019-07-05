################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AST.cpp \
../ASTNode.cpp \
../Debug.cpp \
../Identifier.cpp \
../Identifiers.cpp \
../KeyValuePairList.cpp \
../Lexer.cpp \
../LineBuffer.cpp \
../OutputBuffer.cpp \
../StringList.cpp \
../glitch.cpp 

OBJS += \
./AST.o \
./ASTNode.o \
./Debug.o \
./Identifier.o \
./Identifiers.o \
./KeyValuePairList.o \
./Lexer.o \
./LineBuffer.o \
./OutputBuffer.o \
./StringList.o \
./glitch.o 

CPP_DEPS += \
./AST.d \
./ASTNode.d \
./Debug.d \
./Identifier.d \
./Identifiers.d \
./KeyValuePairList.d \
./Lexer.d \
./LineBuffer.d \
./OutputBuffer.d \
./StringList.d \
./glitch.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/brendan/git/glitch/glitch" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


