#!/bin/bash
tar cvf t.tar ../mach_override/mach.h.txt ../mach_override/mach.txt ../mach_override/mach_override.c ../mach_override/mach_override.h ../mach_override/mach_override.h.gch ../mach_override/mach_override.o ../mach_override/mach_override_overrides.h testdylib.h testdylib.c
gzip -9 t.tar
my_file_to_pointer t.tar t.tar.c
sed -i.bak s/lib_memory()//g index.html
