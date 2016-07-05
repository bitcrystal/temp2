#include "linux_injector.h"
#include "linux_injector_helper.c"

LINUX_INJECTOR_S_t LINUX_INJECTOR_S_t_new()
{
	return (LINUX_INJECTOR_S_t)malloc(sizeof(LINUX_INJECTOR_S));
}

int LINUX_INJECTOR_S_t_delete(LINUX_INJECTOR_S_t mas)
{
	if(mas==0)
		return 0;
	free((void*)mas);
	return 1;
}

void LINUX__Injector__init(LINUX_INJECTOR_S_t mas)
{
	if(mas==0||mas->is_init==1)
		return;
	memset(mas,0,sizeof(LINUX_INJECTOR_S));
	mas->mypid = getpid();
	mas->mylibcaddr = getlibcaddr(mas->mypid);

	mas->mallocAddr = getFunctionAddress("malloc");
	mas->freeAddr = getFunctionAddress("free");
	mas->dlopenAddr = getFunctionAddress("__libc_dlopen_mode");
#ifdef ARM
	mas->raiseAddr = getFunctionAddress("raise");
#endif
	mas->mallocOffset = mas->mallocAddr - mas->mylibcaddr;
	mas->freeOffset = mas->freeAddr - mas->mylibcaddr;
	mas->dlopenOffset = mas->dlopenAddr - mas->mylibcaddr;
#ifdef ARM
	mas->raiseOffset = mas->raiseAddr - mas->mylibcaddr;
#endif
	mas->is_init=1;
}

void LINUX__Injector__deinit(LINUX_INJECTOR_S_t mas)
{
	if(mas==0||mas->is_init==0||mas->is_deinit==1)
		return;
	memset(mas,0,sizeof(LINUX_INJECTOR_S));
	mas->is_init=1;
	mas->is_deinit=1;
}

void LINUX__Injector__reset(LINUX_INJECTOR_S_t mas)
{
	if(mas==0)
		return;
	memset(mas,0,sizeof(LINUX_INJECTOR_S));
}

int LINUX__Injector__inject__pid(LINUX_INJECTOR_S_t mas, pid_t pid, const char* lib)
{
	if(mas==0||mas->is_init==0||mas->is_deinit==1||mas->is_installed==1||lib==0||pid==0)
		return 0;
	memset((void*)&mas->path[0],0,4096);
	memset((void*)&mas->libname[0],0,4096);
	memcpy((void*)&mas->libname[0],(void*)&lib[0],(strlen(lib)+1));
	if(realpath((char*)&lib[0],(char*)&mas->path[0])==0)
		return 0;
	mas->pid=pid;
	mas->libPathLength = strlen(mas->path) + 1;
	mas->targetLibcAddr = getlibcaddr(pid);
	mas->targetMallocAddr = mas->targetLibcAddr + mas->mallocOffset;
	mas->targetFreeAddr = mas->targetLibcAddr + mas->freeOffset;
	mas->targetDlopenAddr = mas->targetLibcAddr + mas->dlopenOffset;
#ifdef ARM
	mas->targetRaiseAddr = mas->targetLibcAddr + mas->raiseOffset;
#endif
	mas->is_installed=1;
	return 0;
}

int LINUX__Injector__inject__processname(LINUX_INJECTOR_S_t mas,const char * processname,const char * lib)
{
	if(mas==0||mas->is_init==0||mas->is_deinit==1||mas->is_installed==1||lib==0)
		return 0;
	pid_t r = LINUX__Injector__getProcessByName(processname);
	if(r==0)
		return 0;
	return LINUX__Injector__inject__pid(mas,r,lib);
}

int LINUX__Injector__inject(LINUX_INJECTOR_S_t mas)
{
	if(mas==0||mas->is_init==0||mas->is_deinit==1||mas->is_installed==0||mas->is_injected==1)
		return 0;
	struct REG_TYPE oldregs, regs;
	memset(&oldregs, 0, sizeof(struct REG_TYPE));
	memset(&regs, 0, sizeof(struct REG_TYPE));

	pid_t target = mas->pid;
	ptrace_attach(target);

	ptrace_getregs(target, &oldregs);
	memcpy(&regs, &oldregs, sizeof(struct REG_TYPE));

	// find a good address to copy code to
	long addr = freespaceaddr(target) + sizeof(long);

	// now that we have an address to copy code to, set the target's
	// program counter to it.
	//
	// subtract 4 bytes from the actual address, because ARM's PC actually
#ifdef ARM
	// refers to the next instruction rather than the current instruction.
	regs.uregs[15] = addr - 4;

	// pass arguments to my function injectSharedLibrary() by loading them
	// into the right registers. see comments in injectSharedLibrary() for
	// more details.
	regs.uregs[1] = mas->targetRaiseAddr;
	regs.uregs[2] = mas->targetMallocAddr;
	regs.uregs[3] = mas->targetDlopenAddr;
	regs.uregs[4] = mas->targetFreeAddr;
	regs.uregs[5] = mas->libPathLength;
#elif defined(X64)
	// now that we have an address to copy code to, set the target's rip to
	// it. we have to advance by 2 bytes here because rip gets incremented
	// by the size of the current instruction, and the instruction at the
	// start of the function to inject always happens to be 2 bytes long.
	regs.rip = addr + 2;

	// pass arguments to my function injectSharedLibrary() by loading them
	// into the right registers. note that this will definitely only work
	// on x64, because it relies on the x64 calling convention, in which
	// arguments are passed via registers rdi, rsi, rdx, rcx, r8, and r9.
	// see comments in injectSharedLibrary() for more details.
	regs.rdi = mas->targetMallocAddr;
	regs.rsi = mas->targetFreeAddr;
	regs.rdx = mas->targetDlopenAddr;
	regs.rcx = mas->libPathLength;
#elif defined(X86)
	// now that we have an address to copy code to, set the target's eip to it.
	regs.eip = addr;

	// pass arguments to my function injectSharedLibrary() by loading them
	// into the right registers. see comments in injectSharedLibrary() for
	// more details.
	regs.ebx = mas->targetMallocAddr;
	regs.edi = mas->targetDlopenAddr;
	regs.esi = mas->targetFreeAddr;
	regs.ecx = mas->libPathLength;
#endif
	ptrace_setregs(target, &regs);

	// figure out the size of injectSharedLibrary() so we know how big of a buffer to allocate. 
	size_t injectSharedLibrary_size = (intptr_t)injectSharedLibrary_end - (intptr_t)injectSharedLibrary;

	// back up whatever data used to be at the address we want to modify.
	char* backup = malloc(injectSharedLibrary_size * sizeof(char));
	ptrace_read(target, addr, backup, injectSharedLibrary_size);

	// set up a buffer containing the code that we'll inject into the target process.
	char* newcode = malloc(injectSharedLibrary_size * sizeof(char));
	memset(newcode, 0, injectSharedLibrary_size * sizeof(char));

	// copy the code of injectSharedLibrary() to the buffer.
	memcpy(newcode, injectSharedLibrary, injectSharedLibrary_size);

	// copy injectSharedLibrary()'s code to the target address inside the
	// target process' address space.
	ptrace_write(target, addr, newcode, injectSharedLibrary_size);

	// now that the new code is in place, let the target run our injected code.
	ptrace_cont(target);

	// at this point, the target should have run malloc(). check its return
	// value to see if it succeeded, and bail out cleanly if it didn't.
	struct REG_TYPE malloc_regs;
	memset(&malloc_regs, 0, sizeof(struct REG_TYPE));
	ptrace_getregs(target, &malloc_regs);
#ifdef ARM
	unsigned long long targetBuf = malloc_regs.uregs[5];
#elif defined(X64)
	unsigned long long targetBuf = malloc_regs.rax;
#elif defined(X86)
	unsigned long long targetBuf = malloc_regs.eax;
#endif
	if(targetBuf == 0)
	{
		//fprintf(stderr, "malloc() failed to allocate memory\n");
		restoreStateAndDetach(target, addr, backup, injectSharedLibrary_size, oldregs);
		free(backup);
		free(newcode);
		mas->is_injected=0;
		return 0;
	}

	// if we get here, then malloc likely succeeded, so now we need to copy
	// the path to the shared library we want to inject into the buffer
	// that the target process just malloc'd. this is needed so that it can
	// be passed as an argument to __libc_dlopen_mode later on.

	// read the buffer returned by malloc() and copy the name of our shared
	// library to that address inside the target process.
	ptrace_write(target, targetBuf, mas->path, mas->libPathLength);

	// continue the target's execution again in order to call
	// __libc_dlopen_mode.
	ptrace_cont(target);

	// check out what the registers look like after calling
	// __libc_dlopen_mode.
	struct REG_TYPE dlopen_regs;
	memset(&dlopen_regs, 0, sizeof(struct REG_TYPE));
	ptrace_getregs(target, &dlopen_regs);
#ifdef ARM
	unsigned long long libAddr = dlopen_regs.uregs[4];
#elif defined(X64)
	unsigned long long libAddr = dlopen_regs.rax;
#elif defined(X86)
	unsigned long long libAddr = dlopen_regs.eax;
#endif

	// if r4 is 0 here, then __libc_dlopen_mode() failed, and we should
	// bail out cleanly.
	if(libAddr == 0)
	{
		//fprintf(stderr, "__libc_dlopen_mode() failed to load %s\n", mas->libname);
		restoreStateAndDetach(target, addr, backup, injectSharedLibrary_size, oldregs);
		free(backup);
		free(newcode);
		mas->is_injected=0;
		return 0;
	}

	// now check /proc/pid/maps to see whether injection was successful.
	if(checkloaded(target, mas->libname))
	{
		//printf("\"%s\" successfully injected\n", mas->libname);
		mas->is_injected=1;
	}
	else
	{
		//fprintf(stderr, "could not inject \"%s\"\n", mas->libname);
		mas->is_injected=0;
	}

	// as a courtesy, free the buffer that we allocated inside the target
	// process. we don't really care whether this succeeds, so don't
	// bother checking the return value.
	ptrace_cont(target);

	// at this point, if everything went according to plan, we've loaded
	// the shared library inside the target process, so we're done. restore
	// the old state and detach from the target.
	restoreStateAndDetach(target, addr, backup, injectSharedLibrary_size, oldregs);
	free(backup);
	free(newcode);
	return (int)mas->is_injected;
}

pid_t LINUX__Injector__getProcessByName(const char *name)
{
	return findProcessByName((char*)name);
}
