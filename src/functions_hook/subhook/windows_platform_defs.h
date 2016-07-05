#ifndef WINDOWS_PLATFORM_DEFS_H
#define WINDOWS_PLATFORM_DEFS_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "my_predef.h"
#ifndef OS_WIN
#include "windows_defs2.h"
#else
#include <windows.h>
#endif
#ifdef __cplusplus
}
#endif
#endif
