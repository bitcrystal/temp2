#include "linux_os_3.h"
#if defined(OS_UNIX_STRUCT)
#if defined(OS_MAC)
void * get_end_ex()
{
	return (void*)get_end();
}
void * get_etext_ex()
{
	return (void*)get_etext();
}
void * get_edata_ex()
{
	return (void*)get_edata();
}
#else
extern char end,etext,edata;
unsigned long get_end()
{
	return (unsigned long)&end;
}
unsigned long get_etext()
{
	return (unsigned long)&etext;
}
unsigned long get_edata()
{
	return (unsigned long)&edata;
}
void * get_end_ex()
{
	return (void*)&end;
}
void * get_etext_ex()
{
	return (void*)&etext;
}
void * get_edata_ex()
{
	return (void*)&edata;
}
#endif
#else
unsigned long get_end()
{
        return (unsigned long)0;
}
unsigned long get_etext()
{
        return (unsigned long)0;
} 
unsigned long get_edata()
{
        return (unsigned long)0;
}
void * get_end_ex()
{
        return (void*)0; 
}
void * get_etext_ex()
{
        return (void*)0;
}
void * get_edata_ex()
{
        return (void*)0;
}
#endif
