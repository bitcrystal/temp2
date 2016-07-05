#ifndef WINDOWS_INJECT_H
#define WINDOWS_INJECT_H
#include "../subhook/my_predef.h"
#ifdef OS_WIN
#include <windows.h>
#include <tlhelp32.h>
#ifndef INJECT_METHOD_1
#define INJECT_METHOD_1
#endif
#ifdef INJECT_METHOD_1
void EnableDebugPriv();
#if defined(UNICODE) || defined(_UNICODE)
HMODULE k_Injector__TryInjectDllW(DWORD adw_ProcessId, const wchar_t * as_DllFile);
BOOL k_Injector__TryUnInjectDllW(DWORD adw_ProcessId, HMODULE ah_ModuleHandle);
DWORD GetProcessIdByNameW(const wchar_t * processName); // non-conflicting function name
HANDLE GetProcessHandleByNameW(const wchar_t * processname);
#define GET_PROCESS_ID_BY_NAME GetProcessIdByNameW
#define GET_PROCESS_HANDLE_BY_NAME GetProcessHandleByNameW
#define MY_INJECT_DLL k_Injector__TryInjectDllW
#define MY_UNINJECT_DLL k_Injector__TryUnInjectDllW
#define MY_CHAR wchar_t
#define MY_CCHAR const wchar_t
#define MY_STR_LEN wcslen
#define MY_STR_COPY wcscpy
#define MY_STR_CMP wcscmp
#else
HMODULE k_Injector__TryInjectDllA(DWORD adw_ProcessId, const char * as_DllFile);
BOOL k_Injector__TryUnInjectDllA(DWORD adw_ProcessId, HMODULE ah_ModuleHandle);
DWORD GetProcessIdByNameA(const char * processName); // non-conflicting function name
HANDLE GetProcessHandleByNameA(const char * processname);
#define Process32FirstA Process32First
#define Process32NextA Process32Next
#define GET_PROCESS_ID_BY_NAME GetProcessIdByNameA
#define GET_PROCESS_HANDLE_BY_NAME GetProcessHandleByNameA
#define PROCESSENTRY32A PROCESSENTRY32
#define MY_INJECT_DLL k_Injector__TryInjectDllA
#define MY_UNINJECT_DLL k_Injector__TryUnInjectDllA
#define MY_CHAR char
#define MY_CCHAR const char
#define MY_STR_LEN strlen
#define MY_STR_COPY strcpy
#define MY_STR_CMP strcmp
#endif
#elif defined(INJECT_METHOD_2)
#elif defined(INJECT_METHOD_3)
#endif
#endif
#endif