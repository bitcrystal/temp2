#ifndef BOOTSTRAP_COMPILE_H
#define BOOTSTRAP_COMPILE_H
#ifdef __cplusplus
	extern "C" {
#endif
int bootstrap_exists();
void delete_bootstrap();
int delete_bootstrap_if_exists();
int bootstrap_compile();
#ifdef __cplusplus 
	}
#endif
#endif
