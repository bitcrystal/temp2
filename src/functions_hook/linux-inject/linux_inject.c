#include "linux_injector.c"

int main(int argc, char**argv)
{
        if(argc < 4)
        {
                usage(argv[0]);
                return 1;
        }

        char* command = argv[1];
        char* commandArg = argv[2];
        char* libname = argv[3];
        char* processName = NULL;
        pid_t target = 0;

	LINUX_INJECTOR_S_t n = LINUX_INJECTOR_S_t_new();
	LINUX__Injector__init(n);
	if(!strcmp(command, "-n"))
        {
                processName = commandArg;
                if(LINUX__Injector__inject__processname(n,processName,libname)==0)
                {
                        fprintf(stderr, "doesn't look like a process named \"%s\" is running right now\n", processName);
                        return 0;
                }

                printf("targeting process \"%s\" with pid %d\n", processName, n->pid);
        }
        else if(!strcmp(command, "-p"))
        {
                target = atoi(commandArg);
		int x = LINUX__Injector__inject__pid(n,target,libname);
                printf("targeting process with pid %d\n", target);
		if(x==0)
			return 0;
        }
        else
        {
                usage(argv[0]);
                return 0;
        }

	LINUX__Injector__inject(n);
	LINUX__Injector__deinit(n);
}
