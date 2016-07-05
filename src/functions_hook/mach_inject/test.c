#include "windows_inject.h"
#include "windows_inject.c"
//#include "bootstrap_dylib.c"

int main()
{
	file_to_char_pointer("bootstrap.c","file_bootstrap.c");
	//char_buffer_to_file(lib_memory(),9376,4000,"bootstrap.test.dylib");
}
