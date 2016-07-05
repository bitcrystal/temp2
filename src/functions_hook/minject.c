#include "minject.h"
#if defined(OS_MAC)
int MACH__Injector__uninject(MACH_INJECTOR_S_t mas)
{
	return 0;
}
#include "mach_inject/mach_injector.c"
#elif defined(OS_WIN)
#include "windows_inject/windows_injector.c"
#else
int LINUX__Injector__uninject(LINUX_INJECTOR_S_t mas)
{
	return 0;
}
#include "linux_inject/linux_injector.c"
#endif

int main()
{
}