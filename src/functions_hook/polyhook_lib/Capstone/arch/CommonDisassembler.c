#include "CommonDisassembler.h"
#if defined(CAPSTONE_HAS_ARM64)
#include "AArch64/AArch64Disassembler.c"
#elif defined(CAPSTONE_HAS_ARM)
#include "ARM/ARMDisassembler.c"
#elif defined(CAPSTONE_HAS_MIPS)
#include "Mips/MipsDisassembler.c"
#elif defined(CAPSTONE_HAS_POWERPC)
#include "PowerPC/PPCDisassembler.c"
#elif defined(CAPSTONE_HAS_SPARC)
#include "Sparc/SparcDisassembler.c"
#elif defined(CAPSTONE_HAS_SYSZ)
#include "SystemZ/SystemZDisassembler.c"
#elif defined(CAPSTONE_HAS_X86)
#include "X86/X86Disassembler.c"
#elif defined(CAPSTONE_HAS_XCORE)
#include "XCore/XCoreDisassembler.c"
#endif