#include "CommonBaseInfo.h"
#if defined(CAPSTONE_HAS_ARM64)
#include "AArch64/AArch64BaseInfo.c"
#elif defined(CAPSTONE_HAS_ARM)
#include "ARM/ARMBaseInfo.c"
#elif defined(CAPSTONE_HAS_MIPS)
#define NO_BASE_INFO_MIPS
#elif defined(CAPSTONE_HAS_POWERPC)
#define NO_BASE_INFO_POWERPC
#elif defined(CAPSTONE_HAS_SPARC)
#define NO_BASE_INFO_SPARC
#elif defined(CAPSTONE_HAS_SYSZ)
#define NO_BASE_INFO_SYSZ
#elif defined(CAPSTONE_HAS_X86)
#define NO_BASE_INFO_X86
#elif defined(CAPSTONE_HAS_XCORE)
#define NO_BASE_INFO_XCORE
#endif