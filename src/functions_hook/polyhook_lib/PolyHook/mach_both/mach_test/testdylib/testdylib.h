#ifndef TESTDYLIB_H
#define TESTDYLIB_H
#include <stdio.h>

#include "../mach_override/mach_override.h"
#include "../mach_override/mach_override_overrides.h"
#include "mach-o/dyld.h"
#include <CoreServices/CoreServices.h>

// Our override function
void my_print_hw();

void install(void) __attribute__ ((constructor));
#endif
