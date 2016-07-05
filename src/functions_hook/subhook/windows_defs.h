#ifndef WINDOWS_DEFS_H
#define WINDOWS_DEFS_H
#include "my_predef.h"
#ifndef OS_WIN
typedef void * PVOID;
typedef int BOOL;
#ifndef FALSE
#define FALSE               0
#endif
#ifndef TRUE
#define TRUE                1
#endif
#ifdef _M_IX86
#define _M_IX86_X64
#elif defined _M_X64
#define _M_IX86_X64
#endif
//#define NULL ( (void *) 0)
#ifndef NULL
#define NULL 0
#endif
#ifndef __int64
#define __int64 long long
#endif
#ifndef CONST
#define CONST const
#endif
typedef CONST void *LPCVOID;
typedef void *LPVOID;
typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef char CHAR;
typedef char CCHAR;
typedef long LONG;
typedef LONG *PLONG;
typedef DWORD COLORREF;
typedef BYTE BOOLEAN;
typedef unsigned __int64 DWORDLONG;
#if defined(_WIN64)
 typedef __int64 LONG_PTR; 
#else
 typedef long LONG_PTR;
#endif
typedef LONG_PTR LPARAM;
#if defined(_WIN64)
 typedef unsigned __int64 ULONG_PTR;
#else
 typedef unsigned long ULONG_PTR;
#endif
typedef ULONG_PTR SIZE_T;
typedef ULONG_PTR DWORD_PTR;
typedef unsigned __int64 DWORD64, *PDWORD64;
#if !defined(_M_IX86)
 typedef unsigned __int64 ULONGLONG;
#else
 typedef double ULONGLONG;
#endif
#ifndef __SIZE_TYPE__
#define __SIZE_TYPE__ ULONG_PTR
#endif
typedef __SIZE_TYPE__ size_t;
typedef SIZE_T size_t_in_windows;
typedef unsigned int ULONG32;
typedef unsigned long _ULONG32;
typedef unsigned __int64 ULONG64;
typedef long _LONG32;
typedef signed int LONG32;
typedef __int64 LONG64;
typedef short SHORT;
typedef unsigned short USHORT;
typedef CHAR *LPSTR;
typedef CONST CHAR *PCSTR;
typedef CHAR *PSTR;
typedef USHORT WCHAR;
typedef WCHAR *PWSTR;
typedef WCHAR *LPWSTR;
typedef DWORD *PDWORD;
typedef DWORD *LPDWORD;
typedef PVOID HANDLE;
typedef HANDLE *LPHANDLE;
#ifndef _UNICODE
#ifdef UNICODE 
#define _UNICODE UNICODE
#endif
#else
#ifndef UNICODE
#define UNICODE _UNICODE
#endif
#endif

#ifdef UNICODE
 typedef WCHAR TBYTE;
#else
 typedef unsigned char TBYTE;
#endif

#ifdef UNICODE
 typedef LPWSTR PTSTR;
#else
typedef LPSTR PTSTR;
#endif
#ifdef UNICODE
 typedef LPWSTR LPTSTR;
#else
 typedef LPSTR LPTSTR;
#endif
typedef struct _UNICODE_STRING {
  USHORT  Length;
  USHORT  MaximumLength;
  PWSTR  Buffer;
} UNICODE_STRING;
typedef CONST CHAR *LPCSTR;
typedef CONST WCHAR *LPCWSTR;
#ifdef UNICODE
 typedef LPCWSTR LPCTSTR; 
#else
 typedef LPCSTR LPCTSTR;
#endif
#ifndef MAX_PATH
#define MAX_PATH             260
#endif
#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE ((HANDLE) -1)
#endif
#ifndef INVALID_FILE_SIZE
#define INVALID_FILE_SIZE    ((DWORD)0xFFFFFFFF)
#endif
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;
#ifdef _UNICODE
#define L(str) L##str
#else
#define L(str) str
#endif
#ifndef __stdcall
#define __stdcall
#endif
#ifndef CALLBACK
#define CALLBACK __stdcall
#endif
#ifndef WINAPI
#define WINAPI __stdcall
#endif
typedef struct _MEMORY_BASIC_INFORMATION {
  PVOID  BaseAddress;
  PVOID  AllocationBase;
  DWORD  AllocationProtect;
  SIZE_T RegionSize;
  DWORD  State;
  DWORD  Protect;
  DWORD  Type;
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;
typedef union ____Win32Helper___NUM_BASE
{
	char value_char[4];
	unsigned char u_value_char[4];
	_LONG32 value : 32;
	_ULONG32 u_value : 32;
} ___Win32Helper___NUM_BASE;
typedef ___Win32Helper___NUM_BASE MY_NUM_BASE;

typedef union ____Win32Helper___NUM64_BASE
{
	char value_char[8];
	unsigned char u_value_char[8];
	LONG64 value : 64;
	ULONG64 u_value : 64;
} ___Win32Helper___NUM64_BASE;
typedef ___Win32Helper___NUM64_BASE MY_NUM64_BASE;
#ifndef CO_MODE
#if defined(OS_UNIX_STRUCT)
#define CO_MODE
#endif
#endif
#ifdef CO_MODE
/*#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>*/
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "linux_defs.h"
#if defined(ENOMEM) && !defined(EFAULT)
#define EFAULT ENOMEM
#elif defined(EFAULT) && !defined(ENOMEM)
#define ENOMEM EFAULT
#endif
#ifndef MAP_ANON
#ifdef MAP_ANONYMOUS
#define MAP_ANON MAP_ANONYMOUS
#endif
#endif
#ifndef MAP_ANONYMOUS
#ifdef MAP_ANON
#define MAP_ANONYMOUS MAP_ANON
#endif
#endif

#endif

#ifndef MY__PROT_READ

#if defined(VM_PROT_READ)
#define MY__PROT_READ VM_PROT_READ
#elif defined(B_READ_AREA)
#define MY__PROT_READ B_READ_AREA
#elif defined(MA_READ)
#define MY__PROT_READ MA_READ
#elif defined(PROT_READ)
#define MY__PROT_READ PROT_READ
#else
#define MY__PROT_READ 1
#endif

#endif

#ifndef MY__PROT_WRITE

#if defined(VM_PROT_WRITE)
#define MY__PROT_WRITE VM_PROT_WRITE
#elif defined(B_WRITE_AREA)
#define MY__PROT_WRITE B_WRITE_AREA
#elif defined(MA_WRITE)
#define MY__PROT_WRITE MA_WRITE
#elif defined(PROT_WRITE)
#define MY__PROT_WRITE PROT_WRITE
#else
#define MY__PROT_WRITE 2
#endif
#endif

#ifndef MY__PROT_EXEC

#if defined(VM_PROT_EXECUTE)
#define MY__PROT_EXEC VM_PROT_EXECUTE
#elif defined(B_WRITE_AREA)
#define MY__PROT_WRITE B_WRITE_AREA
#elif defined(MA_EXEC)
#define MY__PROT_EXEC MA_EXEC
#elif defined(PROT_EXEC)
#define MY__PROT_EXEC PROT_EXEC
#else
#define MY__PROT_EXEC 4
#endif

#endif

#ifndef MY__PROT_NONE

#if defined(VM_PROT_NONE)
#define MY__PROT_NONE VM_PROT_NONE
#elif defined(B_NONE_AREA)
#define MY__PROT_NONE B_NONE_AREA
#elif defined(MA_NONE)
#define MY__PROT_NONE MA_NONE
#elif defined(PROT_NONE)
#define MY__PROT_NONE PROT_NONE
#else
#define MY__PROT_NONE 0
#endif

#endif

#ifndef PROT_NONE
#define PROT_NONE MY__PROT_NONE
#endif
#ifndef PROT_READ
#define PROT_READ MY__PROT_READ
#endif
#ifndef PROT_WRITE
#define PROT_WRITE MY__PROT_WRITE
#endif
#ifndef PROT_EXEC
#define PROT_EXEC MY__PROT_EXEC
#endif

#define MY_PAGE_READONLY PROT_READ
#define MY_PAGE_READWRITE (PROT_READ | PROT_WRITE)
#define MY_PAGE_EXECUTE PROT_EXEC
#define MY_PAGE_EXECUTE_READ (PROT_READ | PROT_EXEC)
#define MY_PAGE_EXECUTE_READWRITE (PROT_READ | PROT_WRITE | PROT_EXEC)
#define MY_PAGE_EXECUTE_WRITECOPY (PROT_READ | PROT_WRITE | PROT_EXEC)
#define MY_PAGE_NOACCESS PROT_NONE
//
// Page protection
//

#ifndef PAGESIZE
#define PAGESIZE  4096
#endif
#ifndef PAGE_NOACCESS
#define PAGE_NOACCESS 0x01
#endif
#ifndef PAGE_READONLY
#define PAGE_READONLY 0x02
#endif
#ifndef PAGE_READWRITE
#define PAGE_READWRITE 0x04
#endif
#ifndef PAGE_WRITECOPY
#define PAGE_WRITECOPY 0x08
#endif
#ifndef PAGE_EXECUTE
#define PAGE_EXECUTE 0x10
#endif
#ifndef PAGE_EXECUTE_READ
#define PAGE_EXECUTE_READ 0x20
#endif
#ifndef PAGE_EXECUTE_READWRITE
#define PAGE_EXECUTE_READWRITE 0x40
#endif
#ifndef PAGE_EXECUTE_WRITECOPY
#define PAGE_EXECUTE_WRITECOPY 0x80
#endif
#ifndef PAGE_GUARD
#define PAGE_GUARD 0x100
#endif
#ifndef PAGE_NOCACHE
#define PAGE_NOCACHE 0x200
#endif
#ifndef PAGE_WRITECOMBINE
#define PAGE_WRITECOMBINE 0x400
#endif
#ifndef MEM_COMMIT
#define MEM_COMMIT 0x1000
#endif
#ifndef MEM_RESERVE
#define MEM_RESERVE 0x2000
#endif
#ifndef MEM_DECOMMIT
#define MEM_DECOMMIT 0x4000
#endif
#ifndef MEM_RELEASE
#define MEM_RELEASE 0x8000
#endif
#ifndef MEM_FREE
#define MEM_FREE 0x10000
#endif
#ifndef MEM_PRIVATE
#define MEM_PRIVATE 0x20000
#endif
#ifndef MEM_MAPPED
#define MEM_MAPPED 0x40000
#endif
#ifndef MEM_RESET
#define MEM_RESET 0x80000
#endif
#ifndef MEM_TOP_DOWN
#define MEM_TOP_DOWN 0x100000
#endif
#ifndef MEM_LARGE_PAGES
#define MEM_LARGE_PAGES 0x20000000
#endif
#ifndef MEM_ALIGN64K
#define MEM_ALIGN64K            0x10000000
#endif
#ifndef MEM_4MB_PAGES
#define MEM_4MB_PAGES 0x80000000
#endif
typedef struct fpi_s_
{
        unsigned long long address;
        unsigned long long size;
        unsigned char is_free:4;
	unsigned char is_readed:4;	
} fpi_s;
extern BOOL WINAPI VirtualProtect(LPVOID lpAddress,SIZE_T dwSize,DWORD flNewProtect,PDWORD lpflOldProtect);
extern SIZE_T WINAPI VirtualQuery(LPCVOID lpAddress,PMEMORY_BASIC_INFORMATION lpBuffer,SIZE_T dwLength);
extern LPVOID WINAPI VirtualAlloc(LPVOID lpAddress,SIZE_T dwSize,DWORD flAllocationType,DWORD flProtect);
extern BOOL WINAPI VirtualFree(LPVOID lpAddress,SIZE_T dwSize,DWORD dwFreeType);
extern BOOL WINAPI FlushInstructionCache(HANDLE hProcess,LPCVOID lpBaseAddress,SIZE_T dwSize);
extern HANDLE WINAPI GetCurrentProcess();
#else
#include <windows.h>
#endif
#define _LONG_PTR(a) ((LONG_PTR)((a)))
#define _ULONG_PTR(a) ((ULONG_PTR)((a)))
typedef struct _vma_it_func
{
	unsigned long long start_address;
	unsigned long long end_address;
	unsigned long long page_size;
	unsigned long long page_alignment_size;
    unsigned long long free_pages;
    unsigned long long free_pages_size;
    unsigned long long reserved_pages_size;
    unsigned long long reserved_pages;
	unsigned long long size;
	unsigned long long base_start_address;
	unsigned long long base_end_address;
	unsigned int base_flags;
	unsigned char ret;
	unsigned char complete_free_region;
	//unsigned long long rest;
	//unsigned long long last_rest;
} vma_it_func;
int vma_iterate_func(void *data,unsigned long long start, unsigned long long end,unsigned int flags);
int vma_iterate_full_addressing_func(void *data,unsigned long long start, unsigned long long end,unsigned int flags);
#if defined(OS_UNIX_STRUCT) && defined(USE_UNIX_IMAGE_EXTENSION_VQ)
int vma_iterate_image_func(void *data,unsigned long long start, unsigned long long end,unsigned int flags);
#endif
unsigned long long MY_GET_SYSTEM_PAGE_SIZE();
unsigned long long MY_ROUND_UP_PAGE_SIZE_MY_ALGORITHM(unsigned long long value);
unsigned long long MY_ROUND_DOWN_PAGE_SIZE_MY_ALGORITHM(unsigned long long value);
unsigned long long MY_ROUND_UP_PAGE_SIZE_DEFAULT_ALGORITHM(unsigned long long value);
unsigned long long MY_ROUND_DOWN_PAGE_SIZE_DEFAULT_ALGORITHM(unsigned long long value);
unsigned long long MY_ROUND_DOWN_PAGE_SIZE(unsigned long long value);
unsigned long long MY_ROUND_UP_PAGE_SIZE(unsigned long long value);
#ifndef _WINDOWS_HELPER_TO_BOOL_IS_DEFINED
#define _WINDOWS_HELPER_TO_BOOL_IS_DEFINED
#define _WINDOWS_HELPER_TO_BOOL(x) ((BOOL)(((int)(x)) & 1))
#endif
#ifndef _WINDOWS_HELPER_TO_HEX_IS_DEFINED
#define _WINDOWS_HELPER_TO_HEX_IS_DEFINED
#define _WINDOWS_HELPER_TO_HEX(i) (i <= 9 ? '0' + i : 'A' - 10 + i)
extern char*_WINDOWS_HELPER_TO_HEX_STRING(unsigned long long x);
#endif
extern SIZE_T WINAPI VirtualQueryUnix(LPCVOID lpAddress,PMEMORY_BASIC_INFORMATION lpBuffer,SIZE_T dwLength);
extern BOOL WINAPI VirtualQueryUnixAdjustment(PMEMORY_BASIC_INFORMATION lpBuffer,PMEMORY_BASIC_INFORMATION newBuffer,SIZE_T dwLength);
extern BOOL WINAPI VirtualQueryUnixGetFreeMemoryRegion(PMEMORY_BASIC_INFORMATION lpBuffer,PMEMORY_BASIC_INFORMATION newBuffer,SIZE_T dwLength);
extern SIZE_T WINAPI VirtualQueryUnixX(LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength,PMEMORY_BASIC_INFORMATION newBuffer,BOOL * sset);
extern BOOL WINAPI VirtualProtectUnixX(LPVOID lpAddress,SIZE_T dwSize,DWORD  flNewProtect,PDWORD lpflOldProtect,PMEMORY_BASIC_INFORMATION newBuffer, BOOL * sset);
extern LPVOID WINAPI VirtualAllocUnixX(LPVOID lpAddress,SIZE_T dwSize,DWORD flAllocationType,DWORD flProtect,PMEMORY_BASIC_INFORMATION newBuffer,BOOL * sset);
extern BOOL WINAPI VirtualFreeUnixX(LPVOID lpAddress,SIZE_T dwSize,DWORD dwFreeType,PMEMORY_BASIC_INFORMATION newBuffer,BOOL * sset);
extern void * GET_VIRTUAL_END_ADDRESS();
extern void * __SBRK__WRAPPER(int increment);
extern int __BRK__WRAPPER(void * p);
extern BOOL MY_IS_PAGE_SIZE_ALIGNED(unsigned long long value);
extern BOOL MY_HAS_PAGE_SIZE();
#ifndef USE_OTHER_ROUND_UP_PAGE_SIZE_ALGORITHM
#define USE_OTHER_ROUND_UP_PAGE_SIZE_ALGORITHM
#endif
#ifndef USE_OTHER_ROUND_DOWN_PAGE_SIZE_ALGORITHM
#define USE_OTHER_ROUND_DOWN_PAGE_SIZE_ALGORITHM
#endif
#ifndef __VOID_POINTER_PROTECTION
#define __VOID_POINTER_PROTECTION
#define __VOID__POINTER__PROTECTOR(s) ((((void*)(s))<=((void*)(0)))?(0):((void*)(s)))
#endif
#endif
