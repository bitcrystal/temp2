#ifndef CAPSTONE_PREDEF_H
#define CAPSTONE_PREDEF_H

#include "../../../subhook/my_predef.h"
#if defined(MY_CPU_ARM64)
#define CAPSTONE_HAS_ARM64
#elif defined(MY_CPU_ARM)
#define CAPSTONE_HAS_ARM
#elif defined(MY_CPU_MIPS)
#define CAPSTONE_HAS_MIPS
#elif defined(MY_CPU_POWERPC) || defined(MY_CPU_POWERPC_64)
#define CAPSTONE_HAS_POWERPC
#elif defined(MY_CPU_SPARC)
#define CAPSTONE_HAS_SPARC
#elif defined(MY_CPU_SYSTEMZ)
#define CAPSTONE_HAS_SYSZ
#elif defined(MY_CPU_X86_64)||defined(MY_CPU_X86)
#define CAPSTONE_HAS_X86
//#define CAPSTONE_X86_REDUCE
//#define CAPSTONE_X86_ATT_DISABLE
#elif defined(OS_MAC)
#define CAPSTONE_HAS_XCORE
#endif
#ifdef OS_MAC
#define CAPSTONE_HAS_OSXKERNEL
#endif
#define CAPSTONE_USE_SYS_DYN_MEM
//#define CAPSTONE_DIET
#define CAPSTONE_STATIC
//#define CAPSTONE_SHARED

#endif
