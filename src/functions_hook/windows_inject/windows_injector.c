#include "windows_injector.h"
#include "windows_inject.c"

WINDOWS_INJECTOR_S_t WINDOWS_INJECTOR_S_t_new()
{
	return (WINDOWS_INJECTOR_S_t)malloc(sizeof(WINDOWS_INJECTOR_S));
}

int WINDOWS_INJECTOR_S_t_delete(WINDOWS_INJECTOR_S_t mas)
{
	if(mas==0)
		return 0;
	free((void*)mas);
	return 1;
}

void WINDOWS__Injector__init(WINDOWS_INJECTOR_S_t mas)
{
	if(mas==0||mas->is_init==1)
		return;
	memset(mas,0,sizeof(WINDOWS_INJECTOR_S));
	mas->is_init=1;
}

void WINDOWS__Injector__deinit(WINDOWS_INJECTOR_S_t mas)
{
	if(mas==0||mas->is_init==0||mas->is_deinit==1)
		return;
	memset(mas,0,sizeof(WINDOWS_INJECTOR_S));
	mas->is_init=1;
	mas->is_deinit=1;
}

void WINDOWS__Injector__reset(WINDOWS_INJECTOR_S_t mas)
{
	if(mas==0)
		return;
	memset(mas,0,sizeof(WINDOWS_INJECTOR_S));
}

int WINDOWS__Injector__inject__pid(WINDOWS_INJECTOR_S_t mas, pid_t pid, MY_CCHAR * lib)
{
	if(mas==0||mas->is_init==0||mas->is_deinit==1||mas->is_installed==1||lib==0||pid==0)
		return 0;
	memset((void*)&mas->path[0],0,(4096*sizeof(MY_CHAR)));
	memcpy((void*)&mas->path[0],(void*)&lib[0],((MY_STR_LEN(lib)*sizeof(MY_CHAR))+sizeof(MY_CHAR)));
	mas->pid=pid;
	mas->is_installed=1;
	return 0;
}

int WINDOWS__Injector__inject__processname(WINDOWS_INJECTOR_S_t mas,MY_CCHAR * processname,MY_CCHAR * lib)
{
	if(mas==0||mas->is_init==0||mas->is_deinit==1||mas->is_installed==1||lib==0)
		return 0;
	pid_t r = WINDOWS__Injector__getProcessByName(processname);
	if(r==0)
		return 0;
	return WINDOWS__Injector__inject__pid(mas,r,lib);
}

int WINDOWS__Injector__inject(WINDOWS_INJECTOR_S_t mas)
{
	if(mas==0||mas->is_init==0||mas->is_deinit==1||mas->is_installed==0||mas->is_injected==1)
		return 0;
		HMODULE in = MY_INJECT_DLL(mas->pid,mas->path);
		if(in==0)
			return 0;
		mas->module=in;
		mas->is_injected=1;
	return (int)mas->is_injected;
}

int WINDOWS__Injector__uninject(WINDOWS_INJECTOR_S_t mas)
{
	if(mas==0||mas->is_init==0||mas->is_deinit==1||mas->is_installed==0||mas->is_injected==0||mas->module==0)
		return 0;
		int in = (int)MY_UNINJECT_DLL(mas->pid,mas->module);
		if(in==1)
			mas->module=0;
	return in;
}

pid_t WINDOWS__Injector__getProcessByName(MY_CCHAR *name)
{
	return GET_PROCESS_ID_BY_NAME(name);
}
