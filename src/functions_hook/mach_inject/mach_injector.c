#include "mach_injector.h"
#ifndef MACH_INJECT_OLD_METHOD
#include "submodules/liblorgnette/lorgnette.c"
#include "task_vaccine.c"
#else
#include "mach_inject.c"
#include "bootstrap_compile.c"
#endif

MACH_INJECTOR_S_t MACH_INJECTOR_S_t_new()
{
	return (MACH_INJECTOR_S_t)malloc(sizeof(MACH_INJECTOR_S));
}

int MACH_INJECTOR_S_t_delete(MACH_INJECTOR_S_t mas)
{
	if(mas==0)
		return 0;
	free((void*)mas);
	return 1;
}

void MACH__Injector__init(MACH_INJECTOR_S_t mas)
{
	if(mas==0||mas->is_init==1)
		return;
	memset(mas,0,sizeof(MACH_INJECTOR_S));
#ifdef MACH_INJECT_OLD_METHOD
    	mas->module = dlopen("bootstrap.dylib",
        	RTLD_NOW | RTLD_LOCAL);
	
    	if (!mas->module)
    	{
		delete_bootstrap_if_exists();
        	if(bootstrap_compile())
		{
			 if(!bootstrap_exists())
				return;
			 mas->module = dlopen("bootstrap.dylib",
		                 RTLD_NOW | RTLD_LOCAL);
			 if(!mas->module)
				return;
		}
    	}

    	mas->bootstrapfn = dlsym(mas->module, "bootstrap");

    	if (!mas->bootstrapfn)
    	{
        	return;
	}
#endif
	mas->is_init=1;
}

void MACH__Injector__deinit(MACH_INJECTOR_S_t mas)
{
	if(mas==0||mas->is_init==0||mas->is_deinit==1)
		return;
	memset(mas,0,sizeof(MACH_INJECTOR_S));
#ifdef MACH_INJECT_OLD_METHOD
	if (mas->module)
    	{
        	dlclose(mas->module);
        	mas->module = 0;
		mas->bootstrapfn = 0;
	}
#endif
	mas->is_init=1;
	mas->is_deinit=1;
}

void MACH__Injector__reset(MACH_INJECTOR_S_t mas)
{
	if(mas==0)
		return;
#ifdef MACH_INJECT_OLD_METHOD
        if (mas->module)
        {
                dlclose(mas->module);
                mas->module = 0;
		mas->bootstrapfn = 0;
        }
#endif
	memset(mas,0,sizeof(MACH_INJECTOR_S));
}

int MACH__Injector__inject__pid(MACH_INJECTOR_S_t mas, pid_t pid, const char* lib)
{
	if(mas==0||mas->is_init==0||mas->is_deinit==1||mas->is_installed==1||lib==0||pid==0)
		return 0;
#ifndef MACH_INJECT_OLD_METHOD
	task_t task;
	int r = MACH__getTask(pid,&task);
#else
	pid_t r = pid;
	pid_t task = r;
#endif
	if(r==0)
		return 0;

	return MACH__Injector__inject__task(mas,task,lib);
}

int MACH__Injector__inject__processname(MACH_INJECTOR_S_t mas,const char * processname,const char * lib)
{
	if(mas==0||mas->is_init==0||mas->is_deinit==1||mas->is_installed==1||lib==0)
		return 0;
#ifndef MACH_INJECT_OLD_METHOD
	task_t task;
	int r = MACH__getTaskByProcessName(processname,&task);
#else
	pid_t r;
	pid_t task;
	r = task = MACH__Injector__getProcessByName(processname);
#endif
	if(r==0)
		return 0;
	return MACH__Injector__inject__task(mas,task,lib);
}

int MACH__Injector__inject__task(MACH_INJECTOR_S_t mas,
#ifndef MACH_INJECT_OLD_METHOD
const task_t task,
#else
const pid_t task,
#endif
const char * lib)
{
        if(mas==0||mas->is_init==0||mas->is_deinit==1||mas->is_installed==1||lib==0)
                return 0;
        mas->task=task;
        memset((void*)&mas->path[0],0,4096);
        realpath((char*)&lib[0],(char*)&mas->path[0]);
        mas->is_installed=1;
        return 1;
}

int MACH__Injector__inject(MACH_INJECTOR_S_t mas)
{
	if(mas==0||mas->is_done==1||mas->is_injected==1||mas->is_init==0||mas->is_deinit==1||mas->is_installed==0)
		return 0;
#ifndef MACH_INJECT_OLD_METHOD
	//printf("ddfff\n");
	kern_return_t kret;
	//printf("%s\n",mas->path);
	//printf("%d\n",mas->task);
	kret=task_vaccine(mas->task, (const char*)&mas->path[0]);
	if(kret==KERN_SUCCESS)
	{
		mas->is_injected=1;
		return 1;
	}
	return 0;
#else
	if (!mas->module || !mas->bootstrapfn)
    	{
        	return 0;
    	}
	mach_error_t err = mach_inject((mach_inject_entry)mas->bootstrapfn, mas->path, strlen(mas->path) + 1, mas->task, 0);	
	int x = err?0:1;
	if(x)
	{
		mas->is_injected=1;
	}
	return x;
#endif
}

int MACH__getTaskByProcessName(const char * name,task_t*t)
{
	if(name==0||t==0)
		return 0;
	pid_t pid = MACH__Injector__getProcessByName(name);
	if(!pid)
		return 0;
	return MACH__getTask(pid,t);
}


int MACH__getTask(pid_t proc,task_t*t)
{
	if(t==0)
		return 0;
	kern_return_t kret;
	kret=task_for_pid(mach_task_self(), proc, t);
	if(kret!=KERN_SUCCESS)
	{
		return 0;
	}
	return 1;
}

pid_t MACH__Injector__getProcessByName(const char *name)
{
	if(name==0)
		return 0;
    int procCnt = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0);
    pid_t pids[1024];
    memset(pids, 0, sizeof pids);
    proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));

    for (int i = 0; i < procCnt; i++)
    {
        if (!pids[i]) continue;
        char curPath[PROC_PIDPATHINFO_MAXSIZE];
        char curName[PROC_PIDPATHINFO_MAXSIZE];
        memset(curPath, 0, sizeof curPath);
        proc_pidpath(pids[i], curPath, sizeof curPath);
        int len = strlen(curPath);
        if (len)
        {
            int pos = len;
            while (pos && curPath[pos] != '/') --pos;
            strcpy(curName, curPath + pos + 1);
            if (!strcmp(curName, name))
            {
                return pids[i];
            }
        }
    }
    return 0;
}
