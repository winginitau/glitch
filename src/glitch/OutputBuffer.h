/*****************************************************************
 OutputBuffer.h

 Copyright (C) 2018 Brendan McLearie 

 Created on: 16 Feb. 2018

 ******************************************************************/

#ifndef OUTPUTBUFFER_H_
#define OUTPUTBUFFER_H_

#include <StringList.h>
//#include <string.h>

class OutputBuffer: public StringList {
public:
	OutputBuffer();
	virtual ~OutputBuffer();

	bool OutputAvailable();
	char* GetOutputAsString(char* output_str);
	void SetOutputAvailable();
private:
    bool output_available;
    //char output_string[MAX_BUFFER_LENGTH];

};


#endif /* OUTPUTBUFFER_H_ */
