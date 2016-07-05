#include "windows_regs.h"
#ifdef OS_WIN
BOOL WINAPI EnableProcessPrivilege(LPCTSTR lpPrivilege, BOOL bEnable) 
{
	typedef BOOL (WINAPI * fnptr_openprocesstoken)(HANDLE,DWORD,PHANDLE);
	typedef BOOL (WINAPI * fnptr_adjusttokenprivileges)(HANDLE,BOOL,PTOKEN_PRIVILEGES,DWORD,PTOKEN_PRIVILEGES,PDWORD);
	typedef BOOL (WINAPI * fnptr_lookupprivilegevalue)(LPCTSTR,LPCTSTR,PLUID);

	TOKEN_PRIVILEGES tp       = { 0 };
	HMODULE          hAdvapi  = NULL;
	HANDLE           hToken   = NULL;
	BOOL             bResult  = FALSE;
	fnptr_openprocesstoken      openprocesstoken;
	fnptr_adjusttokenprivileges adjusttokenprivileges;
	fnptr_lookupprivilegevalue  lookupprivilegevalue;

	/* Load the required functions */
	if (!(hAdvapi = LoadLibrary(TEXT("Advapi32.dll"))) ||
	    !(openprocesstoken      = (fnptr_openprocesstoken)      GetProcAddress(hAdvapi, "OpenProcessToken"))      ||
	    !(adjusttokenprivileges = (fnptr_adjusttokenprivileges) GetProcAddress(hAdvapi, "AdjustTokenPrivileges")) ||
#ifndef UNICODE
	    !(lookupprivilegevalue  = (fnptr_lookupprivilegevalue)  GetProcAddress(hAdvapi, "LookupPrivilegeValueA")))
#else
	    !(lookupprivilegevalue  = (fnptr_lookupprivilegevalue)  GetProcAddress(hAdvapi, "LookupPrivilegeValueW")))
#endif
	{
		goto cleanup;
	}

	/* Get the local id of our desired privilege */
	if (!lookupprivilegevalue(NULL, lpPrivilege, &tp.Privileges[0].Luid))
		goto cleanup;

	tp.PrivilegeCount           = 1;
	tp.Privileges[0].Attributes = (bEnable ? SE_PRIVILEGE_ENABLED : 0);

	/* Get the access token for the current process */
	if (!openprocesstoken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
		goto cleanup;

	/* Enable the privilege */
	if (!adjusttokenprivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
		goto cleanup;

	bResult = TRUE;

cleanup:
	if (hAdvapi) FreeLibrary(hAdvapi);
	if (hToken)  CloseHandle(hToken);
	return bResult;
}

BOOL HAVE_PRIVILGES()
{
	static BOOL HAVE_PRIVILEGE = FALSE;
	if(HAVE_PRIVILEGE==FALSE)
	{
		HAVE_PRIVILEGE = EnableProcessPrivilege(TEXT("SE_BACKUP_NAME"),TRUE);
	}
	return HAVE_PRIVILEGE;
}

LONG SaveRegKeyA(HKEY hKey, LPCSTR file)
{
	if(HAVE_PRIVILEGES()==FALSE)
		return ERROR_FILE_NOT_FOUND;
	LONG nError=RegSaveKeyExA(hKey,file,NULL,REG_LATEST_FORMAT);
	return nError;
}

LONG SaveRegKeyW(HKEY hKey, LPCWSTR file)
{
        if(HAVE_PRIVILEGES()==FALSE)
                return ERROR_FILE_NOT_FOUND;
        LONG nError=RegSaveKeyExW(hKey,file,NULL,REG_LATEST_FORMAT);
        return nError;
}

LONG SaveRegKey(HKEY hKey, LPCTSTR file)
{
#ifndef UNICODE
	return SaveRegKeyA(hKey,(LPCSTR)file);
#else
	return SaveRegKeyW(hKey,(LPCWSTR)file);
#endif
}


LONG CreateRegKeyA(HKEY hKey, LPCSTR strKey,PHKEY handle,BOOL use64BitView)
{
       DWORD access |= KEY_ALL_ACCESS;
       int u = (int)use64BitView;
       if(u==1)
                access|=KEY_WOW64_64KEY; 
       else if(u==2) 
                access|=KEY_WOW64_32KEY; 
	DWORD dwDisposition;
	LONG nError = RegCreateKeyExA(hKey, strKey, 0, NULL, 0, access, NULL, (PHKEY)handle,(PDWORD)&dwDisposition);
	return nError;
}

LONG CreateRegKeyW(HKEY hKey, LPCWSTR strKey,PHKEY handle,BOOL use64BitView)
{
	 DWORD access |= KEY_ALL_ACCESS;
       int u = (int)use64BitView;
       if(u==1)
                access|=KEY_WOW64_64KEY; 
       else if(u==2) 
                access|=KEY_WOW64_32KEY; 
        DWORD dwDisposition;
        LONG nError = RegCreateKeyExW(hKey, strKey, 0, NULL, 0, access, NULL, (PHKEY)handle,(PDWORD)&dwDisposition);
        return nError;
}

LONG CreateRegKey(HKEY hKey, LPCTSTR strKey,PHKEY handle,BOOL use64BitView)
{
#ifndef UNICODE
        return CreateRegKeyA(hKey,(LPCSTR)strKey,handle,use64BitView);
#else
        return CreateRegKeyW(hKey,(LPCWSTR)strKey,handle,use64BitView);
#endif
}

LONG DeleteRegKeyA(HKEY hKey,LPCSTR strKey,BOOL use64BitView)
{
       DWORD access = 0;
       int u = (int)use64BitView;
       if(u==1)
                access|=KEY_WOW64_64KEY; 
       else if(u==2) 
                access|=KEY_WOW64_32KEY; 

	LONG nError = RegDeleteKeyExA(hKey,strKey,access,0);
 	return nError;
}

LONG DeleteRegKeyW(HKEY hKey,LPCWSTR strKey,BOOL use64BitView)
{
       DWORD access = 0;
       int u = (int)use64BitView;
       if(u==1)
                access|=KEY_WOW64_64KEY; 
       else if(u==2) 
                access|=KEY_WOW64_32KEY; 

        LONG nError = RegDeleteKeyExW(hKey,strKey,access,0);
        return nError;
}


LONG DeleteRegKey(HKEY hKey, LPCTSTR strKey,BOOL use64BitView)
{
#ifndef UNICODE
        return DeleteRegKeyA(hKey,(LPCSTR)strKey,use64BitView);
#else
        return DeleteRegKeyW(hKey,(LPCWSTR)strKey,use64BitView);
#endif
}



LONG SetRegKeyA(HKEY hKey, DWORD regType, LPCSTR keyName,LPCSTR keyValue)
{
	DWORD keyValueSize=0;
	while(keyValue[keyValueSize]!=0)
	{
		keyValueSize++;
	}
	keyValueSize*=sizeof(CHAR);
	LONG nError = RegSetValueExA(hKey, keyName, 0, regType, (LPBYTE)keyValue, keyValueSize);
	return nError;
}

LONG SetRegKeyW(HKEY hKey, DWORD regType, LPCWSTR keyName,LPCWSTR keyValue)
{
        DWORD keyValueSize=0;
        while(keyValue[keyValueSize]!=0)
        {
                keyValueSize++;
        }
        keyValueSize*=sizeof(WCHAR);
        LONG nError = RegSetValueExW(hKey, keyName, 0, regType, (LPBYTE)keyValue, keyValueSize);
        return nError;
}

LONG SetRegKey(HKEY hKey, DWORD regType, LPCTSTR keyName, LPCTSTR keyValue)
{
#ifndef UNICODE
        return SetRegKeyA(hKey,regType,(LPCSTR)keyName,(LPCSTR)keyValue);
#else
        return SetRegKeyW(hKey,regType,(LPCWSTR)keyName,(LPCWSTR)keyValue);
#endif
}



LONG OpenRegKeyA(HKEY hKey,LPCSTR reg,PHKEY handle,BOOL use64BitView)
{
	DWORD access |= KEY_ALL_ACCESS;
       int u = (int)use64BitView;
       if(u==1)
                access|=KEY_WOW64_64KEY; 
       else if(u==2) 
                access|=KEY_WOW64_32KEY; 
	LONG ret = RegOpenKeyExA(hKey,reg,NULL,access,(PHKEY)handle);
	return ret;
}

LONG OpenRegKeyW(HKEY hKey,LPCWSTR reg,PHKEY handle,BOOL use64BitView)
{
       DWORD access |= KEY_ALL_ACCESS;
       int u = (int)use64BitView;
       if(u==1)
		access|=KEY_WOW64_64KEY;
       else if(u==2)
		access|=KEY_WOW64_32KEY; 
       LONG ret = RegOpenKeyExW(hKey,reg,NULL,access,(PHKEY)handle);
       return ret;
}

LONG OpenRegKey(HKEY hKey, LPCTSTR reg,PHKEY handle,BOOL use64BitView)
{
#ifndef UNICODE
        return OpenRegKeyA(hKey,(LPCSTR)reg,handle,use64BitView);
#else
        return OpenRegKeyW(hKey,(LPCWSTR)reg,handle,use64BitView);
#endif
}

LONG CloseRegKeyA(HKEY hKey)
{
	LONG nError = RegCloseKey(hKey);
	return nError;
}

LONG CloseRegKeyW(HKEY hKey)
{
	LONG nError = RegCloseKey(hKey);
	return nError;
}



LONG CloseRegKey(HKEY hKey)
{
#ifndef UNICODE
        return CloseRegKeyA(hKey);
#else
        return CloseRegKeyW(hKey);
#endif
}


LONG GetDWORDRegKeyA(HKEY hKey, LPCSTR strValueName, DWORD * nValue, DWORD nDefaultValue)
{
   *nValue = nDefaultValue;
    DWORD dwBufferSize = (DWORD)(sizeof(DWORD));
    DWORD nResult = 0;
    LONG nError = RegQueryValueExA(hKey,
        (LPCSTR)strValueName,
        (LPDWORD)NULL,
        (LPDWORD)NULL,
        ((LPBYTE)&nResult),
        ((LPDWORD)&dwBuffer));

    if (ERROR_SUCCESS == nError)
    {
        *nValue = nResult;
    }
    return nError;
}

LONG GetDWORDRegKeyW(HKEY hKey, LPCWSTR strValueName, DWORD * nValue, DWORD nDefaultValue)
{
   *nValue = nDefaultValue;
    DWORD dwBufferSize = (DWORD)(sizeof(DWORD));
    DWORD nResult = 0;
    LONG nError = RegQueryValueExW(hKey,
        (LPCWSTR)strValueName,
        (LPDWORD)NULL,
        (LPDWORD)NULL,
        ((LPBYTE)&nResult),
        ((LPDWORD)&dwBuffer));

    if (ERROR_SUCCESS == nError)
    {
        *nValue = nResult;
    }
    return nError;
}

LONG GetDWORDRegKey(HKEY hKey, LPCTSTR strValueName, DWORD * nValue, DWORD nDefaultValue)
{
#ifndef UNICODE
        return GetDWORDRegKeyA(hKey,(LPCSTR)strValueName,nValue,nDefaultValue);
#else
        return GetDWORDRegKeyW(hKey,(LPCWSTR)strValueName,nValue,nDefaultValue);
#endif
}



LONG GetBoolRegKeyA(HKEY hKey, LPCSTR strValueName, BOOL * bValue, BOOL bDefaultValue)
{
    DWORD nDefValue = (bDefaultValue==TRUE) ? (1) : (0);
    DWORD nResult = nDefValue;
    LONG nError = GetDWORDRegKeyA(hKey, strValueName, &nResult, nDefValue);
    if (ERROR_SUCCESS == nError)
    {
        *bValue = (nResult != 0) ? TRUE : FALSE;
    }
    return nError;
}


LONG GetBoolRegKeyW(HKEY hKey, LPCWSTR strValueName, BOOL * bValue, BOOL bDefaultValue)
{
    DWORD nDefValue = (bDefaultValue==TRUE) ? (1) : (0);
    DWORD nResult = nDefValue;
    LONG nError = GetDWORDRegKeyW(hKey, strValueName, &nResult, nDefValue);
    if (ERROR_SUCCESS == nError)
    {
        *bValue = (nResult != 0) ? TRUE : FALSE;
    }
    return nError;
}

LONG GetBoolRegKey(HKEY hKey, LPCTSTR strValueName, BOOL * bValue, BOOL bDefaultValue)
{
#ifndef UNICODE
        return GetBoolRegKeyA(hKey,(LPCSTR)strValueName,bValue,bDefaultValue);
#else
        return GetBoolRegKeyW(hKey,(LPCWSTR)strValueName,bValue,bDefaultValue);
#endif
}


LONG GetStringRegKeyA(HKEY hKey, LPCSTR strValueName, GET_STRING_KEY_A_S * k)
{
   if(k==NULL)
	return ERROR_FILE_NOT_FOUND;
    memcpy((void*)&k->strValue[0],(void*)&k->strDefaultValue[0],sizeof(GET_STRING_KEY_A_S)/2);
    CHAR szBuffer[512];
    DWORD dwBufferSize = sizeof(szBuffer);
    LONG nError = RegQueryValueExA(hKey,
        (LPCSTR)strValueName,
        (LPDWORD)NULL,
        (LPDWORD)NULL,
        ((LPBYTE)&szBuffer[0]),
        ((LPDWORD)&dwBufferSize));
    if (ERROR_SUCCESS == nError)
    {
        memcpy((void*)&k->strValue[0],(void*)&szBuffer[0],sizeof(GET_STRING_KEY_A_S)/2);
    }
    return nError;
}

LONG GetStringRegKeyW(HKEY hKey, LPCWSTR strValueName, GET_STRING_KEY_W_S * k)
{
   if(k==NULL)
	return ERROR_FILE_NOT_FOUND;
    memcpy((void*)&k->strValue[0],(void*)&k->strDefaultValue[0],sizeof(GET_STRING_KEY_W_S)/2);
    WCHAR szBuffer[512];
    DWORD dwBufferSize = sizeof(szBuffer);
    LONG nError = RegQueryValueExW(hKey,
        (LPCWSTR)strValueName,
        (LPDWORD)NULL,
        (LPDWORD)NULL,
        ((LPBYTE)&szBuffer[0]),
        ((LPDWORD)&dwBufferSize));
    if (ERROR_SUCCESS == nError)
    {
        memcpy((void*)&k->strValue[0],(void*)&szBuffer[0],sizeof(GET_STRING_KEY_W_S)/2);
    }
    return nError;
}

LONG GetStringRegKey(HKEY hKey, LPCTSTR strValueName, GET_STRING_KEY_T_S * k)
{
#ifndef UNICODE
        return GetStringRegKeyA(hKey,(LPCSTR)strValueName,(GET_STRING_KEY_A_S*)k);
#else
        return GetStringRegKeyW(hKey,(LPCWSTR)strValueName,(GET_STRING_KEY_W_S*)k);
#endif
}

#endif
