#ifndef LINUX_INJECTOR_H
#define LINUX_INJECTOR_H

#include "linux_injector_helper.h"

//#ifdef __cplusplus
//	extern "C" {
//#endif

typedef struct LINUX_INJECTOR_S_
{
	pid_t pid;
	int libPathLength;

	int mypid;
	long mylibcaddr;

	// find the addresses of the syscalls that we'd like to use inside the
	// target, as loaded inside THIS process (i.e. NOT the target process)
	long mallocAddr;
	long freeAddr;
	long dlopenAddr;
#ifdef ARM
	long raiseAddr;
#endif
	// use the base address of libc to calculate offsets for the syscalls
	// we want to use
	long mallocOffset;
	long freeOffset;
	long dlopenOffset;
#ifdef ARM
	long raiseOffset;
#endif
	// get the target process' libc address and use it to find the
	// addresses of the syscalls we want to use inside the target process
	long targetLibcAddr;
	long targetMallocAddr;
	long targetFreeAddr;
	long targetDlopenAddr;
#ifdef ARM
	long targetRaiseAddr;
#endif
	char path[4096];
	char libname[4096];
	char is_installed:4;
	char is_injected:4;
	char is_done;
	char is_init;
	char is_deinit;
} LINUX_INJECTOR_S,*LINUX_INJECTOR_S_t;

LINUX_INJECTOR_S_t LINUX_INJECTOR_S_t_new();

int LINUX_INJECTOR_S_t_delete(LINUX_INJECTOR_S_t mas);

void LINUX__Injector__init(LINUX_INJECTOR_S_t mas);

void LINUX__Injector__deinit(LINUX_INJECTOR_S_t mas);

void LINUX__Injector__reset(LINUX_INJECTOR_S_t mas);

int LINUX__Injector__inject__pid(LINUX_INJECTOR_S_t mas, pid_t pid, const char* lib);

int LINUX__Injector__inject__processname(LINUX_INJECTOR_S_t mas,const char * processname,const char * lib);

int LINUX__Injector__inject(LINUX_INJECTOR_S_t mas);

pid_t LINUX__Injector__getProcessByName(const char *name);

//#ifdef __cplusplus
//	}
//#endif

#endif
