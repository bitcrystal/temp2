#include "linux_injector_helper.h"
#include "utils.c"
#include "ptrace.c"

#if defined(MY_CPU_X86_64)
void injectSharedLibrary(long mallocaddr, long freeaddr, long dlopenaddr)
{
	// here are the assumptions I'm making about what data will be located
	// where at the time the target executes this code:
	//
	//   rdi = address of malloc() in target process
	//   rsi = address of free() in target process
	//   rdx = address of __libc_dlopen_mode() in target process
	//   rcx = size of the path to the shared library we want to load

	// save addresses of free() and __libc_dlopen_mode() on the stack for later use
	asm(
		// rsi is going to contain the address of free(). it's going to get wiped
		// out by the call to malloc(), so save it on the stack for later
		"push %rsi \n"
		// same thing for rdx, which will contain the address of _dl_open()
		"push %rdx"
	);

	// call malloc() from within the target process
	asm(
		// save previous value of r9, because we're going to use it to call malloc()
		"push %r9 \n"
		// now move the address of malloc() into r9
		"mov %rdi,%r9 \n"
		// choose the amount of memory to allocate with malloc() based on the size
		// of the path to the shared library passed via rcx
		"mov %rcx,%rdi \n"
		// now call r9; malloc()
		"callq *%r9 \n"
		// after returning from malloc(), pop the previous value of r9 off the stack
		"pop %r9 \n"
		// break in so that we can see what malloc() returned
		"int $3"
	);

	// call __libc_dlopen_mode() to load the shared library
	asm(
		// get the address of __libc_dlopen_mode() off of the stack so we can call it
		"pop %rdx \n"
		// as before, save the previous value of r9 on the stack
		"push %r9 \n"
		// copy the address of __libc_dlopen_mode() into r9
		"mov %rdx,%r9 \n"
		// 1st argument to __libc_dlopen_mode(): filename = the address of the buffer returned by malloc()
		"mov %rax,%rdi \n"
		// 2nd argument to __libc_dlopen_mode(): flag = RTLD_LAZY
		"movabs $1,%rsi \n"
		// call __libc_dlopen_mode()
		"callq *%r9 \n"
		// restore old r9 value
		"pop %r9 \n"
		// break in so that we can see what __libc_dlopen_mode() returned
		"int $3"
	);

	// call free() to free the buffer we allocated earlier.
	//
	// Note: I found that if you put a nonzero value in r9, free() seems to
	// interpret that as an address to be freed, even though it's only
	// supposed to take one argument. As a result, I had to call it using a
	// register that's not used as part of the x64 calling convention. I
	// chose rbx.
	asm(
		// at this point, rax should still contain our malloc()d buffer from earlier.
		// we're going to free it, so move rax into rdi to make it the first argument to free().
		"mov %rax,%rdi \n"
		// pop rsi so that we can get the address to free(), which we pushed onto the stack a while ago.
		"pop %rsi \n"
		// save previous rbx value
		"push %rbx \n"
		// load the address of free() into rbx
		"mov %rsi,%rbx \n"
		// zero out rsi, because free() might think that it contains something that should be freed
		"xor %rsi,%rsi \n"
		// break in so that we can check out the arguments right before making the call
		"int $3 \n"
		// call free()
		"callq *%rbx \n"
		// restore previous rbx value
		"pop %rbx"
	);

	// we already overwrote the RET instruction at the end of this function
	// with an INT 3, so at this point the injector will regain control of
	// the target's execution.
}

/*
 * injectSharedLibrary_end()
 *
 * This function's only purpose is to be contiguous to injectSharedLibrary(),
 * so that we can use its address to more precisely figure out how long
 * injectSharedLibrary() is.
 *
 */

void injectSharedLibrary_end()
{
}
#elif defined(MY_CPU_X86)
void injectSharedLibrary(long mallocaddr, long freeaddr, long dlopenaddr)
{
	// here are the assumptions I'm making about what data will be located
	// where at the time the target executes this code:
	//
	//   ebx = address of malloc() in target process
	//   edi = address of __libc_dlopen_mode() in target process
	//   esi = address of free() in target process
	//   ecx = size of the path to the shared library we want to load

	// for some reason it's adding 1 to esi, so subtract 1 from it
	asm("dec %esi");

	// call malloc() from within the target process
	asm(
		// choose the amount of memory to allocate with malloc() based on the size
		// of the path to the shared library passed via ecx
		"push %ecx \n"
		// call malloc
		"call *%ebx \n"
		// copy malloc's return value (i.e. the address of the allocated buffer) into ebx
		"mov %eax, %ebx \n"
		// break back in so that the injector can get the return value
		"int $3"
	);

	// call __libc_dlopen_mode() to load the shared library
	asm(
		// 2nd argument to __libc_dlopen_mode(): flag = RTLD_LAZY
		"push $1 \n"
		// 1st argument to __libc_dlopen_mode(): filename = the buffer we allocated earlier
		"push %ebx \n"
		// call __libc_dlopen_mode()
		"call *%edi \n"
		// break back in so that the injector can check the return value
		"int $3"
	);

	// call free() on the previously malloc'd buffer
	asm(
		// 1st argument to free(): ptr = the buffer we allocated earlier
		"push %ebx \n"
		// call free()
		"call *%esi"
	);

	// we already overwrote the RET instruction at the end of this function
	// with an INT 3, so at this point the injector will regain control of
	// the target's execution.
}

/*
 * injectSharedLibrary_end()
 *
 * This function's only purpose is to be contiguous to injectSharedLibrary(),
 * so that we can use its address to more precisely figure out how long
 * injectSharedLibrary() is.
 *
 */

void injectSharedLibrary_end()
{
}

#elif defined(MY_CPU_ARM) || defined(MY_CPU_ARM64)
void injectSharedLibrary(long mallocaddr, long freeaddr, long dlopenaddr)
{
	// r1 = address of raise()
	// r2 = address of malloc()
	// r3 = address of __libc_dlopen_mode()
	// r4 = address of free()
	// r5 = size of the path to the shared library we want to load
	//
	// unfortunately, each function call we make will wipe out these
	// register values, so in order to avoid losing the function addresses,
	// we need to save them on the stack.
	//
	// here's the sequence of calls we're going to make:
	//
	// * malloc() - allocate a buffer to store the path to the shared
	//   library we're injecting
	//
	// * raise() - raise a SIGTRAP signal to break into the target process
	//   so that we can check the return value of malloc() in order to know
	//   where to copy the shared library path to
	//
	// * __libc_dlopen_mode() - load the shared library
	//
	// * raise() - raise a SIGTRAP signal to break into the target process
	//   to check the return value of __libc_dlopen_mode() in order to see
	//   whether it succeeded
	//
	// * free() - free the buffer containing the path to the shared library
	//
	// * raise() - raise a SIGTRAP signal to break into the target process
	//   so that we can restore the parts of memory that we overwrote
	//
	// we need to push the addresses of the functions we want to call in
	// the reverse of the order we want to call them in (except for the
	// first call):

	asm("push {r1}");	// raise()
	asm("push {r4}");	// free()
	asm("push {r1}");	// raise()
	asm("push {r3}");	// __libc_dlopen_mode()
	asm("push {r1}");	// raise()

	// call malloc() to allocate a buffer to store the path to the shared
	// library to inject.
	asm(
		// choose the amount of memory to allocate with malloc() based
		// on the size of the path to the shared library passed via r5
		"mov r0, r5 \n"
		// call malloc(), whose address is already in r2
		"blx r2 \n"
		// copy the return value (which is in r0) into r5 so that it
		// doesn't get wiped out later
		"mov r5, r0"
	);

	// call raise(SIGTRAP) to get back control of the target.
	asm(
		// pop off the stack to get the address of raise()
		"pop {r1} \n"
		// specify SIGTRAP as the first argument
		"mov r0, #5 \n"
		// call raise()
		"blx r1"
	);

	// call __libc_dlopen_mode() to actually load the shared library.
	asm(
		// pop off the stack to get the address of __libc_dlopen_mode()
		"pop {r2} \n"
		// copy r5 (the address of the malloc'd buffer) into r0 to make
		// it the first argument to __libc_dlopen_mode()
		"mov r0, r5 \n"
		// set the second argument to RTLD_LAZY
		"mov r1, #1 \n"
		// call __libc_dlopen_mode()
		"blx r2 \n"
		// copy the return value (which is in r0) into r4 so that it
		// doesn't get wiped out later
		"mov r4, r0"
	);

	// call raise(SIGTRAP) to get back control of the target.
	asm(
		// pop off the stack to get the address of raise()
		"pop {r1} \n"
		// specify SIGTRAP as the first argument
		"mov r0, #5 \n"
		// call raise()
		"blx r1"
	);

	// call free() in order to free the buffer containing the path to the
	// shared library.
	asm(
		// pop off the stack to get the address of free()
		"pop {r2} \n"
		// copy r5 (the malloc'd buffer) into r0 to make it the first
		// argument to free()
		"mov r0, r5 \n"
		// call __libc_dlopen_mode()
		"blx r2 \n"
		// copy return value r0 into r4 so that it doesn't get wiped
		// out later
		"mov r4, r0"
	);

	// call raise(SIGTRAP) to get back control of the target.
	asm(
		// pop off the stack to get the address of raise()
		"pop {r1} \n"
		// specify SIGTRAP as the first argument
		"mov r0, #5 \n"
		// call raise()
		"blx r1"
	);
}

void injectSharedLibrary_end()
{
}
#else
void injectSharedLibrary(long mallocaddr, long freeaddr, long dlopenaddr)
{
}
void injectSharedLibrary_end()
{
}
#endif
