#ifndef WINDOWS_INJECTOR_H
#define WINDOWS_INJECTOR_H

#include "windows_inject.h"

#ifdef __cplusplus
	extern "C" {
#endif

#ifndef pid_t
#define pid_t DWORD
#endif

typedef struct WINDOWS_INJECTOR_S_
{
	pid_t pid;
	HMODULE module;
	MY_CHAR path[4096];
	char is_installed:4;
	char is_injected:4;
	char is_done;
	char is_init;
	char is_deinit;
} WINDOWS_INJECTOR_S,*WINDOWS_INJECTOR_S_t;

WINDOWS_INJECTOR_S_t WINDOWS_INJECTOR_S_t_new();

int WINDOWS_INJECTOR_S_t_delete(WINDOWS_INJECTOR_S_t mas);

void WINDOWS__Injector__init(WINDOWS_INJECTOR_S_t mas);

void WINDOWS__Injector__deinit(WINDOWS_INJECTOR_S_t mas);

void WINDOWS__Injector__reset(WINDOWS_INJECTOR_S_t mas);

int WINDOWS__Injector__inject__pid(WINDOWS_INJECTOR_S_t mas, pid_t pid, MY_CCHAR * lib);

int WINDOWS__Injector__inject__processname(WINDOWS_INJECTOR_S_t mas,MY_CCHAR * processname,MY_CCHAR * lib);

int WINDOWS__Injector__inject(WINDOWS_INJECTOR_S_t mas);

int WINDOWS__Injector__uninject(WINDOWS_INJECTOR_S_t mas);

pid_t WINDOWS__Injector__getProcessByName(MY_CCHAR *name);

#ifdef __cplusplus
	}
#endif

#endif
