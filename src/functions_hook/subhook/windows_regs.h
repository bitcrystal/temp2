#ifndef WINDOWS_REGS_H
#define WINDOWS_REGS_H
#include "my_predef.h"
#ifdef OS_WIN
#include <windows.h>
#define REG_KEY_OK(lRes) ((((lRes) == (ERROR_SUCCESS))?(1):(0)))
#define REG_KEY_NOT_OK(lRes) ((((lRes) != (ERROR_SUCCESS))?(1):(0)))
#define REG_KEY_FILE_NOT_FOUND(lRes) ((((lRes) == (ERROR_FILE_NOT_FOUND))?(1):(0)))
#define REG_KEY_FILE_FOUND(lRes) ((((lRes) != (ERROR_FILE_NOT_FOUND))?(1):(0)))
typedef struct _GET_STRING_REG_KEY_A_S
{
        CHAR strValue[512];
        CHAR strDefaultValue[512];
} GET_STRING_KEY_A_S;

typedef struct _GET_STRING_REG_KEY_W_S
{
        WCHAR strValue[512];
        WCHAR strDefaultValue[512];
} GET_STRING_KEY_W_S;
#ifndef UNICODE
typedef GET_STRING_KEY_A_S GET_STRING_KEY_T_S;
#else
typedef GET_STRING_KEY_W_S GET_STRING_KEY_T_S;
#endif
extern BOOL WINAPI EnableProcessPrivilege(LPCTSTR lpPrivilege, BOOL bEnable);
extern BOOL HAVE_PRIVILEGES();
extern LONG SaveRegKeyA(HKEY hKey, LPCSTR file);
extern LONG SaveRegKeyW(HKEY hKey, LPCWSTR file);
extern LONG SaveRegKey(HKEY hKey, LPCTSTR file);
extern LONG CreateRegKeyA(HKEY hKey, LPCSTR strKey,PHKEY handle,BOOL use64BitView);
extern LONG CreateRegKeyW(HKEY hKey, LPCWSTR strKey,PHKEY handle,BOOL use64BitView);
extern LONG CreateRegKey(HKEY hKey, LPCTSTR strKey,PHKEY handle,BOOL use64BitView);
extern LONG DeleteRegKeyA(HKEY hKey, LPCSTR strKey,BOOL use64BitView);
extern LONG DeleteRegKeyW(HKEY hKey, LPCWSTR strKey,BOOL use64BitView);
extern LONG DeleteRegKey(HKEY hKey, LPCTSTR strKey,BOOL use64BitView);
extern LONG SetRegKeyA(HKEY hKey, DWORD regType, LPCSTR keyName,LPCSTR keyValue);
extern LONG SetRegKeyW(HKEY hKey, DWORD regType, LPCWSTR keyName,LPCWSTR keyValue);
extern LONG SetRegKey(HKEY hKey, DWORD regType, LPCTSTR keyName, LPCTSTR keyValue);
extern LONG OpenRegKeyA(HKEY hKey, LPCSTR reg, PHKEY handle,BOOL use64BitView);
extern LONG OpenRegKeyW(HKEY hKey, LPCWSTR reg, PHKEY handle,BOOL use64BitView);
extern LONG OpenRegKey(HKEY hKey, LPCTSTR reg, PHKEY handle,BOOL use64BitView);
extern LONG CloseRegKeyA(HKEY hKey);
extern LONG CloseRegKeyW(HKEY hKey);
extern LONG CloseRegKey(HKEY hKey);
extern LONG OpenRegKey(HKEY hKey, LPCTSTR reg, PHKEY handle,BOOL use64BitView);
extern LONG GetDWORDRegKeyA(HKEY hKey, LPCSTR strValueName, DWORD * nValue, DWORD nDefaultValue);
extern LONG GetDWORDRegKeyW(HKEY hKey, LPCWSTR strValueName, DWORD * nValue, DWORD nDefaultValue);
extern LONG GetDWORDRegKey(HKEY hKey, LPCTSTR strValueName, DWORD * nValue, DWORD nDefaultValue);
extern LONG GetBoolRegKeyA(HKEY hKey, LPCSTR strValueName, BOOL * bValue, BOOL bDefaultValue);
extern LONG GetBoolRegKeyW(HKEY hKey, LPCWSTR strValueName, BOOL * bValue, BOOL bDefaultValue);
extern LONG GetBoolRegKey(HKEY hKey, LPCTSTR strValueName, BOOL * bValue, BOOL bDefaultValue);
extern LONG GetStringRegKeyA(HKEY hKey, LPCSTR strValueName, GET_STRING_KEY_A_S * k);
extern LONG GetStringRegKeyW(HKEY hKey, LPCWSTR strValueName, GET_STRING_KEY_W_S * k);
extern LONG GetStringRegKey(HKEY hKey, LPCTSTR strValueName, GET_STRING_KEY_T_S * k);
#endif
#endif
