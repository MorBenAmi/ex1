#include <stdio.h>
#include "InputHandler.h"
#include "HammingUtil.h"
#include "OutputFileHandler.h"

#define WORD_COUNT 8

//Local variable
file_decorder decoded_file;

void decodeMessage(char *message)
{
	int current_position = 0;
	int i;
	char decodedMessage[CODE_DATA_SIZE + 1] = { '/0' };
	
	for (i = 0; i < WORD_COUNT; i++, current_position += CODE_WORD_SIZE)
	{
		//Correct errors
		if (hammingDecoder(current_position, message) > 0)
		{
			//Error was found
			decoded_file.corrected_counter++;
		}
		//Convert 63 bits into 57 bits
		removeCheckBits(message, decodedMessage, i);
	}

	decoded_file.wrote_counter += CODE_DATA_SIZE;
	decoded_file.received_counter += CODE_WORD_SIZE;

	decodedMessage[CODE_DATA_SIZE] = '\0';
	//Print decoded buffer to file	

	writeToFile(decodedMessage);	
}


file_decorder getDecodedFile()
{
	return decoded_file;
}
