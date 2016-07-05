#ifndef MY_MHOOK_H
#define MY_MHOOK_H

#include "subhook/my_predef.h"
#ifndef OS_MAC
#include "mhook_predef.h"
#if defined(USE_SUBHOOK)
	#include "subhook/hookerpref.h"
	BOOL Mhook_SetHook(PVOID *ppSystemFunctionP, PVOID pHookFunction);
	BOOL Mhook_Unhook(PVOID *ppHookedFunctionP);
	#define MHOOKS_MAX_SUPPORTED_HOOKS	64
	//Old Hooks
	static subhook_t mhooks_subhooks[MHOOKS_MAX_SUPPORTED_HOOKS];
	static BOOL mhooks_bool_init = FALSE;
	static int mhooks_subhooks_count = 0;
#elif defined(USE_MHOOK)
	#include "cpu.h"
	#include "disasm_n.h"
	#include "mhook_lib/mhook-lib/mhook.h"
#elif defined(USE_POLYHOOK)
	#include "polyhook_lib/PolyHook/PolyHook_c_version.h"
	#define MHOOKS_MAX_SUPPORTED_HOOKS	64
	//Old Hooks
	static PLH_ALL_S_t mhooks_subhooks[MHOOKS_MAX_SUPPORTED_HOOKS];
	static int mhooks_subhooks_count = 0; 
	static BOOL mhooks_bool_init = FALSE;
	BOOL Mhook_SetHook(PVOID *ppSystemFunctionP, PVOID pHookFunction);
	BOOL Mhook_Unhook(PVOID *ppHookedFunctionP);
#else
	BOOL Mhook_SetHook(PVOID *ppSystemFunctionP, PVOID pHookFunction);
        BOOL Mhook_Unhook(PVOID *ppHookedFunctionP);
#endif
	void Mhook_MyInit();
	BOOL Mhook_SetHookEx(PVOID ppSystemFunction, PVOID pHookFunction);
	BOOL Mhook_UnhookEx(PVOID ppHookedFunction);
#else
#define MHOOKS_MAX_SUPPORTED_HOOKS      64
//Old Hooks
#include "mach_override/mach_override_overrides.h"
static mopc * mhooks_subhooks[MHOOKS_MAX_SUPPORTED_HOOKS];
static BOOL mhooks_subhooks_[MHOOKS_MAX_SUPPORTED_HOOKS];
static int mhooks_subhooks_count = 0;
static BOOL mhooks_bool_init = FALSE;
BOOL Mhook_SetHook(PVOID *ppSystemFunctionP, PVOID pHookFunction);
BOOL Mhook_Unhook(PVOID *ppHookedFunctionP);
void Mhook_MyInit();
BOOL Mhook_SetHookEx(PVOID ppSystemFunction, PVOID pHookFunction);
BOOL Mhook_UnhookEx(PVOID ppHookedFunction);
#endif
#ifndef MHOOK_SETHOOK_DEF
#define MHOOK_SETHOOK_DEF
#define MHOOK_SETHOOK(x,y) Mhook_SetHook(x,y)
#endif
#ifndef MHOOK_UNHOOK_DEF
#define MHOOK_UNHOOK_DEF
#define MHOOK_UNHOOK(x,y) Mhook_Unhook(x,y)
#endif
#endif
