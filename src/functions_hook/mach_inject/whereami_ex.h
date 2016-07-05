#ifndef WHEREAMI_EX_H
#define WHEREAMI_EX_H

#ifdef __cplusplus
	extern "C" {
#endif

char * getFullExecutablePath();
char * getFullModulePath();
char * getExecutablePath(char lastchar);
char * getModulePath(char lastchar);

#ifdef __cplusplus
	}
#endif

#endif
