#include <stdio.h>
#include "InputHandler.h"
#include "HammingUtil.h"
#include "OutputFileHandler.h"

#define WORD_COUNT 8

void decodeMessage(char *message)
{
	int current_position = 0;
	int i;
	char decodedMessage[CODE_DATA_SIZE + 1] = { '/0' };
	
	for (i = 0; i < WORD_COUNT; i++, current_position += CODE_WORD_SIZE)
	{
		//Correct errors
		hammingDecoder(current_position, message); 
		//Convert 63 bits into 57 bits
		removeCheckBits(message, decodedMessage, i);
	}

	decodedMessage[CODE_DATA_SIZE] = '\0';
	//Print decoded buffer to file	

	writeToFile(decodedMessage);	
}