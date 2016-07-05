#ifndef MACH_INJECTOR_H
#define MACH_INJECTOR_H
#include "mach_injector_settings.h"

#ifndef MACH_INJECT_OLD_METHOD
#include "task_vaccine.h"
#else
#include "mach_inject.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <mach/mach.h>
#include <mach/thread_act.h>
#include <mach/mach_init.h>
#include <pthread.h>
#include <dlfcn.h>
#include <signal.h>
#include <sys/proc_info.h>
#include <libproc.h>
#ifdef __cplusplus
	extern "C" {
#endif

typedef struct MACH_INJECTOR_S_
{
#ifndef MACH_INJECT_OLD_METHOD
	task_t task;
#else
	pid_t task;
	void *module;
	void *bootstrapfn;
#endif
	char path[4096];
	char is_installed:4;
	char is_injected:4;
	char is_done;
	char is_init;
	char is_deinit;
} MACH_INJECTOR_S,*MACH_INJECTOR_S_t;

MACH_INJECTOR_S_t MACH_INJECTOR_S_t_new();

int MACH_INJECTOR_S_t_delete(MACH_INJECTOR_S_t mas);

void MACH__Injector__init(MACH_INJECTOR_S_t mas);

void MACH__Injector__deinit(MACH_INJECTOR_S_t mas);

void MACH__Injector__reset(MACH_INJECTOR_S_t mas);

int MACH__Injector__inject__pid(MACH_INJECTOR_S_t mas, pid_t pid, const char* lib);

int MACH__Injector__inject__processname(MACH_INJECTOR_S_t mas,const char * processname,const char * lib);

int MACH__Injector__inject__task(MACH_INJECTOR_S_t mas,
#ifndef MACH_INJECT_OLD_METHOD
const task_t task,
#else
const pid_t task,
#endif
const char * lib);

int MACH__Injector__inject(MACH_INJECTOR_S_t mas);

int MACH__getTaskByProcessName(const char * name,task_t*t);

int MACH__getTask(pid_t proc,task_t*t);

pid_t MACH__Injector__getProcessByName(const char *name);

#ifdef __cplusplus
	}
#endif

#endif
