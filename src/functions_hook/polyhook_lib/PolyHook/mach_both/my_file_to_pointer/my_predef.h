//Copyright by bitcrystal
#ifndef MY_PREDEF_H
#define MY_PREDEF_H

#if defined(__arm__) || defined(__thumb__) || defined(__TARGET_ARCH_ARM) || defined(__TARGET_ARCH_THUMB) || defined(_ARM) || defined(_M_ARM) || defined(_M_ARMT)
#define MY_CPU_ARM
#define MY_CPU_VERSION 0
#elif defined(__aarch64__)
#define MY_CPU_ARM64
#define MY_CPU_VERSION 1
#elif defined(__bfin) || defined(__BFIN__)
#define MY_CPU_BARFIN
#define MY_CPU_VERSION 2
#elif defined(__convex__)
#define MY_CPU_CONVEX
#define MY_CPU_VERSION 3
#elif defined(__epiphany__)
#define MY_CPU_EPIPHANY
#define MY_CPU_VERSION 4
#elif defined(__hppa__) || defined(__HPPA__) || defined(__hppa)
#define MY_CPU_HPPA
#define MY_CPU_VERSION 5
#elif defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64) || defined(_LP64) || defined(__LP64__) || defined(_ILP64) || defined(LP64) || defined(ILP64) || defined(__MINGW64__) || defined(_WIN64) || defined(__ILP64__) || defined(WIN64)
#define MY_CPU_X86_64
#define MY_CPU_VERSION 6_1
#elif defined(__i386) || defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || defined(__IA32__) || defined(_M_I86) || defined(_M_IX86) || defined(__X86__) || defined(_X86_) || defined(__THW_INTEL__) || defined(__I86__) || defined(__INTEL__) || defined(__386) || defined(_ILP32) || defined(__ILP32__)
#define MY_CPU_X86
#define MY_CPU_VERSION 6
#elif defined(__ia64__) || defined(_IA64) || defined(__IA64__) || defined(__ia64) || defined(_M_IA64) || defined(__itanium__)
#define MY_CPU_IA64
#define MY_CPU_VERSION 7
#elif defined(__m68k__) || defined(M68000) || defined(__MC68K__)
#define MY_CPU_M68K
#define MY_CPU_VERSION 8
#elif defined(__mips__) || defined(mips) || defined(__mips) || defined(__MIPS__)
#define MY_CPU_MIPS
#define MY_CPU_VERSION 9
#elif defined(__powerpc64__) || defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) || defined(_ARCH_PPC64)
#define MY_CPU_POWERPC_64
#define MY_CPU_VERSION 10_1
#elif defined(__powerpc) || defined(__powerpc__) || defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC) || defined(_M_PPC) || defined(__ppc)
#define MY_CPU_POWERPC
#define MY_CPU_VERSION 10
#elif defined(pyr)
#define MY_CPU_PYRAMID_9810
#define MY_CPU_VERSION 11
#elif defined(__THW_RS6000) || defined(_IBMR2) || defined(_POWER) || defined(_ARCH_PWR) || defined(_ARCH_PWR2) || defined(_ARCH_PWR3) || defined(_ARCH_PWR4)
#define MY_CPU_RS6000
#define MY_CPU_VERSION 12
#elif defined(__sparc__) || defined(__sparc)
#define MY_CPU_SPARC
#define MY_CPU_VERSION 13
#elif defined(__sh__)
#define MY_CPU_SUPERH
#define MY_CPU_VERSION 14
#elif defined(__370__) || defined(__THW_370__) || defined(__s390__) || defined(__s390x__) || defined(__zarch__) || defined(__SYSC_ZARCH__)
#define MY_CPU_SYSTEMZ
#define MY_CPU_VERSION 15
#elif defined(_TMS320C2XX) || defined(__TMS320C2000__) || defined(_TMS320C5X) || defined(__TMS320C55X__) || defined(_TMS320C6X) || defined(__TMS320C6X__)
#define MY_CPU_TMS320
#define MY_CPU_VERSION 16
#elif defined(__TMS470__)
#define MY_CPU_TMS470
#define MY_CPU_VERSION 17
#elif defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA)
#define MY_CPU_ALPHA
#define MY_CPU_VERSION 18
#else
#define MY_CPU_UNKNOWN
#define MY_CPU_VERSION 19
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__) || defined(__MINGW64__)
#define OS_WIN
#elif defined(__linux__)
#define OS_LINUX
#elif defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
#define OS_IPHONE_SIMULATOR  
#elif TARGET_OS_IPHONE
#define OS_IPHONE
#else
#define OS_OSX
#endif
#define OS_APPLE
#elif defined(__unix__)
#define OS_UNIX
#elif defined(__posix)
#define OS_POSIX
#elif defined(__MACH__)
#define OS_MACH
#elif defined(__FreeBSD__)
#define OS_FREE_BSD
#elif defined(__NetBSD__)
#define OS_NET_BSD
#elif defined(HAVE_MQUERY)
#define OS_OPEN_BSD
#elif defined(__BEOS__)
#define OS_BEOS
#elif defined(__HAIKU__)
#define OS_HAIKU
#else
#define OS_UNKNOWN
#endif
#if defined(OS_APPLE) || defined(OS_UNIX) || defined(OS_MACH)
#define OS_AUFGEGESSENER_APFEL
#endif
#ifdef OS_AUFGEGESSENER_APFEL
#define OS_EATEN_APPLE
#define OS_MAC
#endif
#if defined(OS_FREE_BSD)||defined(OS_NET_BSD)||defined(OS_OPEN_BSD)
#define OS_BSD_STRUCT
#endif
#if defined(OS_MAC)||defined(OS_BSD_STRUCT)||defined(OS_LINUX)
#define OS_UNIX_STRUCT
#endif
#if defined(MY_CPU_X86_64)
#define IS_X64
#elif defined(MY_CPU_X86)
#define IS_X86
#else
#define IS_X86
#endif
#endif
#if defined(OS_WIN)
	#ifndef WIN
		#define WIN
	#endif
	#ifndef _WIN
		#define _WIN
	#endif
	#ifndef _WIN32
		#define _WIN32
	#endif
	#ifndef WIN32
		#define WIN32
	#endif
	#define BITCRYSTAL_OS_VERSION 1
#elif defined(OS_LINUX)
	#ifndef __linux__
		#define __linux__
	#endif
	#define BITCRYSTAL_OS_VERSION 2
#elif defined(OS_MAC)
	#if defined(OS_UNIX)
		#ifndef __unix__
			#define __unix__
		#endif
	#elif defined(OS_APPLE)
		#ifndef __APPLE__
			#define __APPLE__
		#endif
	#elif defined(OS_MACH)
		#ifndef __MACH__
			#define __MACH__
		#endif
	#else
		#define OS_UNKNOWN
	#endif
	#define BITCRYSTAL_OS_VERSION 3
#endif
#if defined(IS_X86)
#ifndef _M_IX86
	#define _M_IX86
#endif
#ifndef __i386__
	#define __i386__
#endif
//#ifndef _WIN32
//	#define _WIN32
//#endif
//	#define WIN32
#ifndef _ILP32
	#define _ILP32
#endif
#ifndef __ILP32__
	#define __ILP32__
#endif
#ifndef _LP32
	#define _LP32
#endif
//	#defined __MINGW32__
//	#define WIN32
	#define BITCRYSTAL_OS_ARCH 1
#elif defined(IS_X64)
#ifndef _M_X64
	#define _M_X64
#endif
#ifndef _M_AMD64
	#define _M_AMD64
#endif
#ifndef __amd64__
	#define __amd64__
#endif
#ifndef _LP64
	#define _LP64
#endif
#ifndef _ILP64
	#define _ILP64
#endif
#ifndef LP64
	#define LP64
#endif
#ifndef ILP64
	#define ILP64
#endif
//	#define __MINGW64__
//#ifndef _WIN64
//	#define _WIN64
//#endif
//	#define WIN64
	#define BITCRYSTAL_OS_ARCH 2

#endif
