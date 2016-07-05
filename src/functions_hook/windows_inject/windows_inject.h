#ifndef WINDOWS_INJECT_H
#define WINDOWS_INJECT_H
#include "../subhook/my_predef.h"
#ifdef OS_WIN
#include <windows.h>
#ifndef INJECT_METHOD_1
#define INJECT_METHOD_1
#endif
#ifdef INJECT_METHOD_1
HMODULE k_Injector__TryInjectDllA(DWORD adw_ProcessId, const char * as_DllFile);
HMODULE k_Injector__TryInjectDllW(DWORD adw_ProcessId, const wchar_t * as_DllFile);
BOOL k_Injector__TryUnInjectDllW(DWORD adw_ProcessId, HMODULE ah_ModuleHandle);
BOOL k_Injector__TryUnInjectDllA(DWORD adw_ProcessId, HMODULE ah_ModuleHandle);
#elif defined(INJECT_METHOD_2)
#elif defined(INJECT_METHOD_3)
#endif
#endif
