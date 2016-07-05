#include "file_bootstrap.c"
#include "bootstrap_compile.h"
#include "windows_inject.h"
#include "windows_inject.c"
#include <stdio.h>
#include <stdlib.h>

int bootstrap_exists()
{
	FILE * open = fopen("bootstrap.dylib","rb");
        if(open)
        {
                fclose(open);
                return 1;
        }
	return 0;
}

void delete_bootstrap()
{
	system("rm bootstrap.dylib");
}

int delete_bootstrap_if_exists()
{
	if(bootstrap_exists())
	{
		delete_bootstrap();
		return 1;
	}
	return 0;
}

int bootstrap_compile()
{
	char * bootstrap_file = file_bootstrap_1503();
	char_buffer_to_file(bootstrap_file,1503,1503,"bootstrap__.c");
	system("gcc -c bootstrap__.c -o bootstrap__.o");
	system("gcc -dynamiclib -undefined suppress -flat_namespace bootstrap__.o -o bootstrap.dylib");	
}

int main()
{
	bootstrap_compile();
}
