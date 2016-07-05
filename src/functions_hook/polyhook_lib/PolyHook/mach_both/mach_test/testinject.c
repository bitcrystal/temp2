#include "testinject.h"

int main()
{
 MACH_INJECTOR_S_t n = MACH_INJECTOR_S_t_new();
 MACH__Injector__init(n);
 int e = (int)MACH__Injector__inject__processname(n,"testapp","./libtestdylib.dylib");
 printf("First %d\n",e);
 int x = MACH__Injector__inject(n);
 printf("Second %d\n",x);
 MACH__Injector__deinit(n);
 MACH_INJECTOR_S_t_delete(n);
}
