#include <stdio.h>
#include <stdlib.h>
#include <mach/mach_traps.h>
#include <mach/mach_init.h>
#include <mach/mach_error.h>

int main(int argc, char* argv[])
{
    task_t task;
    pid_t pid = argc >= 2 ? atoi(argv[1]) : 1;
    printf("pid: %d\n",pid);
    kern_return_t error = task_for_pid(mach_task_self(), pid, &task);
    printf("%d -> %x [%d - %s]\n", pid, task, error, mach_error_string(error));
    return error;
}
