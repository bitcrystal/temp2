#include "windows_inject.h"

#ifdef OS_WIN

#ifndef INJECT_METHOD_1
#define INJECT_METHOD_1
#endif

#ifdef INJECT_METHOD_1
HMODULE k_Injector__TryInjectDllA(DWORD adw_ProcessId, const char * as_DllFile) 
{ 
    //Find the address of the LoadLibrary api, luckily for us, it is loaded in the same address for every process 
    HMODULE hLocKernel32 = GetModuleHandleA("KERNEL32"); 
    FARPROC hLocLoadLibrary = GetProcAddressA(hLocKernel32, "LoadLibraryA"); 
     
    //Adjust token privileges to open system processes 
    HANDLE hToken; 
    TOKEN_PRIVILEGES tkp; 
    if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
    { 
        LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid); 
        tkp.PrivilegeCount = 1; 
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
        AdjustTokenPrivileges(hToken, 0, &tkp, sizeof(tkp), NULL, NULL); 
        CloseHandle(hToken); 
    } 

    //Open the process with all access 
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, adw_ProcessId); 
    if (hProc == NULL) 
        return NULL; 

    //Allocate memory to hold the path to the Dll File in the process's memory 
    LPVOID hRemoteMem = VirtualAllocEx(hProc, NULL, strlen(as_DllFile)*sizeof(char), MEM_COMMIT, PAGE_READWRITE); 

    //Write the path to the Dll File in the location just created 
    DWORD numBytesWritten; 
    WriteProcessMemory(hProc, hRemoteMem, as_DllFile, strlen(as_DllFile)*sizeof(char), &numBytesWritten); 

    //Create a remote thread that starts begins at the LoadLibrary function and is passed are memory pointer 
    HANDLE hRemoteThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)hLocLoadLibrary, hRemoteMem, 0, NULL); 

    //Wait for the thread to finish 
    WaitForSingleObject( hRemoteThread, INFINITE ); 
    DWORD  hLibModule = 0; 
    GetExitCodeThread( hRemoteThread, &hLibModule ); 

    //Free the memory created on the other process 
    VirtualFreeEx(hProc, hRemoteMem, strlen(as_DllFile)*sizeof(char), MEM_RELEASE); 

    //Release the handle to the other process 
    CloseHandle(hProc); 

    return (HMODULE)hLibModule; 
} 

HMODULE k_Injector__TryInjectDllW(DWORD adw_ProcessId, const wchar_t * as_DllFile) 
{ 
    //Find the address of the LoadLibrary api, luckily for us, it is loaded in the same address for every process 
    HMODULE hLocKernel32 = GetModuleHandleW(L"KERNEL32"); 
    FARPROC hLocLoadLibrary = GetProcAddressW(hLocKernel32, "LoadLibraryW"); 
     
    //Adjust token privileges to open system processes 
    HANDLE hToken; 
    TOKEN_PRIVILEGES tkp; 
    if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
    { 
        LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid); 
        tkp.PrivilegeCount = 1; 
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
        AdjustTokenPrivileges(hToken, 0, &tkp, sizeof(tkp), NULL, NULL); 
        CloseHandle(hToken); 
    } 

    //Open the process with all access 
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, adw_ProcessId); 
    if (hProc == NULL) 
        return NULL; 

    //Allocate memory to hold the path to the Dll File in the process's memory 
    LPVOID hRemoteMem = VirtualAllocEx(hProc, NULL, wcslen(as_DllFile), MEM_COMMIT, PAGE_READWRITE); 

    //Write the path to the Dll File in the location just created 
    DWORD numBytesWritten; 
    WriteProcessMemory(hProc, hRemoteMem, as_DllFile, wcslen(as_DllFile), &numBytesWritten); 

    //Create a remote thread that starts begins at the LoadLibrary function and is passed are memory pointer 
    HANDLE hRemoteThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)hLocLoadLibrary, hRemoteMem, 0, NULL); 

    //Wait for the thread to finish 
    WaitForSingleObject( hRemoteThread, INFINITE ); 
    DWORD  hLibModule = 0; 
    GetExitCodeThread( hRemoteThread, &hLibModule ); 

    //Free the memory created on the other process 
    VirtualFreeEx(hProc, hRemoteMem, wcslen(as_DllFile), MEM_RELEASE); 

    //Release the handle to the other process 
    CloseHandle(hProc); 

    return (HMODULE)hLibModule; 
} 

BOOL k_Injector__TryUnInjectDllW(DWORD adw_ProcessId, HMODULE ah_ModuleHandle) 
{ 
    //Open the process with all access 
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, adw_ProcessId); 
    if (hProc == NULL) 
        return FALSE; 

    BOOL lb_ReturnValue = FALSE; 

    HMODULE hLocKernel32 = GetModuleHandleW(L"KERNEL32"); 
    FARPROC hLocLoadLibrary = GetProcAddressW(hLocKernel32, L"FreeLibrary"); 

    if(ah_ModuleHandle != NULL) 
    { 
        HANDLE hRemoteThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)hLocLoadLibrary, (void*)ah_ModuleHandle, 0, NULL );

        if( hRemoteThread != NULL ) 
        { 
            DWORD ldw_ReturnCode; 
            WaitForSingleObject( hRemoteThread, INFINITE ); 
            GetExitCodeThread( hRemoteThread, &ldw_ReturnCode ); 
            CloseHandle( hRemoteThread ); 

            lb_ReturnValue = ldw_ReturnCode != 0 ? TRUE : FALSE; 
        } 
    } 

    CloseHandle(hProc); 

    return lb_ReturnValue; 
}

BOOL k_Injector__TryUnInjectDllA(DWORD adw_ProcessId, HMODULE ah_ModuleHandle) 
{ 
    //Open the process with all access 
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, adw_ProcessId); 
    if (hProc == NULL) 
        return FALSE; 

    BOOL lb_ReturnValue = FALSE; 

    HMODULE hLocKernel32 = GetModuleHandleA("KERNEL32"); 
    FARPROC hLocLoadLibrary = GetProcAddressA(hLocKernel32, "FreeLibrary"); 

    if(ah_ModuleHandle != NULL) 
    { 
        HANDLE hRemoteThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)hLocLoadLibrary, (void*)ah_ModuleHandle, 0, NULL );

        if( hRemoteThread != NULL ) 
        { 
            DWORD ldw_ReturnCode; 
            WaitForSingleObject( hRemoteThread, INFINITE ); 
            GetExitCodeThread( hRemoteThread, &ldw_ReturnCode ); 
            CloseHandle( hRemoteThread ); 

            lb_ReturnValue = ldw_ReturnCode != 0 ? TRUE : FALSE; 
        } 
    } 

    CloseHandle(hProc); 

    return lb_ReturnValue; 
}

#elif defined(INJECT_METHOD_2)
/*BOOL AdjustDacl(HANDLE h, DWORD DesiredAccess) 
{ 
    SID world = { SID_REVISION, 1, SECURITY_WORLD_SID_AUTHORITY, 0 }; 
     
    EXPLICIT_ACCESS ea = 
    { 
        DesiredAccess, SET_ACCESS, NO_INHERITANCE, 
        { 
            0, NO_MULTIPLE_TRUSTEE, TRUSTEE_IS_SID, TRUSTEE_IS_USER, 
            reinterpret_cast<LPTSTR>(&world) 
        } 
    }; 

    ACL* pdacl = 0; 
    DWORD err = SetEntriesInAcl(1, &ea, 0, &pdacl); 
    if(err == ERROR_SUCCESS) 
    { 
        err = SetSecurityInfo(h, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, pdacl, 0); 
        LocalFree(pdacl); 
        return(err == ERROR_SUCCESS); 
    } 
    else 
        return FALSE; 
} 

BOOL EnableTokenPrivilege(HANDLE htok, LPCTSTR szPrivilege,    TOKEN_PRIVILEGES& tpOld) 
{ 
    TOKEN_PRIVILEGES tp; 
    tp.PrivilegeCount = 1; 
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
    if(LookupPrivilegeValue(0, szPrivilege, &tp.Privileges[0].Luid)) 
    { 
        DWORD cbOld = sizeof tpOld; 
        if (AdjustTokenPrivileges(htok,    FALSE, &tp, cbOld, &tpOld, &cbOld)) 
            return(ERROR_NOT_ALL_ASSIGNED != GetLastError()); 
        else return FALSE; 
    } 
    else 
        return FALSE; 
} 


BOOL RestoreTokenPrivilege(HANDLE htok, const TOKEN_PRIVILEGES&    tpOld) 
{ 
    return(AdjustTokenPrivileges(htok, FALSE, const_cast<TOKEN_PRIVILEGES*>(&tpOld), 0,    0, 0)); 
} 

HANDLE GetProcessHandleWithEnoughRights(DWORD PID, DWORD AccessRights) 
{ 
    HANDLE hProcess = ::OpenProcess(AccessRights, FALSE, PID); 
    if(hProcess == NULL) 
    { 
        HANDLE hpWriteDAC = OpenProcess(WRITE_DAC, FALSE, PID); 
        if(hpWriteDAC == NULL) 
        { 
            HANDLE htok; 
            if(!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &htok)) 
                return(FALSE); 
             
            TOKEN_PRIVILEGES tpOld; 
            if (EnableTokenPrivilege(htok, SE_TAKE_OWNERSHIP_NAME, tpOld)) 
            { 
                HANDLE hpWriteOwner = OpenProcess(WRITE_OWNER, FALSE, PID); 
                if (hpWriteOwner != NULL) 
                { 
                    BYTE buf[512]; 
                    DWORD cb = sizeof buf; 
                    if (GetTokenInformation(htok, TokenUser, buf, cb, &cb)) 
                    { 
                        DWORD err = SetSecurityInfo(hpWriteOwner, SE_KERNEL_OBJECT, OWNER_SECURITY_INFORMATION, reinterpret_cast<TOKEN_USER*>(buf)->User.Sid, 0, 0, 0); 
                        if(err == ERROR_SUCCESS) 
                        { 
                            if (!DuplicateHandle(GetCurrentProcess(), hpWriteOwner, GetCurrentProcess(), &hpWriteDAC, WRITE_DAC, FALSE, 0) )
                                hpWriteDAC = NULL; 
                        } 
                    } 
                    CloseHandle(hpWriteOwner); 
                } 
                RestoreTokenPrivilege(htok,    tpOld); 
            } 
            CloseHandle(htok); 
        } 
         
        if(hpWriteDAC) 
        { 
            AdjustDacl(hpWriteDAC, AccessRights); 

            if(!DuplicateHandle( GetCurrentProcess(), hpWriteDAC, GetCurrentProcess(), &hProcess, AccessRights, FALSE, 0)) 
                hProcess = NULL; 
            CloseHandle(hpWriteDAC); 
        } 
    } 
    return hProcess; 
} 

BOOL WINAPI InjectLibW(DWORD dwProcessId, PCWSTR pszLibFile) 
{ 
    BOOL fOk = FALSE; 
    HANDLE hProcess = NULL, hThread = NULL; 
    PWSTR pszLibFileRemote = NULL; 

    hProcess = GetProcessHandleWithEnoughRights(dwProcessId, 
        PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD | 
        PROCESS_VM_OPERATION | PROCESS_VM_WRITE); 

    if(hProcess == NULL) 
        return FALSE; 
     
    int cch = 1 + lstrlenW(pszLibFile); 
    int cb = cch * sizeof(WCHAR); 
     
    pszLibFileRemote = (PWSTR)VirtualAllocEx(hProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE); 
     
    if(pszLibFileRemote != NULL) 
    { 
        if(WriteProcessMemory(hProcess, pszLibFileRemote, (PVOID)pszLibFile, cb, NULL)) 
        { 
            PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE) 
                GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW"); 
            if(pfnThreadRtn != NULL) 
            { 
                hThread = CreateRemoteThread(hProcess, NULL, 0, pfnThreadRtn, pszLibFileRemote, 0, NULL); 

                if(hThread != NULL) 
                { 
                    WaitForSingleObject(hThread, INFINITE); 
                    fOk = TRUE; 
                    CloseHandle(hThread); 
                } 
            } 
        } 
        VirtualFreeEx(hProcess, pszLibFileRemote, 0, MEM_RELEASE); 
    } 
    CloseHandle(hProcess); 
    return fOk; 
} 

BOOL WINAPI InjectLibA(DWORD dwProcessId, PCSTR    pszLibFile) 
{ 
    PWSTR pszLibFileW = (PWSTR) _alloca((lstrlenA(pszLibFile) + 1) * sizeof(WCHAR)); 
    wsprintfW(pszLibFileW, L"%S", pszLibFile); 
    return(InjectLibW(dwProcessId, pszLibFileW)); 
} 


int CDECL main(int argc, char **argv) 
{ 
    INT nProcessId = -1; 

    STARTUPINFO si; 
    PROCESS_INFORMATION pi; 

    ZeroMemory( &si, sizeof(si) ); 
    si.cb = sizeof(si); 
    ZeroMemory( &pi, sizeof(pi) ); 

    std::string AppFile, AppPath; 
    AppFile = ... 
    AppPath = ... 
    if(FALSE==CreateProcess(AppFile.c_str(),NULL,NULL,NULL,FALSE,CREATE_SUSPENDED,NULL,AppPath.c_str(),&si,&pi)) 
    { 
        return -1; 
    } 
     
     
    if (!InjectLibA(pi.dwProcessId,DllPath.c_str()))  
    { 
        return -1; 
    } 

    if(0xFFFFFFFF==ResumeThread(pi.hThread)) 
    { 
        return -1; 
    } 

    // Wait until child process exits. 
    WaitForSingleObject( pi.hProcess, INFINITE ); 

    // Close process and thread handles.  
    CloseHandle( pi.hProcess ); 
    CloseHandle( pi.hThread ); 

    return 0; 
}
*/
#elif defined(INJECT_METHOD_3)
/*
void k_Application__StartInjection(const std::wstring& as_Dll) 
{ 
    HMODULE lh_Module = ::LoadLibraryW(as_Dll.c_str()); 
    if (lh_Module != NULL) 
    { 
        typeBaseFunc l_InstallHook_ = (typeBaseFunc)::GetProcAddress(lh_Module, "InstallHook"); 
        if (l_InstallHook_ != NULL) 
            l_InstallHook_(); 
    } 
} 


void k_Application__EndInjection(const std::wstring& as_Dll) 
{ 
    HMODULE lh_Module = ::LoadLibraryW(as_Dll.c_str()); 
    if (lh_Module != NULL) 
    { 
        typeBaseFunc l_UnInstallHook_ = (typeBaseFunc)::GetProcAddress(lh_Module, "RemoveHook"); 
        if (l_UnInstallHook_ != NULL) 
            l_UnInstallHook_(); 
    } 
}
*/
/*
HHOOK gh_GlobalHook = NULL; 

static LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)  
{ 
    CWPSTRUCT *lr_WndProc_ = reinterpret_cast<CWPSTRUCT *>(lParam); 
    if (lr_WndProc_)  
    { 
        switch (lr_WndProc_->message)  
        { 
            ... 
        } 
    } 
    return CallNextHookEx(gh_GlobalHook, nCode, wParam, lParam); 
} 


extern "C" __declspec(dllexport) void __cdecl RemoveHook() 
{ 
    if (gh_GlobalHook != NULL) 
    { 
        UnhookWindowsHookEx(gh_GlobalHook); 
        gh_GlobalHook = NULL; 
    } 
} 


extern "C" __declspec(dllexport) void __cdecl InstallHook() 
{ 
    HMODULE lh_Self = NULL; 
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (wchar_t *) &InstallHook, &lh_Self);
    if (lh_Self == NULL)  
    { 
        
    }  
    else  
    { 
        gh_GlobalHook = ::SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, lh_Self, 0); 
        if (gh_GlobalHook == NULL) 
        { 
             
        } 
    } 
}*/
#endif
