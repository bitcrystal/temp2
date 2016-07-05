#include "CommonInstPrinter.h"
#if defined(CAPSTONE_HAS_ARM64)
#include "AArch64/AArch64InstPrinter.c"
#elif defined(CAPSTONE_HAS_ARM)
#include "ARM/ARMInstPrinter.c"
#elif defined(CAPSTONE_HAS_MIPS)
#include "Mips/MipsInstPrinter.c"
#elif defined(CAPSTONE_HAS_POWERPC)
#include "PowerPC/PPCInstPrinter.c"
#elif defined(CAPSTONE_HAS_SPARC)
#include "Sparc/SparcInstPrinter.c"
#elif defined(CAPSTONE_HAS_SYSZ)
#include "SystemZ/SystemZInstPrinter.c"
#elif defined(CAPSTONE_HAS_X86)
#define NO_INST_PRINTER_X86
#elif defined(CAPSTONE_HAS_XCORE)
#include "XCore/XCoreInstPrinter.c"
#endif