#ifndef MACH_OVERRIDE_H_OVERRIDES
#define MACH_OVERRIDE_H_OVERRIDES
//#ifndef MACH_OVERRIDE_CPLUSPLUS
#ifdef  __cplusplus
 	extern  "C"     {
#endif
//#endif
	
#if defined(__ppc__) || defined(__POWERPC__)
#define TEMPLATE_SIZE ((sizeof(long)*7))
#ifndef TEMPLATE_SIZE
#define TEMPLATE_SIZE 28
#endif
#elif defined(__i386__) 
#define TEMPLATE_SIZE 21
#elif defined(__x86_64__)
#define TEMPLATE_SIZE 46
#endif

typedef	struct	{
	char	instructions[TEMPLATE_SIZE];
}	BranchIsland;

typedef union mopc_union_
{
	char oldInstructions[256];
	long oldInstruction;
} mopc_union;

typedef struct mopc_
{
	mopc_union oi;
	unsigned long long instruction_length;
	void *originalFunctionAddress;
	void *overrideFunctionAddress;
	void **originalFunctionReentryIsland;
	BranchIsland * re;
	BranchIsland * es;
	mach_error_t err;
} mopc;

#if defined(__ppc__) || defined(__POWERPC__) 
mach_error_t mach_override_ptr_ppc(mopc * oldinfo);
mach_error_t mach_unoverride_ptr_ppc(mopc * oldinfo);
#elif defined(__i386__)
mach_error_t mach_override_ptr_i386(mopc * oldinfo);
mach_error_t mach_unoverride_ptr_i386(mopc * oldinfo);
#elif defined(__x86_64__)
mach_error_t mach_override_ptr_x86_64(mopc * oldinfo);
mach_error_t mach_unoverride_ptr_x86_64(mopc * oldinfo);
#endif
mach_error_t mach_override_ptr_my(mopc * oldinfo);
mach_error_t mach_unoverride_ptr_my(mopc * oldinfo);
mach_error_t mach_override_ptr_old_my(void *originalFunctionAddress,const void *overrideFunctionAddress,void **originalFunctionReentryIsland );

//#ifndef MACH_OVERRIDE_CPLUSPLUS
#ifdef  __cplusplus
 	}
#endif
//#endif

#endif
