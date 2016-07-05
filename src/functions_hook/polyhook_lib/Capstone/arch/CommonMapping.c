#include "CommonMapping.h"
#if defined(CAPSTONE_HAS_ARM64)
#include "AArch64/AArch64Mapping.c"
#elif defined(CAPSTONE_HAS_ARM)
#include "ARM/ARMMapping.c"
#elif defined(CAPSTONE_HAS_MIPS)
#include "Mips/MipsMapping.c"
#elif defined(CAPSTONE_HAS_POWERPC)
#include "PowerPC/PPCMapping.c"
#elif defined(CAPSTONE_HAS_SPARC)
#include "Sparc/SparcMapping.c"
#elif defined(CAPSTONE_HAS_SYSZ)
#include "SystemZ/SystemZMapping.c"
#elif defined(CAPSTONE_HAS_X86)
#include "X86/X86Mapping.c"
#elif defined(CAPSTONE_HAS_XCORE)
#include "XCore/XCoreMapping.c"
#endif