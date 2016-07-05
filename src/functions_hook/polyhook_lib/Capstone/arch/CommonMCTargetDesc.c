#include "CommonMCTargetDesc.h"
#if defined(CAPSTONE_HAS_ARM64)
#define NO_MC_TARGET_DESC_ARM64
#elif defined(CAPSTONE_HAS_ARM)
#define NO_MC_TARGET_DESC_ARM
#elif defined(CAPSTONE_HAS_MIPS)
#define NO_MC_TARGET_DESC_MIPS
#elif defined(CAPSTONE_HAS_POWERPC)
#define NO_MC_TARGET_DESC_POWERPC
#elif defined(CAPSTONE_HAS_SPARC)
#define NO_MC_TARGET_DESC_SPARC
#elif defined(CAPSTONE_HAS_SYSZ)
#include "SystemZ/SystemZMCTargetDesc.c"
#elif defined(CAPSTONE_HAS_X86)
#define NO_MC_TARGET_DESC_X86
#elif defined(CAPSTONE_HAS_XCORE)
#define NO_MC_TARGET_DESC_XCORE
#endif