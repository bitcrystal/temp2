#include "windows_inject.h"
#include "windows_inject.c"
#include <stdio.h>

int main(int argc, char ** argv)
{
	if(argc < 3)
	{
		printf("Usage:    %s <path/to/file> <path/to/file>\n,",argv[0]);
		printf("Example:  %s ./sample.c ./sample_binary_.c\n",argv[0]);
		return 0; 
	}
	int allok = file_to_char_pointer(argv[1],argv[2]);
	if(allok)
	{
		printf("%s file successfully created!\n",argv[2]);
	} else {
		printf("%s file can not be created!\n",argv[2]);
	}
}
