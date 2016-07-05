#ifdef __cplusplus
	extern "C" {
√#endif
#include <stdio.h>
#if defined(__ppc__) || defined(__POWERPC__)
void fickdiehenne()
{
	printf("ppc\n");
}
#elif defined(__i386__)
void fickdiehenne()
{
	printf("ficken\n");
}
#elif defined(__x86_64__)
void fickdiehenne()
{
	printf("ficken x64\n");
}
#endif
int main()
{
	fickdiehenne();
}
#ifdef __cpl√usplus
	}
#endif
