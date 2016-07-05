#include "PolyHook_c_version.h"

PLH_ALL_S_t PLH_ALL_S_t_new()
{
	void * t = ((void*)(malloc(sizeof(PLH_ALL_S))));
	if(t==NULL)
	{
		return NULL;
	}
	memset(t,0,sizeof(PLH_ALL_S));
	return (PLH_ALL_S_t)t;
}

BOOL PLH_ALL_S_t_delete(PLH_ALL_S_t p)
{
	if(p==NULL)
		return FALSE;
	memset((void*)p,0,sizeof(PLH_ALL_S));
	free((void*)p);
	return TRUE;
}

int8_t PLH__ASMHelper__GetDisplacementType(PLH_ALL_S_t a, const uint8_t DispVal)
{
	switch (DispVal)
	{
		case PLH__ASMHelper__D_INT8:
			return PLH__ASMHelper__D_INT8;
		case PLH__ASMHelper__D_INT16:
			return PLH__ASMHelper__D_INT16;
		case PLH__ASMHelper__D_INT32:
			return PLH__ASMHelper__D_INT32;
		case PLH__ASMHelper__D_INT64:
			return PLH__ASMHelper__D_INT64;
		default:
			return PLH__ASMHelper__D_INVALID;
	}
}

BOOL PLH__ASMHelper__IsConditionalJump(PLH_ALL_S_t a, const BYTE* bytes,const uint16_t Size)
{
	//http://unixwiz.net/techtips/x86-jumps.html
	if (Size < 1)
		return TRUE;

	if (bytes[0] == 0x0F && Size > 1)
	{
		if (bytes[1] >= 0x80 && bytes[1] <= 0x8F)
			return TRUE;
	}

	if (bytes[0] >= 0x70 && bytes[0] <= 0x7F)
		return TRUE;

	if (bytes[0] == 0xE3)
		return TRUE;

	return FALSE;
}

int8_t PLH__ASMHelper__GetDisplacement__int8_t(PLH_ALL_S_t a, BYTE* Instruction, const uint32_t Offset)
{
	int8_t Disp;
	memset(&Disp, 0x00, sizeof(int8_t));
	memcpy(&Disp, &Instruction[Offset], sizeof(int8_t));
	return Disp;
}

int16_t PLH__ASMHelper__GetDisplacement__int16_t(PLH_ALL_S_t a, BYTE* Instruction, const uint32_t Offset)
{
	int16_t Disp;
	memset(&Disp, 0x00, sizeof(int16_t));
	memcpy(&Disp, &Instruction[Offset], sizeof(int16_t));
	return Disp;
}

int32_t PLH__ASMHelper__GetDisplacement__int32_t(PLH_ALL_S_t a, BYTE* Instruction, const uint32_t Offset)
{
	int32_t Disp;
	memset(&Disp, 0x00, sizeof(int32_t));
	memcpy(&Disp, &Instruction[Offset], sizeof(int32_t));
	return Disp;
}

BOOL PLH__MyDetour__GetNeedFree(PLH_ALL_S_t a)
{
	if(a==0)
		return 0;
	return a->plhAbstractDetourS.m_NeedFree;
}

void PLH__MyDetour__SetNeedFree(PLH_ALL_S_t a,BOOL needFree)
{
	if(a==0)
		return;
	a->plhAbstractDetourS.m_NeedFree=needFree;
}

void PLH__MyDetour__SetHookType(PLH_ALL_S_t a,PLH__ASMHelper__HookType hookType)
{
	if(a==0)
		return;
	a->plhAbstractDetourS.plhAsmHelperHookType=hookType;
}

PLH__ASMHelper__HookType PLH__MyDetour__GetHookType(PLH_ALL_S_t a)
{
	if(a==0)
		return 0;
	return a->plhAbstractDetourS.plhAsmHelperHookType;
}

BYTE * PLH__MyDetour__GetTrampoline(PLH_ALL_S_t a)
{
	if(a==0)
		return 0;
	return a->plhAbstractDetourS.m_Trampoline;
}

void PLH__MyDetour__SetTrampoline(PLH_ALL_S_t a,BYTE*trampoline)
{
	if(a==0)
		return;
	a->plhAbstractDetourS.m_Trampoline=trampoline;
}


BYTE * PLH__MyDetour__GetOriginalCode(PLH_ALL_S_t a)
{
	if(a==0)
		return 0;
	return (BYTE*)&a->plhAbstractDetourS.m_OriginalCode[0];
}

void PLH__MyDetour__SetOriginalCode(PLH_ALL_S_t a,BYTE*originalcode)
{
	if(a==0)
		return;
	if(originalcode==NULL)
		return;
	
	memcpy((void*)&a->plhAbstractDetourS.m_OriginalCode[0],(void*)originalcode,64);
}

BYTE * PLH__MyDetour__GetHkSrc(PLH_ALL_S_t a)
{
	if(a==0)
		return 0;
	return a->plhAbstractDetourS.m_hkSrc;
}

void PLH__MyDetour__SetHkSrc(PLH_ALL_S_t a,BYTE*hkSrc)
{
	if(a==0)
		return;
	a->plhAbstractDetourS.m_hkSrc=hkSrc;
}

BYTE * PLH__MyDetour__GetHkDst(PLH_ALL_S_t a)
{
	if(a==0)
		return 0;
	return a->plhAbstractDetourS.m_hkDst;
}

void PLH__MyDetour__SetHkDst(PLH_ALL_S_t a,BYTE*hkDst)
{
	if(a==0)
		return;
	a->plhAbstractDetourS.m_hkDst=hkDst;
}

SIZE_T PLH__MyDetour__GetHkLength(PLH_ALL_S_t a)
{
	if(a==0)
		return 0;
	return a->plhAbstractDetourS.m_hkLength;
}

void PLH__MyDetour__SetHkLength(PLH_ALL_S_t a,SIZE_T hkLength)
{
	if(a==0)
		return;
	a->plhAbstractDetourS.m_hkLength=hkLength;
}

SIZE_T PLH__MyDetour__GetOriginalLength(PLH_ALL_S_t a)
{
	if(a==0)
		return 0;
	return a->plhAbstractDetourS.m_OriginalLength;
}

void PLH__MyDetour__SetOriginalLength(PLH_ALL_S_t a,SIZE_T originalLength)
{
	if(a==0)
		return;
	a->plhAbstractDetourS.m_OriginalLength=originalLength;
}

csh PLH__MyDetour__GetCapstoneHandleValue(PLH_ALL_S_t a)
{
	if(a==0)
		return 0;
	return a->plhAbstractDetourS.m_CapstoneHandle;
}

void PLH__MyDetour__SetCapstoneHandleValue(PLH_ALL_S_t a,csh capstoneHandle)
{
	if(a==0)
		return;
	a->plhAbstractDetourS.m_CapstoneHandle=capstoneHandle;
}

HANDLE PLH__MyDetour__GetCapstoneHandle(PLH_ALL_S_t a)
{
	if(a==0)
		return 0;
	return (HANDLE)&a->plhAbstractDetourS.m_CapstoneHandle;
}

void PLH__MyDetour__SetCapstoneHandle(PLH_ALL_S_t a,HANDLE capstoneHandle)
{
	if(a==0)
		return;
	a->plhAbstractDetourS.m_CapstoneHandle=*((csh*)capstoneHandle);
}

x86_reg PLH__AbstractDetour__GetIpReg(PLH_ALL_S_t a)
{
	#ifdef IS_X64
		return PLH__X64Detour__GetIpReg(a);
	#else
		return PLH__X86Detour__GetIpReg(a);
	#endif
}

void PLH__AbstractDetour__RelocateASM(PLH_ALL_S_t a,BYTE* Code, SIZE_T * CodeSize, DWORD64 From, DWORD64 To)
{
	cs_insn* InstructionInfo;
	size_t InstructionCount = cs_disasm(PLH__MyDetour__GetCapstoneHandleValue(a), Code, ((size_t)(*CodeSize)), (uint64_t)Code, 0, &InstructionInfo);

	//XTrace("\nTrampoline:\n");
	size_t i;
	size_t j;
	for (i = 0; i < InstructionCount; i++)
	{
		cs_insn* CurIns = (cs_insn*)&InstructionInfo[i];
		cs_x86* x86 = &(CurIns->detail->x86);

		//XTrace("%I64X: ", CurIns->address);
		//for (int j = 0; j < CurIns->size; j++)
			//XTrace("%02X ", CurIns->bytes[j]);
		//XTrace("%s %s\n", CurIns->mnemonic,CurIns->op_str);
		for (j = 0; j < x86->op_count; j++)
		{
			cs_x86_op* op = &(x86->operands[j]);
			if (op->type == X86_OP_MEM)
			{
				//MEM are types like lea rcx,[rip+0xdead]
				if (op->mem.base == X86_REG_INVALID)
					continue;

				//Are we relative to instruction pointer?
				if (op->mem.base != PLH__AbstractDetour__GetIpReg(a))
					continue;

				PLH__AbstractDetour__Relocate(a, CurIns, From, To, x86->offsets.displacement_size, x86->offsets.displacement_offset);
			}else if (op->type == X86_OP_IMM) {
				//IMM types are like call 0xdeadbeef
				if (x86->op_count > 1) //exclude types like sub rsp,0x20
					continue;

				
				char* mnemonic = CurIns->mnemonic;
				if (PLH__ASMHelper__IsConditionalJump(a, CurIns->bytes,CurIns->size))
				{
					PLH__AbstractDetour__RelocateConditionalJMP(a, CurIns, CodeSize, From, To, x86->offsets.imm_size, x86->offsets.imm_offset);
					continue;
				}

				//types like push 0x20 slip through, check mnemonic
				if (strcmp(mnemonic, "call") != 0 && strcmp(mnemonic, "jmp") != 0) //probably more types than just these, update list as they're found
					continue;

				PLH__AbstractDetour__Relocate(a, CurIns, From, To, x86->offsets.imm_size, x86->offsets.imm_offset);
			}
		}
	}

	//XTrace("\nFixed Trampoline\n");
	InstructionCount = cs_disasm(PLH__MyDetour__GetCapstoneHandleValue(a), Code, (size_t)(*CodeSize), (uint64_t)Code, 0, &InstructionInfo);
	for (i = 0; i < InstructionCount; i++)
	{
		cs_insn* CurIns = (cs_insn*)&InstructionInfo[i];

		//XTrace("%I64X: ", CurIns->address);
		//for (int j = 0; j < CurIns->size; j++)
			//XTrace("%02X ", CurIns->bytes[j]);
		//XTrace("%s %s\n", CurIns->mnemonic, CurIns->op_str);
	}
	cs_free(InstructionInfo, InstructionCount);
}

void PLH__AbstractDetour__Relocate(PLH_ALL_S_t a,cs_insn* CurIns, DWORD64 From, DWORD64 To, const uint8_t DispSize, const uint8_t DispOffset)
{
	//XTrace("Relocating...\n");

	int8_t DispType = PLH__ASMHelper__GetDisplacementType(a,DispSize);
	if (DispType == PLH__ASMHelper__D_INT8)
	{
		int8_t Disp = PLH__ASMHelper__GetDisplacement__int8_t(a,CurIns->bytes, DispOffset);
		Disp -= (To - From);
		*(int8_t*)(CurIns->address + DispOffset) = Disp;
	}else if (DispType == PLH__ASMHelper__D_INT16) {
		int16_t Disp = PLH__ASMHelper__GetDisplacement__int16_t(a,CurIns->bytes, DispOffset);
		Disp -= (To - From);
		*(int16_t*)(CurIns->address + DispOffset) = Disp;
	}else if (DispType == PLH__ASMHelper__D_INT32) {
		int32_t Disp = PLH__ASMHelper__GetDisplacement__int32_t(a,CurIns->bytes, DispOffset);
		Disp -= (To - From);
		*(int32_t*)(CurIns->address + DispOffset) = Disp;
	}
}

void PLH__MemoryProtect__SetAddress(PLH_ALL_S_t a,void * Address)
{
	if(a==0)
		return;
	a->plhMemoryProtectS.m_Address = Address;
}

void * PLH__MemoryProtect__GetAddress(PLH_ALL_S_t a)
{
	if(a==0)
		return 0;
	return a->plhMemoryProtectS.m_Address;
}

void PLH__MemoryProtect__SetSize(PLH_ALL_S_t a,SIZE_T size)
{
	if(a==0)
		return;
	a->plhMemoryProtectS.m_Size = size;
}

SIZE_T PLH__MemoryProtect__GetSize(PLH_ALL_S_t a)
{
	if(a==0)
		return 0;
	return a->plhMemoryProtectS.m_Size;
}

void PLH__MemoryProtect__SetFlags(PLH_ALL_S_t a,DWORD flags)
{
	if(a==0)
		return;
	a->plhMemoryProtectS.m_Flags = flags;
}

DWORD PLH__MemoryProtect__GetFlags(PLH_ALL_S_t a)
{
	if(a==0)
		return 0;
	return a->plhMemoryProtectS.m_Flags;
}

void PLH__MemoryProtect__SetOldProtection(PLH_ALL_S_t a,DWORD oldProtection)
{
	if(a==0)
		return;
	a->plhMemoryProtectS.m_OldProtection = oldProtection;
}

DWORD PLH__MemoryProtect__GetOldProtection(PLH_ALL_S_t a)
{
	if(a==0)
		return 0;
	return a->plhMemoryProtectS.m_OldProtection;
}

void PLH__MemoryProtect__init(PLH_ALL_S_t a,void* Address, SIZE_T Size, DWORD ProtectionFlags)
{
	if(a==0)
		return;
	a->plhMemoryProtectS.m_Address = Address;
	a->plhMemoryProtectS.m_Size = Size;
	a->plhMemoryProtectS.m_Flags = ProtectionFlags;
	my_memory_holder * h=(my_memory_holder*)&a->extraData[0];
	if(h->extraData[sizeof(my_memory_holder)-1]==1&&h->extraData[sizeof(my_memory_holder)-2]==1)
	{
		unsigned long long x = (unsigned long long)(((unsigned long long)a->plhMemoryProtectS.m_Address)+((unsigned long long)a->plhMemoryProtectS.m_Size));
        	unsigned long long y = (unsigned long long)(((unsigned long long)h->my.mbi.AllocationBase)+((unsigned long long)h->my.mbi.RegionSize));
        	unsigned long long z = (unsigned long long)((unsigned long long)h->my.mbi.AllocationBase);
        	if((((unsigned long long)x)>((unsigned long long)z))&&(((unsigned long long)x)<((unsigned long long)y)))
		{
			a->plhMemoryProtectS.m_Address = NULL;
			return;
		}
	}

	PLH__MemoryProtect__Protect(a,a->plhMemoryProtectS.m_Address, a->plhMemoryProtectS.m_Size, a->plhMemoryProtectS.m_Flags);
}

BOOL PLH__MemoryProtect__Protect(PLH_ALL_S_t a,void* Address, SIZE_T Size, DWORD ProtectionFlags)
{
	if(a==0)
		return FALSE;
	DWORD oldPr;
	BOOL ret = VirtualProtect(Address, Size, ProtectionFlags, (PDWORD)&oldPr);
	a->plhMemoryProtectS.m_OldProtection=oldPr;
	return ret;
}

void PLH__MemoryProtect__deinit(PLH_ALL_S_t a)
{
	if(a==0)
		return;
	if(a->plhMemoryProtectS.m_Address==NULL)
		return;
	void * address = a->plhMemoryProtectS.m_Address;
	SIZE_T size =  a->plhMemoryProtectS.m_Size;
	DWORD oldPr = a->plhMemoryProtectS.m_OldProtection;
	PLH__MemoryProtect__Protect(a,address,size,oldPr);
}

void PLH__AbstractDetour__Initialize(PLH_ALL_S_t a,cs_mode Mode)
{
	if(a==0)
		return;
	a->last_error=0;
	if ((a->last_error = cs_open(CS_ARCH_X86, Mode, ((csh*)(PLH__MyDetour__GetCapstoneHandle(a))))) != CS_ERR_OK)
	{	
		
		return;
		//XTrace("Error Initializing Capstone x86\n");
	}

	cs_option(PLH__MyDetour__GetCapstoneHandleValue(a), CS_OPT_DETAIL, CS_OPT_ON);
}

SIZE_T PLH__AbstractDetour__CalculateLength(PLH_ALL_S_t a,BYTE* Src, SIZE_T NeededLength)
{
	//Grab First 100 bytes of function, disasm until invalid instruction
	cs_insn* InstructionInfo;
	size_t InstructionCount = (size_t)cs_disasm(PLH__MyDetour__GetCapstoneHandleValue(a), Src, 0x100, (uint64_t)Src, 0, &InstructionInfo);

	//Loop over instructions until we have at least NeededLength's Size
	//XTrace("\nORIGINAL:\n");
	SIZE_T InstructionSize = 0;
	bool BigEnough = false;
	int i;
	for (i = 0; i < InstructionCount && !BigEnough; i++)
	{
		cs_insn* CurIns = (cs_insn*)&InstructionInfo[i];
		InstructionSize += CurIns->size;
		if (InstructionSize >= NeededLength)
			BigEnough = true;

		//XTrace("%I64X [%d]: ", CurIns->address, CurIns->size);
		//for (int j = 0; j < CurIns->size; j++)
			//XTrace("%02X ", CurIns->bytes[j]);
		//XTrace("%s %s\n", CurIns->mnemonic, CurIns->op_str);
	}
	if (!BigEnough)
		InstructionSize = 0;

	cs_free(InstructionInfo, InstructionCount);
	return InstructionSize;
}


long PLH__AbstractDetour__CalculateRelativeDisplacement__long(PLH_ALL_S_t a,DWORD64 From, DWORD64 To, SIZE_T InsSize)
{
	if (To < From)
		return 0 - (From - To) - InsSize;
	return To - (From + InsSize);
}

int8_t PLH__AbstractDetour__CalculateRelativeDisplacement__int8_t(PLH_ALL_S_t a,DWORD64 From, DWORD64 To, SIZE_T InsSize)
{
	if (To < From)
		return 0 - (From - To) - InsSize;
	return To - (From + InsSize);
}

int16_t PLH__AbstractDetour__CalculateRelativeDisplacement__int16_t(PLH_ALL_S_t a,DWORD64 From, DWORD64 To, SIZE_T InsSize)
{
	if (To < From)
		return 0 - (From - To) - InsSize;
	return To - (From + InsSize);
}

int32_t PLH__AbstractDetour__CalculateRelativeDisplacement__int32_t(PLH_ALL_S_t a,DWORD64 From, DWORD64 To, SIZE_T InsSize)
{
	if (To < From)
		return 0 - (From - To) - InsSize;
	return To - (From + InsSize);
}

void PLH__AbstractDetour__WriteJMP(PLH_ALL_S_t a,DWORD_PTR From, DWORD_PTR To)
{
	#ifdef IS_X64
		PLH__X64Detour__WriteJMP(a,From,To);
	#else
		PLH__X86Detour__WriteJMP(a,From,To);
	#endif
}

int PLH__AbstractDetour__GetJMPSize(PLH_ALL_S_t a)
{
	#ifdef IS_X64
		return PLH__X64Detour__GetJMPSize(a);
	#else
		return PLH__X86Detour__GetJMPSize(a);
	#endif
}
		
void PLH__AbstractDetour__RelocateConditionalJMP(PLH_ALL_S_t a,cs_insn* CurIns, SIZE_T * CodeSize, DWORD64 From, DWORD64 To, const uint8_t DispSize, const uint8_t DispOffset)
{
	/*This function automatically begins to build a jump table at the end of the trampoline to allow relative jumps to function properly:
	-Changes relative jump to point to an absolute jump
	-Absolute jump then does the long distance to jump to where the relative jump originally went
	*/
	int8_t DispType = PLH__ASMHelper__GetDisplacementType(a,DispSize);
	DWORD64 TrampolineEnd = To + *CodeSize;
	if (DispType == PLH__ASMHelper__D_INT8)
	{
		int8_t Disp = PLH__ASMHelper__GetDisplacement__int8_t(a,CurIns->bytes, DispOffset);
		DWORD64 OriginalDestination = CurIns->address + (Disp - (To - From)) + CurIns->size;
		PLH__AbstractDetour__WriteJMP(a, TrampolineEnd, OriginalDestination);
		Disp = PLH__AbstractDetour__CalculateRelativeDisplacement__int8_t(a,CurIns->address, (DWORD64)TrampolineEnd, CurIns->size); //set relative jmp to go to our absolute
		*(int8_t*)(CurIns->address + DispOffset) = Disp;
		*CodeSize += PLH__AbstractDetour__GetJMPSize(a);
	}else if (DispType == PLH__ASMHelper__D_INT16) {
		int16_t Disp = Disp = PLH__ASMHelper__GetDisplacement__int16_t(a,CurIns->bytes, DispOffset);
		DWORD64 OriginalDestination = CurIns->address + (Disp - (To - From)) + CurIns->size;
		PLH__AbstractDetour__WriteJMP(a,TrampolineEnd, OriginalDestination);
		Disp = PLH__AbstractDetour__CalculateRelativeDisplacement__int16_t(a,CurIns->address, (DWORD64)TrampolineEnd, CurIns->size);
		*(int16_t*)(CurIns->address + DispOffset) = Disp;
		*CodeSize += PLH__AbstractDetour__GetJMPSize(a);
	}else if (DispType == PLH__ASMHelper__D_INT32) {
		int32_t Disp = PLH__ASMHelper__GetDisplacement__int32_t(a,CurIns->bytes, DispOffset);
		DWORD64 OriginalDestination = CurIns->address + (Disp - (To - From)) + CurIns->size;
		PLH__AbstractDetour__WriteJMP(a,TrampolineEnd, OriginalDestination);
		Disp = PLH__AbstractDetour__CalculateRelativeDisplacement__int32_t(a,CurIns->address, (DWORD64)TrampolineEnd, CurIns->size);
		*(int32_t*)(CurIns->address + DispOffset) = Disp;
		*CodeSize += PLH__AbstractDetour__GetJMPSize(a);
	}
}

void PLH__AbstractDetour__SetupHook(PLH_ALL_S_t a,BYTE* Src, BYTE* Dest)
{
	printf("%16x\n",(unsigned long long)Src);
	PLH__MyDetour__SetHkSrc(a,Src);
	PLH__MyDetour__SetHkDst(a,Dest);
}

void PLH__AbstractDetour__FlushSrcInsCache(PLH_ALL_S_t a)
{
	FlushInstructionCache(GetCurrentProcess(), PLH__MyDetour__GetHkSrc(a), PLH__MyDetour__GetHkLength(a));
}

void PLH__AbstractDetour__FreeTrampoline(PLH_ALL_S_t a)
{
	#ifdef IS_X64
		PLH__X64Detour__FreeTrampoline(a);
	#else
		PLH__X86Detour__FreeTrampoline(a);
	#endif
}


BOOL PLH__AbstractDetour__Hook(PLH_ALL_S_t a)
{
	#ifdef IS_X64
		return PLH__X64Detour__Hook(a);
	#else 
		return PLH__X86Detour__Hook(a);
	#endif
}

void PLH__AbstractDetour__UnHook(PLH_ALL_S_t a)
{
	PLH__MemoryProtect__init(a,PLH__MyDetour__GetHkSrc(a), PLH__MyDetour__GetHkLength(a), PAGE_EXECUTE_READWRITE);
	memcpy(PLH__MyDetour__GetHkSrc(a), PLH__MyDetour__GetOriginalCode(a), PLH__MyDetour__GetOriginalLength(a)); //Copy original from trampoline back to src
	PLH__AbstractDetour__FlushSrcInsCache(a);
	PLH__AbstractDetour__FreeTrampoline(a);
	PLH__MemoryProtect__deinit(a);
}

PLH__AbstractDetour__init(PLH_ALL_S_t a)
{
#ifdef IS_X64
	PLH__AbstractDetour__Initialize(a,CS_MODE_64);
#else
	PLH__AbstractDetour__Initialize(a,CS_MODE_32);
#endif // _WIN64
}

PLH__AbstractDetour__deinit(PLH_ALL_S_t a)
{
	cs_close(((csh*)(PLH__MyDetour__GetCapstoneHandle(a))));
}

void PLH__MyDetour__init(PLH_ALL_S_t a)
{
	if(a==0)
	{
		return;
	}
	memset(a,0,sizeof(PLH_ALL_S));
	PLH__AbstractDetour__init(a);
}

void PLH__MyDetour__deinit(PLH_ALL_S_t a)
{
	if(a==0)
	{
		return;
	}
	PLH__AbstractDetour__deinit(a);
	memset(a,0,sizeof(PLH_ALL_S));
}

void PLH__X86Detour__WriteRelativeJMP(PLH_ALL_S_t a,DWORD Destination, DWORD JMPDestination)
{
	*(BYTE*)Destination = 0xE9;       //Write jump opcode to jump back to non overwritten code
	*(long*)(Destination + 1) = PLH__AbstractDetour__CalculateRelativeDisplacement__long(a, Destination, JMPDestination, 5);
}

void PLH__X86Detour__WriteJMP(PLH_ALL_S_t a,DWORD_PTR From, DWORD_PTR To)
{
	PLH__X86Detour__WriteRelativeJMP(a, From, To);
}

int PLH__X86Detour__GetJMPSize(PLH_ALL_S_t a)
{
	return 5;
}

void PLH__X86Detour__init(PLH_ALL_S_t a)
{

}

void PLH__X86Detour__deinit(PLH_ALL_S_t a)
{
	PLH__X86Detour__FreeTrampoline(a);
}

PLH__ASMHelper__HookType PLH__X86Detour__GetType(PLH_ALL_S_t a)
{
	return X86Detour;
}

SIZE_T * PLH__MyDetour__GetHkLengthP(PLH_ALL_S_t a)
{
	if(a==0)
		return 0;
	return ((SIZE_T*)(&(a->plhAbstractDetourS.m_hkLength)));
}

SIZE_T * PLH__MyDetour__GetOriginalLengthP(PLH_ALL_S_t a)
{
	if(a==0)
		return 0;
	 return ((SIZE_T*)(&(a->plhAbstractDetourS.m_OriginalLength)));
}

BOOL PLH__X86Detour__Hook(PLH_ALL_S_t a)
{
	if(a==0)
		return FALSE;
	DWORD OldProtection;

	PLH__MyDetour__SetHkLength(a, PLH__AbstractDetour__CalculateLength(a,PLH__MyDetour__GetHkSrc(a), 5));
	PLH__MyDetour__SetOriginalLength(a,PLH__MyDetour__GetHkLength(a));
	if (PLH__MyDetour__GetHkLength(a) == 0)
	{
		//XTrace("Function to small to hook\n");
		return FALSE;
	}
    
	PLH__MyDetour__SetTrampoline(a,(BYTE*)malloc(((PLH__MyDetour__GetHkLength(a)) + 30)));   //Allocate Space for original plus extra to jump back and for jmp table
	if(!PLH__MyDetour__GetTrampoline(a))
		return FALSE;
	PLH__MyDetour__SetNeedFree(a,TRUE);
	DWORD oldPr;
	VirtualProtect(PLH__MyDetour__GetTrampoline(a), ((PLH__MyDetour__GetHkLength(a)) + 5), PAGE_EXECUTE_READWRITE, (PDWORD)&oldPr); //Allow Execution
	a->plhMemoryProtectS.m_OldProtection=oldPr;
	memcpy(PLH__MyDetour__GetOriginalCode(a), PLH__MyDetour__GetHkSrc(a), PLH__MyDetour__GetHkLength(a));
	memcpy(PLH__MyDetour__GetTrampoline(a), PLH__MyDetour__GetHkSrc(a), PLH__MyDetour__GetHkLength(a)); //Copy original into allocated space
	PLH__AbstractDetour__RelocateASM(a,PLH__MyDetour__GetTrampoline(a), PLH__MyDetour__GetHkLengthP(a), (DWORD)PLH__MyDetour__GetHkSrc(a), (DWORD)PLH__MyDetour__GetTrampoline(a));
	PLH__X86Detour__WriteRelativeJMP(a,(DWORD)&((PLH__MyDetour__GetTrampoline(a))[(PLH__MyDetour__GetHkLength(a))]), (DWORD)((PLH__MyDetour__GetHkSrc(a)) + (PLH__MyDetour__GetHkLength(a)))); //JMP back to original code

	//Change protection to allow write on original function
	PLH__MemoryProtect__init(a, PLH__MyDetour__GetHkSrc(a), PLH__MyDetour__GetHkLength(a), PAGE_EXECUTE_READWRITE);
	//Encode Jump from Hooked Function to the Destination function
	PLH__X86Detour__WriteRelativeJMP(a,(DWORD)PLH__MyDetour__GetHkSrc(a), (DWORD)PLH__MyDetour__GetHkDst(a));

	//Write nops over bytes of overwritten instructions
	int length = PLH__MyDetour__GetOriginalLength(a);
	BYTE * ps = (BYTE*)PLH__MyDetour__GetHkSrc(a);
	int i;
	for (i = 5; i < length; i++)
		ps[i] = 0x90;
	PLH__AbstractDetour__FlushSrcInsCache(a);
	PLH__MemoryProtect__deinit(a);
	//PostError(RuntimeError(RuntimeError::Severity::Warning, "PolyHook x86Detour: Some opcodes may not be relocated properly"));
	return TRUE;
	/*Original
	-JMP Destination
	-NOP (extends to length of overwritten opcode)
	-Rest of function

	Destination
	-Do your shit
	-Return Trampoline (goes to trampoline)

	Trampoline
	-Execute Overwritten Opcodes
	-Patch original relative jmps to point to jump table (JE Jumptable entry 1)
	-JMP to rest of function (in original)
	-*BEGIN JUMPTABLE*
	-1)JMP to location of relative jmp one
	-2)...continue pattern for all relative jmps
	*/
}

x86_reg PLH__X86Detour__GetIpReg(PLH_ALL_S_t a)
{
	return X86_REG_EIP;
}

void PLH__X86Detour__FreeTrampoline(PLH_ALL_S_t a)
{
	if (PLH__MyDetour__GetNeedFree(a))
	{
		free(PLH__MyDetour__GetTrampoline(a));
	    PLH__MyDetour__SetNeedFree(a,0);
		PLH__MyDetour__SetTrampoline(a,0);
	}
}

void PLH__X64Detour__init(PLH_ALL_S_t a)
{

}

void PLH__X64Detour__deinit(PLH_ALL_S_t a)
{
	PLH__X64Detour__FreeTrampoline(a);
}

PLH__ASMHelper__HookType PLH__X64Detour__GetType()
{
	return X64Detour;
}

BOOL PLH__X64Detour__Hook(PLH_ALL_S_t a)
{
	if(a==NULL)
		return FALSE;
	//Allocate Memory as close as possible to src, to minimize chance 32bit displacements will be out of range (for relative jmp type)
	MEMORY_BASIC_INFORMATION mbi;
	#ifdef OS_UNIX_STRUCT
		MEMORY_BASIC_INFORMATION nbi;
		BOOL sset = FALSE;
		DWORD oldPr;
		unsigned long long myAddr=0;
		unsigned long long myAddrOriginal=0;
		BYTE*naddress;
		#define HOOK_REGION_SIZE 0x1000
		#define HOOK_ADD_TO_ADDRESS 1
	#endif
	size_t Addr;
	#if defined(OS_UNIX_STRUCT)
	for (myAddr = (unsigned long long)PLH__MyDetour__GetHkSrc(a),myAddrOriginal=myAddr; (((unsigned long long)(myAddr-myAddrOriginal))<(unsigned long long)0x80000000); myAddr=(((unsigned long long)mbi.AllocationBase) + ((unsigned long long)mbi.RegionSize)),myAddr+=(myAddr<myAddrOriginal)?((unsigned long long)4096):((unsigned long long)HOOK_ADD_TO_ADDRESS))
	#elif defined(OS_WIN)
	for (Addr = (size_t)PLH__MyDetour__GetHkSrc(a); Addr > (size_t)(PLH__MyDetour__GetHkSrc(a) - 0x80000000); Addr = (size_t)((mbi.BaseAddress) - 1))
	#else
	for(Addr=(size_t)0; Addr != (size_t)0; Addr = (size_t)0)
	#endif
	{
		#ifdef OS_UNIX_STRUCT
			printf("aa: %016x\n",myAddr);
			printf("bb: %016x\n",myAddrOriginal);
			memset((void*)&mbi,0,sizeof(MEMORY_BASIC_INFORMATION));
			memset((void*)&nbi,0,sizeof(MEMORY_BASIC_INFORMATION));
			memset((void*)&a->extraData[0],0,1024);
			mbi.State=MEM_FREE;
			mbi.AllocationProtect=PAGE_WRITECOPY;
			sset=FALSE;
			if(!VirtualQueryUnix((LPCVOID)myAddr,&mbi,HOOK_REGION_SIZE))
			{
				printf("dd\n");
				break;
			}
			printf("ba: %016x\n",mbi.BaseAddress);
			printf("st: %d\n",mbi.State==MEM_FREE?1:0);
			printf("rs: %d\n",mbi.RegionSize);
			if(mbi.State!=MEM_FREE)
			{
				if(mbi.RegionSize<HOOK_REGION_SIZE)
				{
					printf("fuck off\n");
					continue;
				}
				//return (BOOL)mbi.RegionSize;
				sset=VirtualQueryUnixAdjustment((PMEMORY_BASIC_INFORMATION)&mbi,(PMEMORY_BASIC_INFORMATION)&nbi,HOOK_REGION_SIZE);
				if(sset==FALSE||nbi.State!=MEM_FREE)
 				{
					printf("fuck here off 1\n");
					continue;
				}
				sset=VirtualProtect((LPVOID)mbi.BaseAddress,mbi.RegionSize,PAGE_EXECUTE_READWRITE,&oldPr);
				if(sset==FALSE)
				{
					printf("fuck here off 2\n");
					continue;
				}
				if(nbi.BaseAddress==NULL)
				{
					printf("fuck here off 3\n");
					continue;
				}
				my_memory_holder * my = (my_memory_holder*)&a->extraData[0];
				memset((void*)my,0,sizeof(my_memory_holder));
				my->extraData[(sizeof(my_memory_holder)-1)]=1;
				memcpy((void*)&my->my.nbi,(void*)&nbi,sizeof(MEMORY_BASIC_INFORMATION));
				memcpy((void*)&my->my.mbi,(void*)&mbi,sizeof(MEMORY_BASIC_INFORMATION));
				memcpy((void*)&my->my.sset,(void*)&sset,sizeof(BOOL));
				memcpy((void*)&my->my.oldPr,(void*)&oldPr,sizeof(DWORD));
				if(myAddrOriginal==myAddr)
				{
					my->extraData[sizeof(my_memory_holder)-2]=1;
				}
				naddress=(BYTE*)nbi.BaseAddress;
			} else {
				if(mbi.RegionSize<HOOK_REGION_SIZE)
					continue;
				naddress=(BYTE*)VirtualAlloc(mbi.BaseAddress,HOOK_REGION_SIZE, MEM_RESERVE|MEM_COMMIT,PAGE_EXECUTE_READWRITE);
				if(naddress==NULL)
					continue;
			}
			printf("%016x\n", (unsigned long long)naddress); // gives 12ab
			PLH__MyDetour__SetTrampoline(a,naddress);
		#elif defined(OS_WIN)
			if (!VirtualQuery((LPCVOID)Addr, &mbi, sizeof(mbi)))
                                break;

                        if (mbi.State != MEM_FREE)
                                continue;

                        PLH__MyDetour__SetTrampoline(a,(BYTE*)VirtualAlloc(mbi.BaseAddress, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));
		#else
			break;
		#endif
		if (PLH__MyDetour__GetTrampoline(a))
			break;
	}
	if (!PLH__MyDetour__GetTrampoline(a))
	{
		printf("fuck me\n");
		return FALSE;
	}
	PLH__MyDetour__SetNeedFree(a,TRUE);

	//Decide which jmp type to use based on function size
	BOOL UseRelativeJmp = FALSE;
	PLH__MyDetour__SetHkLength(a,PLH__AbstractDetour__CalculateLength(a, PLH__MyDetour__GetHkSrc(a), 16)); //More stable 16 byte jmp
	PLH__MyDetour__SetOriginalLength(a, PLH__MyDetour__GetHkLength(a)); //We modify hkLength in Relocation routine
	if (PLH__MyDetour__GetHkLength(a) == 0)
	{
		UseRelativeJmp = TRUE;
		PLH__MyDetour__SetHkLength(a,PLH__AbstractDetour__CalculateLength(a, PLH__MyDetour__GetHkSrc(a), 6)); //Smaller, less safe 6 byte (jmp could be out of bounds)
		if (PLH__MyDetour__GetHkLength(a) == 0)
		{
			//PostError(RuntimeError(RuntimeError::Severity::UnRecoverable, "PolyHook x64Detour: Function to small to hook"));
			return FALSE;
		}
	}

	memcpy(PLH__MyDetour__GetOriginalCode(a), PLH__MyDetour__GetHkSrc(a), PLH__MyDetour__GetHkLength(a));
	memcpy(PLH__MyDetour__GetTrampoline(a), PLH__MyDetour__GetHkSrc(a), PLH__MyDetour__GetHkLength(a));
	PLH__AbstractDetour__RelocateASM(a, PLH__MyDetour__GetTrampoline(a), PLH__MyDetour__GetHkLengthP(a), (DWORD64)PLH__MyDetour__GetHkSrc(a), (DWORD64)PLH__MyDetour__GetTrampoline(a));
	//Write the jmp from our trampoline back to the original
	PLH__X64Detour__WriteAbsoluteJMP(a,(DWORD64)&((PLH__MyDetour__GetTrampoline(a))[PLH__MyDetour__GetHkLength(a)]), ((DWORD64)((PLH__MyDetour__GetHkSrc(a)) + (PLH__MyDetour__GetHkLength(a))))); 
	// Build a far jump to the Destination function. (jmps not to address pointed at but to the value in the address)
	PLH__MemoryProtect__init(a,((PLH__MyDetour__GetHkSrc(a))), ((PLH__MyDetour__GetHkLength(a))), PAGE_EXECUTE_READWRITE);
	int HookSize = 0;
	if (UseRelativeJmp)
	{
		//return (BOOL)4;
		HookSize = 6;
		((PLH__MyDetour__GetHkSrc(a))[0]) = 0xFF;
		((PLH__MyDetour__GetHkSrc(a))[1]) = 0x25;
		//Write 32Bit Displacement from rip
		*(long*)((PLH__MyDetour__GetHkSrc(a)) + 2) = PLH__AbstractDetour__CalculateRelativeDisplacement__long(a, (DWORD64)PLH__MyDetour__GetHkSrc(a), (DWORD64)&(((PLH__MyDetour__GetTrampoline(a))[((PLH__MyDetour__GetHkLength(a)) + 16)])), 6);
		*(DWORD64*)&((PLH__MyDetour__GetTrampoline(a))[((PLH__MyDetour__GetHkLength(a)) + 16)]) = (DWORD64)PLH__MyDetour__GetHkDst(a); //Write the address into memory at [RIP+Displacement]
	}else {
		//return (BOOL)9;
		HookSize = 16;
		PLH__X64Detour__WriteAbsoluteJMP(a,(DWORD64)((PLH__MyDetour__GetHkSrc(a))), (DWORD64)((PLH__MyDetour__GetHkDst(a))));
	}
	//Nop Extra bytes from overwritten opcode
	int i;
	int length = PLH__MyDetour__GetOriginalLength(a);
	BYTE * ps = (BYTE*)PLH__MyDetour__GetHkSrc(a);
	for (i = HookSize; i < length; i++)
		ps[i] = 0x90;
	FlushInstructionCache(GetCurrentProcess(), PLH__MyDetour__GetHkSrc(a), PLH__MyDetour__GetHkLength(a));
	PLH__MemoryProtect__deinit(a);
	//PostError(RuntimeError(RuntimeError::Severity::Warning, "PolyHook x64Detour: Relocation can be out of range"));
	return TRUE;
}

x86_reg PLH__X64Detour__GetIpReg(PLH_ALL_S_t a)
{
	return X86_REG_RIP;
}

void PLH__X64Detour__FreeTrampoline(PLH_ALL_S_t a)
{
	
	if (PLH__MyDetour__GetNeedFree(a))
	{
		#if defined(OS_WIN)
			VirtualFree(PLH__MyDetour__GetTrampoline(a), 0, MEM_RELEASE);
		#elif defined(OS_UNIX_STRUCT)
			my_memory_holder * my=(my_memory_holder*)&a->extraData[0];
			if(my->extraData[sizeof(my_memory_holder)-1]==1)
			{
				DWORD nOldPr;
				if(my->my.sset==TRUE)
				{
					VirtualFreeUnixX(PLH__MyDetour__GetTrampoline(a), 0, MEM_RELEASE, (PMEMORY_BASIC_INFORMATION)&my->my.nbi, (BOOL*)&my->my.sset);
					my->my.sset=VirtualProtect(my->my.mbi.BaseAddress,my->my.mbi.RegionSize,my->my.oldPr,(PDWORD)&nOldPr);
					my->my.oldPr=nOldPr;
				}
			} else {
				VirtualFree(PLH__MyDetour__GetTrampoline(a), 0, MEM_RELEASE);
			}
		#endif
		PLH__MyDetour__SetNeedFree(a,FALSE);

	}
}

void PLH__X64Detour__WriteAbsoluteJMP(PLH_ALL_S_t a,DWORD64 Destination, DWORD64 JMPDestination)
{
	/*push rax
	mov rax ...   //Address to original
	xchg qword ptr ss:[rsp], rax
	ret*/
	BYTE detour[] = { 0x50, 0x48, 0xB8, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x48, 0x87, 0x04, 0x24, 0xC3 };
	memcpy((BYTE*)Destination, detour, sizeof(detour));
	*(DWORD64*)&((BYTE*)Destination)[3] = JMPDestination;
}

void PLH__X64Detour__WriteJMP(PLH_ALL_S_t a,DWORD_PTR From, DWORD_PTR To)
{
	PLH__X64Detour__WriteAbsoluteJMP(a,From, To);
}

int PLH__X64Detour__GetJMPSize(PLH_ALL_S_t a)
{
	return 16;
}
