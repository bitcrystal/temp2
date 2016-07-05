#ifndef WINDOWS_INJECT_H
#define WINDOWS_INJECT_H
#include "my_predef.h"
extern int inject_libs(const char * libraryPath, int is_x64_library);
extern int inject_libs_portable(const char * libraryPath);
extern char * get_current_directory_os();
extern char get_current_os_seperator();
char * _uint_to_c_string(unsigned long long x,unsigned int * string_size);
char * _string_to_c_string(const char * sstring,unsigned int * sstring_size);
int file_to_char_pointer(const char * file, const char * copyto);
#endif
