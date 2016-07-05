#include "whereami.h"
#include "whereami_ex.h"
#include "whereami.c"

char * getFullExecutablePath()
{
	static char path[4096];
	int length = wai_getExecutablePath(NULL, 0, NULL);
	if(length>=4096)
		return 0;
	wai_getExecutablePath(path,length,NULL);
	return path;
}

char * getFullModulePath()
{
	static char path[4096];
	int length = wai_getModulePath(NULL,0,NULL);
	if(length>=4096)
		return 0;
	wai_getModulePath(path,length,NULL);
	return path;
}

char * getExecutablePath(char lastchar)
{
	char * path = getFullExecutablePath();
	int i;
	for(i=4095;i>=0;i--)
	{
		if(path[i]=='/'||path[i]=='\\')
		{
			if(i<4095)
			{
				path[i]=lastchar;
				path[i+1]=0;
			} else {
				path[i]=0;
			}
			return path;
		}
	}
}

char * getModulePath(char lastchar)
{
        char * path = getFullModulePath();
        int i;
        for(i=4095;i>=0;i--)
        {
                if(path[i]=='/'||path[i]=='\\')
                {
                        if(i<4095)
			{
				path[i]=lastchar;
				path[i+1]=0;
			} else {
				path[i]=0;
			}
                        return path;
                }
        }
}
