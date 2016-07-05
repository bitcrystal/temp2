#ifdef __cplusplus
extern "C"
{
#endif
#include "src/functions_hook/subhook/windows_platform_defs.h"
#ifdef __cplusplus
}
#endif
int main()
{
	VirtualAlloc(0,0,0,0);
	return 0;
}
