#ifndef LINUX_DEFS_C
#define LINUX_DEFS_C
#include "linux_defs.h"
#ifdef LINUX_CACHEFLUSH_DEF
int cacheflush(char *addr, int nbytes, int cache)
{
	#ifndef OS_MAC
	if(cache & BCACHE)
	{
		FILE* f;
		f = fopen("/proc/sys/vm/drop_caches", "w");
		if (f == NULL) {
			return -1;
		}
		if (fprintf(f, "3\n") != 2) {
			fprintf(stderr, "flush-cache: Couldn't write 3 to /proc/sys/vm/drop_caches\n");
			fclose(f);
			return -1;
		}
		fclose(f);
	} else {
		if(cache & ICACHE)
		{
			FILE* f;
			f = fopen("/proc/sys/vm/drop_caches", "w");
			if (f == NULL) {
				return -1;
			}
			if (fprintf(f, "1\n") != 2) {
				fprintf(stderr, "flush-cache: Couldn't write 3 to /proc/sys/vm/drop_caches\n");
				fclose(f);
				return -1;
			}
			fclose(f);
		}
		if(cache & DCACHE)
		{
			FILE* f;
			f = fopen("/proc/sys/vm/drop_caches", "w");
			if (f == NULL) {
				return -1;
			}
			if (fprintf(f, "2\n") != 2) {
				fprintf(stderr, "flush-cache: Couldn't write 3 to /proc/sys/vm/drop_caches\n");
				fclose(f);
				return -1;
			}
			fclose(f);
		}
	}
	#else
		system("sync & sudo purge");
	#endif
	return 0;
}
#endif
#endif