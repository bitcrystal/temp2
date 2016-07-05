#include "testdylib_tar_gz.c"
#include "testdylib_compile.h"
#include "windows_inject.h"
#include "windows_inject.c"
#include <stdio.h>
#include <stdlib.h>

int testdylib_exists()
{
	FILE * open = fopen("testdylib.dylib","rb");
        if(open)
        {
                fclose(open);
                return 1;
        }
	return 0;
}

void delete_testdylib()
{
	system("rm testdylib.dylib");
}

int delete_testdylib_if_exists()
{
	if(testdylib_exists())
	{
		delete_testdylib();
		return 1;
	}
	return 0;
}

int testdylib_compile()
{
	char * testdylib_file = file_testdylib_tar_gz_213525();
	system("mkdir -p nf");
	system("cd nf");
	system("rm testdylib.tar.gz 2> /dev/null 1> /dev/null");
	system("rm *.o 2> /dev/null 1> /dev/null");
	char_buffer_to_file(testdylib_file,4096,4096,"testdylib.tar.gz");
	system("tar xvzf testdylib.tar.gz");
	system("gcc -c testdylib.c -o testdylib.o");
	system("gcc -dynamiclib -undefined suppress -flat_namespace testdylib.o -o libtestdylib.dylib");	
}

int main()
{
	testdylib_compile();
}
