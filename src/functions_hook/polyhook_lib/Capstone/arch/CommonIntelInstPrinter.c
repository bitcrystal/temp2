#include "CommonIntelInstPrinter.h"
#if defined(CAPSTONE_HAS_ARM64)
#define NO_INTEL_INST_PRINTER_ARM64
#elif defined(CAPSTONE_HAS_ARM)
#define NO_INTEL_INST_PRINTER_ARM
#elif defined(CAPSTONE_HAS_MIPS)
#define NO_INTEL_INST_PRINTER_MIPS
#elif defined(CAPSTONE_HAS_POWERPC)
#define NO_INTEL_INST_PRINTER_POWERPC
#elif defined(CAPSTONE_HAS_SPARC)
#define NO_INTEL_INST_PRINTER_SPARC
#elif defined(CAPSTONE_HAS_SYSZ)
#define NO_INTEL_INST_PRINTER_SYSZ
#elif defined(CAPSTONE_HAS_X86)
#include "X86/X86IntelInstPrinter.c"
#elif defined(CAPSTONE_HAS_XCORE)
#define NO_INTEL_INST_PRINTER_XCORE
#endif