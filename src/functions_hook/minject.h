#ifndef M_INJECT_H
#define M_INJECT_H
#include "subhook/my_predef.h"
#if defined(OS_MAC)
#define MY_COMMON MACH
#define MY_CCHAR const char
#define MY_CHAR char
#define MY_STR_COPY strcpy
#define MY_STR_LEN strlen
#define MY_STR_CMP strcmp
#include "mach_inject/mach_injector.h"
int MACH__Injector__uninject(MACH_INJECTOR_S_t mas);
#elif defined(OS_WIN)
#define MY_COMMON WINDOWS
#include "windows_inject/windows_injector.h"
#else
#define MY_COMMON LINUX
#include "linux_inject/linux_injector.h"
int LINUX__Injector__uninject(LINUX_INJECTOR_S_t mas);
#define MY_CCHAR const char
#define MY_CHAR char
#define MY_STR_COPY strcpy
#define MY_STR_LEN strlen
#define MY_STR_CMP strcmp
#endif
#endif