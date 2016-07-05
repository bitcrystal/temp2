#include "CommonModule.h"
#if defined(CAPSTONE_HAS_ARM64)
#include "AArch64/AArch64Module.c"
#elif defined(CAPSTONE_HAS_ARM)
#include "ARM/ARMModule.c"
#elif defined(CAPSTONE_HAS_MIPS)
#include "Mips/MipsModule.c"
#elif defined(CAPSTONE_HAS_POWERPC)
#include "PowerPC/PPCModule.c"
#elif defined(CAPSTONE_HAS_SPARC)
#include "Sparc/SparcModule.c"
#elif defined(CAPSTONE_HAS_SYSZ)
#include "SystemZ/SystemZModule.c"
#elif defined(CAPSTONE_HAS_X86)
#include "X86/X86Module.c"
#elif defined(CAPSTONE_HAS_XCORE)
#include "XCore/XCoreModule.c"
#endif