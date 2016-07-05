#ifndef LINUX_OS_3_H
#define LINUX_OS_3_H
#include "my_predef.h"
#if defined(OS_MAC)
#include <mach-o/getsect.h>
extern void * get_end_ex();
extern void * get_etext_ex();
extern void * get_edata_ex();
#else
extern unsigned long get_end();
extern unsigned long get_etext();
extern unsigned long get_edata();
#endif
extern void * get_end_ex();
extern void * get_etext_ex();
extern void * get_edata_ex();
#endif
