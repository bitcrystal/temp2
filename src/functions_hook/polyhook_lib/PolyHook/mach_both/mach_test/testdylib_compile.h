#ifndef TESTDYLIB_COMPILE_H
#define TESTDYLIB_COMPILE_H
#ifdef __cplusplus
	extern "C" {
#endif
int testdylib_exists();
void delete_testdylib();
int delete_testdylib_if_exists();
int testdylib_compile();
#ifdef __cplusplus 
	}
#endif
#endif
