#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include "OutputFileHandler.h"

static FILE* output_file;
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
//the file containe all the functions that responsable for wting to the output file// 
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
void openFile(char* output_path)
{
	DWORD last_error = 0;
	if (output_file != NULL)
		return;
	last_error = GetLastError();
	output_file = fopen(output_path, "w+");
	if (output_file == NULL)
	{
		printf("Unable to open output file\n");
		exit(GetLastError());
	}
	SetLastError(last_error);
}

void closeFile()
{
	if (output_file == NULL)
		return;
	fclose(output_file);
	output_file = NULL;
}

void writeToFile(const char* message)
{
	if (output_file == NULL)
		return;

	fputs(message, output_file);
}



