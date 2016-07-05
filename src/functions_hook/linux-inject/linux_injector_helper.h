#ifndef LINUX_INJECTOR_HELPER_H
#define LINUX_INJECTOR_HELPER_H

#include "../subhook/my_predef.h"

#ifndef ARM
        #if defined(MY_CPU_ARM) || defined(MY_CPU_ARM64)
                #define ARM
        #endif
#endif

#ifndef X86
	#if defined(MY_CPU_X86)
                #define X86
        #endif
#endif

#ifndef X86_64
      #if defined(MY_CPU_X86_64)
      	#define X86_64
      #endif
#endif
#ifdef X86_64
	#ifndef X64
		#define X64
	#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/user.h>
#include <wait.h>

#include "utils.h"
#include "ptrace.h"

void injectSharedLibrary(long mallocaddr, long freeaddr, long dlopenaddr);
void injectSharedLibrary_end();
#endif
