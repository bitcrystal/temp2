// Copyright (C) 2004, Matt Conover (mconover@gmail.com)
#ifndef DISASM_H
#define DISASM_H
#ifdef __cplusplus
extern "C" {
#endif
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include "misc.h"
#include "cpu.h"

typedef signed char S8;
typedef unsigned char U8;
typedef signed short S16;
typedef unsigned short U16;
typedef signed long S32;
typedef unsigned long U32;
typedef LONG64 S64;
typedef ULONG64 U64;

//Forward Declarations
typedef struct _INSTRUCTION INSTRUCTION;
typedef struct _DISASSEMBLER DISASSEMBLER;
typedef struct _X86_OPCODE X86_OPCODE;
typedef struct _INSTRUCTION_OPERAND INSTRUCTION_OPERAND;
typedef struct _CODE_BRANCH CODE_BRANCH;
typedef enum _INSTRUCTION_TYPE INSTRUCTION_TYPE;
typedef struct _DATA_REFERENCE DATA_REFERENCE;
typedef enum _ARCHITECTURE_TYPE ARCHITECTURE_TYPE;

#ifdef SPEEDY
// On Visual Studio 6, making the internal functions inline makes compiling take forever
#define INTERNAL static _inline 
#define INLINE _inline
#else
#define INTERNAL static
#define INLINE
#endif

#define VALID_INSTRUCTION(i) ((i) && !((i)->ErrorOccurred))
#define NEXT_INSTRUCTION(i) ((i)->Address + (i)->Length)
#define DISASM_ARCH_TYPE(dis) ((dis)->ArchType)
#define INS_ARCH_TYPE(ins) DISASM_ARCH_TYPE((ins)->Disassembler)

// NOTE: these should be as big set to the maximum of the supported architectures
#define MAX_PREFIX_LENGTH 15
#define MAX_OPERAND_COUNT 3
#define MAX_INSTRUCTION_LENGTH 25
#define MAX_OPCODE_LENGTH 3
#define MAX_OPCODE_DESCRIPTION 256

/////////////////////////////////////////////////////////////////////
// Code branch
/////////////////////////////////////////////////////////////////////

#define MAX_CODE_REFERENCE_COUNT 3

typedef struct _CODE_BRANCH
{
	U64 Addresses[MAX_CODE_REFERENCE_COUNT]; // NULL if multiple to addresses
	U32 Count;
	U8 IsLoop : 1;
	U8 IsCall : 1; // branch if false
	U8 IsIndirect : 1; // call/jmp [Address]
	U8 AddressOffset: 5;
	struct _INSTRUCTION_OPERAND *Operand; // the operand containg the address
} CODE_BRANCH;

/////////////////////////////////////////////////////////////////////
// Data references
/////////////////////////////////////////////////////////////////////

#define MAX_DATA_REFERENCE_COUNT 3

typedef struct _DATA_REFERENCE
{
	U64 Addresses[MAX_DATA_REFERENCE_COUNT]; // NULL if multiple to addresses
	U32 Count;
	ULONG_PTR DataSize;
	struct _INSTRUCTION_OPERAND *Operand; // the operand containg the address
} DATA_REFERENCE;

////////////////////////////////////////////////////////////////////
// Instruction
/////////////////////////////////////////////////////////////////////

//
// Instruction types (bits 0-7)
// Instruction groups (bits 8-26)
//
#define ITYPE_EXEC_OFFSET     (1<<8)
#define ITYPE_ARITH_OFFSET    (1<<9)
#define ITYPE_LOGIC_OFFSET    (1<<10)
#define ITYPE_STACK_OFFSET    (1<<11)
#define ITYPE_TESTCOND_OFFSET (1<<12)
#define ITYPE_LOAD_OFFSET     (1<<13)
#define ITYPE_ARRAY_OFFSET    (1<<14)
#define ITYPE_BIT_OFFSET      (1<<15)
#define ITYPE_FLAG_OFFSET     (1<<16)
#define ITYPE_FPU_OFFSET      (1<<17)
#define ITYPE_TRAPS_OFFSET    (1<<18)
#define ITYPE_SYSTEM_OFFSET   (1<<19)
#define ITYPE_OTHER_OFFSET    (1<<20)
#define ITYPE_UNUSED1_OFFSET  (1<<21)
#define ITYPE_UNUSED2_OFFSET  (1<<22)
#define ITYPE_UNUSED3_OFFSET  (1<<23)
#define ITYPE_UNUSED4_OFFSET  (1<<24)
#define ITYPE_UNUSED5_OFFSET  (1<<25)
#define ITYPE_UNUSED6_OFFSET  (1<<26)
#define ITYPE_EXT_UNUSED1     (1<<27)
#define ITYPE_EXT_UNUSED2     (1<<28)
#define ITYPE_EXT_UNUSED3     (1<<29)
#define ITYPE_EXT_UNUSED4     (1<<30)
#define ITYPE_EXT_UNUSED5     (1<<31)

//
// X86-specific flags (bits 27-31)
//

#define ITYPE_EXT_64     ITYPE_EXT_UNUSED1 // Use index 1 if in 64-bit mode and 0 otherwise
#define ITYPE_EXT_MODRM  ITYPE_EXT_UNUSED2 // ModRM byte may extend the opcode
#define ITYPE_EXT_SUFFIX ITYPE_EXT_UNUSED3 // byte after ModRM/SIB/displacement is the third opcode
#define ITYPE_EXT_PREFIX ITYPE_EXT_UNUSED4 // prefix
#define ITYPE_EXT_FPU    ITYPE_EXT_UNUSED5 // FPU instructions require special handling

#define ITYPE_3DNOW_OFFSET ITYPE_UNUSED1_OFFSET
#define ITYPE_MMX_OFFSET   ITYPE_UNUSED2_OFFSET
#define ITYPE_SSE_OFFSET   ITYPE_UNUSED3_OFFSET
#define ITYPE_SSE2_OFFSET  ITYPE_UNUSED4_OFFSET
#define ITYPE_SSE3_OFFSET  ITYPE_UNUSED5_OFFSET

//
// Instruction types
//

#define ITYPE_TYPE_MASK  0x7FFFFFFF
#define ITYPE_GROUP_MASK 0x7FFFFF00

typedef enum _INSTRUCTION_TYPE
{
	// ITYPE_EXEC group
	ITYPE_EXEC = ITYPE_EXEC_OFFSET,
	ITYPE_BRANCH,
	ITYPE_BRANCHCC, // conditional (not necessarily just flags)
	ITYPE_CALL,
	ITYPE_CALLCC, // conditional (not necessarily just flags)
	ITYPE_RET,
	ITYPE_LOOPCC,

	// ITYPE_ARITH group
	ITYPE_ARITH = ITYPE_ARITH_OFFSET,
	ITYPE_XCHGADD,
	ITYPE_ADD,
	ITYPE_SUB,
	ITYPE_MUL,
	ITYPE_DIV,
	ITYPE_INC,
	ITYPE_DEC,
	ITYPE_SHL,
	ITYPE_SHR,
	ITYPE_ROL,
	ITYPE_ROR,

	// ITYPE_LOGIC group
	ITYPE_LOGIC=ITYPE_LOGIC_OFFSET,
	ITYPE_AND,
	ITYPE_OR,
	ITYPE_XOR,
	ITYPE_NOT,
	ITYPE_NEG,

	// ITYPE_STACK group
	ITYPE_STACK=ITYPE_STACK_OFFSET,
	ITYPE_PUSH,
	ITYPE_POP,
	ITYPE_PUSHA,
	ITYPE_POPA,
	ITYPE_PUSHF,
	ITYPE_POPF,
	ITYPE_ENTER,
	ITYPE_LEAVE,

	// ITYPE_TESTCOND group
	ITYPE_TESTCOND=ITYPE_TESTCOND_OFFSET,
		ITYPE_TEST,
		ITYPE_CMP,

	// ITYPE_LOAD group
	ITYPE_LOAD=ITYPE_LOAD_OFFSET,
		ITYPE_MOV,
		ITYPE_MOVCC, // conditional
		ITYPE_LEA,
		ITYPE_XCHG,
		ITYPE_XCHGCC, // conditional

	// ITYPE_ARRAY group
	ITYPE_ARRAY=ITYPE_ARRAY_OFFSET,
		ITYPE_STRCMP,
		ITYPE_STRLOAD,
		ITYPE_STRMOV,
		ITYPE_STRSTOR,
		ITYPE_XLAT,

	// ITYPE_BIT group
	ITYPE_BIT=ITYPE_BIT_OFFSET,
		ITYPE_BITTEST,
		ITYPE_BITSET,
		ITYPE_BITCLR,

	// ITYPE_FLAG group
	// PF = parify flag
	// ZF = zero flag
	// OF = overflow flag
	// DF = direction flag
	// SF = sign flag
	ITYPE_FLAG=ITYPE_FLAG_OFFSET,
		// clear
		ITYPE_CLEARCF, 
		ITYPE_CLEARZF,
		ITYPE_CLEAROF,
		ITYPE_CLEARDF,
		ITYPE_CLEARSF,
		ITYPE_CLEARPF,
		// set
		ITYPE_SETCF, 
		ITYPE_SETZF,
		ITYPE_SETOF,
		ITYPE_SETDF,
		ITYPE_SETSF,
		ITYPE_SETPF,
		// toggle
		ITYPE_TOGCF, 
		ITYPE_TOGZF,
		ITYPE_TOGOF,
		ITYPE_TOGDF,
		ITYPE_TOGSF,
		ITYPE_TOGPF,

	// ITYPE_FPU group
	ITYPE_FPU=ITYPE_FPU_OFFSET,
		ITYPE_FADD,
		ITYPE_FSUB,
		ITYPE_FMUL,
		ITYPE_FDIV,
		ITYPE_FCOMP,
		ITYPE_FEXCH,
		ITYPE_FLOAD,
		ITYPE_FLOADENV,
		ITYPE_FSTORE,
		ITYPE_FSTOREENV,
		ITYPE_FSAVE,
		ITYPE_FRESTORE,
		ITYPE_FMOVCC,

	ITYPE_UNUSED1=ITYPE_UNUSED1_OFFSET,
	ITYPE_UNUSED2=ITYPE_UNUSED2_OFFSET,
	ITYPE_UNUSED3=ITYPE_UNUSED3_OFFSET,

	// ITYPE_MMX group
	ITYPE_MMX=ITYPE_MMX_OFFSET,
		ITYPE_MMX_MOV,
		ITYPE_MMX_ADD,
		ITYPE_MMX_SUB,
		ITYPE_MMX_MUL,
		ITYPE_MMX_DIV,
		ITYPE_MMX_AND,
		ITYPE_MMX_OR,
		ITYPE_MMX_XOR,
		ITYPE_MMX_CMP,

	// ITYPE_SSE group
	ITYPE_SSE=ITYPE_SSE_OFFSET,
		ITYPE_SSE_MOV,
		ITYPE_SSE_ADD,
		ITYPE_SSE_SUB,
		ITYPE_SSE_MUL,
		ITYPE_SSE_DIV,
		ITYPE_SSE_AND,
		ITYPE_SSE_OR,
		ITYPE_SSE_XOR,
		ITYPE_SSE_CMP,
		
		// ITYPE_SSE2 group
	ITYPE_SSE2=ITYPE_SSE2_OFFSET,
		ITYPE_SSE2_MOV,
		ITYPE_SSE2_ADD,
		ITYPE_SSE2_SUB,
		ITYPE_SSE2_MUL,
		ITYPE_SSE2_DIV,
		ITYPE_SSE2_AND,
		ITYPE_SSE2_OR,
		ITYPE_SSE2_XOR,
		ITYPE_SSE2_CMP,

	// ITYPE_SSE3 group
	ITYPE_SSE3=ITYPE_SSE3_OFFSET,
		ITYPE_SSE3_MOV,
		ITYPE_SSE3_ADD,
		ITYPE_SSE3_SUB,
		ITYPE_SSE3_MUL,
		ITYPE_SSE3_DIV,
		ITYPE_SSE3_AND,
		ITYPE_SSE3_OR,
		ITYPE_SSE3_XOR,
		ITYPE_SSE3_CMP,

	// ITYPE_3DNOW group
	ITYPE_3DNOW=ITYPE_3DNOW_OFFSET,
		ITYPE_3DNOW_ADD,
		ITYPE_3DNOW_SUB,
		ITYPE_3DNOW_MUL,
		ITYPE_3DNOW_DIV,
		ITYPE_3DNOW_CMP,
		ITYPE_3DNOW_XCHG,

	// ITYPE_TRAP
	ITYPE_TRAPS=ITYPE_TRAPS_OFFSET, 
		ITYPE_TRAP, // generate trap
		ITYPE_TRAPCC,  // conditional trap gen
		ITYPE_TRAPRET,    // return from trap
		ITYPE_BOUNDS,  // gen bounds trap
		ITYPE_DEBUG,   // gen breakpoint trap
		ITYPE_TRACE,   // gen single step trap
		ITYPE_INVALID, // gen invalid instruction
		ITYPE_OFLOW,   // gen overflow trap

	// ITYPE_SYSTEM group
	ITYPE_SYSTEM=ITYPE_SYSTEM_OFFSET,
		ITYPE_HALT,    // halt machine
		ITYPE_IN,      // input form port
		ITYPE_OUT,     // output to port
		ITYPE_CPUID,   // identify cpu
		ITYPE_SETIF,   // allow interrupts
		ITYPE_CLEARIF, // block interrupts
		ITYPE_SYSCALL,
		ITYPE_SYSCALLRET,

	// ITYPE_OTHER group
	ITYPE_OTHER = ITYPE_OTHER_OFFSET,
		ITYPE_NOP,
		ITYPE_BCDCONV, // convert to/from BCD
		ITYPE_SZCONV   // convert size of operand
} INSTRUCTION_TYPE;

//
// Operand flags
//

// Type = bits 0-6 (these are mutually exclusive -- bits 0-6 will always be a power of 2))
#define OPTYPE_NONE    0x00
#define OPTYPE_IMM    0x01 // immediate value
#define OPTYPE_OFFSET 0x02 // relative offset
#define OPTYPE_FLOAT  0x03 // floating point
#define OPTYPE_BCD    0x04
#define OPTYPE_STRING 0x05
#define OPTYPE_SPECIAL 0x06
#define OPTYPE_MASK   0x7F

// Flags = bits 7-23 (these can be combinations)
// These are used in the X86 opcode table
#define OP_REG      (1<<7) // 0x80
#define OP_SIGNED   (1<<8)
#define OP_SYS      (1<<9) // parameter is an index into some system structure
#define OP_CONDR    (1<<10)
#define OP_CONDW    (1<<11)
#define OP_UNUSED   (1<<12)
#define OP_SRC      (1<<13) // operand is source operand
#define OP_DST      (1<<14) // operand is destination operand
#define OP_EXEC     (1<<15) // operand is executed

#define OP_CONDE     OP_CONDR
#define OP_COND_EXEC (OP_CONDE|OP_EXEC) // executed only if the pre-conditions are met
#define OP_COND_SRC  (OP_CONDR|OP_SRC) // set only if pre-conditions are met
#define OP_COND_DST  (OP_CONDW|OP_DST) // set only if pre-conditions are met
#define OP_COND      (OP_CONDR|OP_CONDW)

// Bits 16-31 are available for use outside of the opcode table, but they can only
// be used in INSTRUCTION_OPERAND.Flags, they may conflit with the architecture specific
// operands. For example, bits 16-31 are used in X86 for AMODE_* and OPTYPE_*
#define OP_ADDRESS    (1<<16)
#define OP_LOCAL      (1<<17)
#define OP_PARAM      (1<<18)
#define OP_GLOBAL     (1<<19)
#define OP_FAR        (1<<20)
#define OP_IPREL      (1<<21)

//
// X86-specific flags (bits 27-31)
//
#define OP_MSR      (OP_SYS|OP_UNUSED)

//
// Other architecture flags
//
#define OP_DELAY  OP_UNUSED // delayed instruction (e.g., delayed branch that executes after the next instruction)

/////////////////////////////////////////////////////////////////////
// Architectures
/////////////////////////////////////////////////////////////////////

typedef enum _ARCHITECTURE_TYPE
{
	ARCH_UNKNOWN=0,
	
	// x86-based
	ARCH_X86,    // 32-bit x86
	ARCH_X86_16, // 16-bit x86
	ARCH_X64,    // AMD64 and Intel EMD64
	
	// everything else
	ARCH_ALPHA,
	ARCH_ARM,
	ARCH_DOTNET,
	ARCH_EFI,
	ARCH_IA64,
	ARCH_M68K,
	ARCH_MIPS,
	ARCH_PPC,
	ARCH_SH3,
	ARCH_SH4,
	ARCH_SPARC,
	ARCH_THUMB

} ARCHITECTURE_TYPE;

typedef BOOL (*INIT_INSTRUCTION)(struct _INSTRUCTION *Instruction);
typedef void (*DUMP_INSTRUCTION)(struct _INSTRUCTION *Instruction, BOOL ShowBytes, BOOL Verbose);
typedef BOOL (*GET_INSTRUCTION)(struct _INSTRUCTION *Instruction, U8 *Address, U32 Flags);
typedef U8 *(*FIND_FUNCTION_BY_PROLOGUE)(struct _INSTRUCTION *Instruction, U8 *StartAddress, U8 *EndAddress, U32 Flags);

typedef struct _ARCHITECTURE_FORMAT_FUNCTIONS
{
	INIT_INSTRUCTION InitInstruction;
	DUMP_INSTRUCTION DumpInstruction;
	GET_INSTRUCTION GetInstruction;
	FIND_FUNCTION_BY_PROLOGUE FindFunctionByPrologue;
} ARCHITECTURE_FORMAT_FUNCTIONS;

typedef struct _ARCHITECTURE_FORMAT
{
	ARCHITECTURE_TYPE Type;
	ARCHITECTURE_FORMAT_FUNCTIONS *Functions;
} ARCHITECTURE_FORMAT;

#define DISASSEMBLER_INITIALIZED 0x1234566F
#define INSTRUCTION_INITIALIZED 0x1234567F

// Since addresses are internally represented as 64-bit, we need to specially handle
// cases where IP + Displacement wraps around for 16-bit/32-bit operand size
// Otherwise, ignorethe possibility of wraparounds
#define SUPPORT_WRAPAROUND

#ifdef NO_SANITY_CHECKS
#undef NDEBUG
#undef DEBUG_DISASM
#undef assert
#define assert(x)
#endif

#ifdef DEBUG_DISASM
#define DISASM_OUTPUT(x) printf x
#else
#define DISASM_OUTPUT(x)
#endif

#ifdef _WIN64
#pragma warning(disable:4311 4312)
#endif

////////////////////////////////////////////////////////////////////////
// Internal macros
////////////////////////////////////////////////////////////////////////

#define VIRTUAL_ADDRESS ((U64)Instruction->Address + Instruction->VirtualAddressDelta)

#define AMD64_DIFF (AMD64_8BIT_OFFSET-X86_8BIT_OFFSET)
#define IS_AMD64() (INS_ARCH_TYPE(Instruction) == ARCH_X64)
#define IS_X86_32() (INS_ARCH_TYPE(Instruction) == ARCH_X86)
#define IS_X86_16() (INS_ARCH_TYPE(Instruction) == ARCH_X86_16)

#define X86_BOUND 0x62
#define X86_PUSH_REG 0x50
#define X86_PUSH_CS 0x0e
#define X86_PUSH_DS 0x1e
#define X86_PUSH_SS 0x16
#define X86_PUSH_ES 0x06
#define X86_PUSH_FS 0xa0
#define X86_PUSH_GS 0xa8
#define X86_PUSH_U8 0x6a
#define X86_PUSH_U32 0x68
#define X86_POP_DS 0x1f
#define X86_POP_ES 0x07
#define X86_POP_SS 0x17
#define X86_POP_FS 0xa1
#define X86_POP_GS 0xa9
#define X86_POP_REG 0x58

#define OPCSTR Instruction->String+Instruction->StringIndex
#define APPEND Instruction->StringIndex += (U8)_snprintf
#define APPENDPAD(x) \
{ \
	if (Instruction->StringAligned) \
	{  \
			if (Instruction->StringIndex > x) assert(0); \
			while (x != Instruction->StringIndex) APPENDB(' ');  \
	}  \
	else if (Instruction->StringIndex) \
	{  \
		APPENDB(' '); \
	} \
}

#define APPENDB(a) Instruction->String[Instruction->StringIndex++] = a
#define APPENDS(a) APPEND(OPCSTR, SIZE_LEFT, a);

#define SIZE_LEFT (MAX_OPCODE_DESCRIPTION-1 > Instruction->StringIndex ? MAX_OPCODE_DESCRIPTION-Instruction->StringIndex : 0)

// If an address size prefix is used for an instruction that doesn't make sense, restore it
// to the default

#define SANITY_CHECK_OPERAND_SIZE() \
{ \
	if (!Instruction->AnomalyOccurred && X86Instruction->HasOperandSizePrefix) \
	{ \
		if (!SuppressErrors) printf("[0x%08I64X] ANOMALY: Unexpected operand size prefix\n", VIRTUAL_ADDRESS); \
		Instruction->AnomalyOccurred = TRUE; \
		X86Instruction->HasOperandSizePrefix = FALSE; \
		switch (X86Instruction->OperandSize) \
		{ \
			case 4: X86Instruction->OperandSize = 2; break; \
			case 2: X86Instruction->OperandSize = 4; break; \
			default: assert(0); \
		} \
	} \
}

#define SANITY_CHECK_ADDRESS_SIZE() \
{ \
	if (!Instruction->AnomalyOccurred && X86Instruction->HasAddressSizePrefix) \
	{ \
		if (!SuppressErrors) printf("[0x%08I64X] ANOMALY: Unexpected address size prefix\n", VIRTUAL_ADDRESS); \
		Instruction->AnomalyOccurred = TRUE; \
	} \
	X86Instruction->HasAddressSizePrefix = FALSE; \
	switch (INS_ARCH_TYPE(Instruction)) \
	{ \
		case ARCH_X64: X86Instruction->AddressSize = 8; break; \
		case ARCH_X86: X86Instruction->AddressSize = 4; break; \
		case ARCH_X86_16: X86Instruction->AddressSize = 2; break; \
	} \
}

#define SANITY_CHECK_SEGMENT_OVERRIDE() \
	if (!Instruction->AnomalyOccurred && X86Instruction->HasSegmentOverridePrefix) \
	{ \
		if (!SuppressErrors) printf("[0x%08I64X] ANOMALY: Unexpected segment override\n", VIRTUAL_ADDRESS); \
		Instruction->AnomalyOccurred = TRUE; \
	}

#define INSTR_INC(size) \
{ \
	Instruction->Length += size; \
	Address += size; \
}

#define X86_SET_TARGET() \
{ \
	if (X86Instruction->HasSelector) \
	{ \
		if (!Instruction->AnomalyOccurred) \
		{ \
			if (!SuppressErrors) printf("[0x%08I64X] ANOMALY: unexpected segment 0x%02X\n", VIRTUAL_ADDRESS, X86Instruction->Selector); \
			Instruction->AnomalyOccurred = TRUE; \
		} \
	} \
	else \
	{ \
		switch (X86Instruction->Segment) \
		{ \
			case SEG_CS: \
			case SEG_DS: \
			case SEG_SS: \
			case SEG_ES: \
				assert(!X86Instruction->HasSelector); \
				Operand->TargetAddress = (U64)X86Instruction->Displacement; \
				/* assert(!GetAbsoluteAddressFromSegment((BYTE)X86Instruction->Segment, (DWORD)X86Instruction->Displacement) || GetAbsoluteAddressFromSegment(X86Instruction->Segment, (DWORD)X86Instruction->Displacement) == Operand->TargetAddress); */ \
				break; \
			case SEG_FS: \
			case SEG_GS: \
				assert(!X86Instruction->HasSelector); \
				Operand->TargetAddress = (U64)GetAbsoluteAddressFromSegment((BYTE)X86Instruction->Segment, (DWORD)X86Instruction->Displacement); \
				break; \
			default: \
				assert(0); /* shouldn't be possible */ \
				break; \
		} \
	} \
}

#define X86_SET_SEG(reg) \
{ \
	if (!X86Instruction->HasSegmentOverridePrefix && (reg == REG_EBP || reg == REG_ESP)) \
	{ \
		assert(!X86Instruction->HasSelector); \
		X86Instruction->Segment = SEG_SS; \
	} \
}

#define X86_SET_ADDR() \
{ \
	if (Operand->Flags & OP_DST) \
	{ \
		assert(!X86Instruction->HasDstAddressing); \
		X86Instruction->HasDstAddressing = TRUE; \
		X86Instruction->DstOpIndex[X86Instruction->DstOpCount] = (U8)OperandIndex; \
		X86Instruction->DstOpCount++; \
		X86Instruction->DstAddressIndex = (U8)OperandIndex; \
	} \
	if (Operand->Flags & OP_SRC) \
	{ \
		if (Instruction->Type != ITYPE_STRCMP) assert(!X86Instruction->HasSrcAddressing); \
		X86Instruction->HasSrcAddressing = TRUE; \
		X86Instruction->SrcOpIndex[X86Instruction->SrcOpCount] = (U8)OperandIndex; \
		X86Instruction->SrcOpCount++; \
		X86Instruction->SrcAddressIndex = (U8)OperandIndex; \
	} \
}

#define X86_SET_REG(reg) \
{ \
	if (Operand->Flags & OP_DST) \
	{ \
		X86Instruction->DstOpIndex[X86Instruction->DstOpCount] = (U8)OperandIndex; \
		X86Instruction->DstOpCount++; \
		assert(OperandIndex < 2); \
		if (Operand->Length > 1 && reg == REG_ESP) Instruction->Groups |= ITYPE_STACK; \
	} \
	if (Operand->Flags & OP_SRC) \
	{ \
		X86Instruction->SrcOpIndex[X86Instruction->SrcOpCount] = (U8)OperandIndex; \
		X86Instruction->SrcOpCount++; \
	} \
} 

#define CHECK_AMD64_REG() { if (IS_AMD64()) Operand->Register += AMD64_DIFF; }

////////////////////////////////////////////////////////////////////////
// Internal structures/variables
////////////////////////////////////////////////////////////////////////

char *X86_Registers[0xE0] = 
{
	// Segments
	"es", // 0x00
	"cs", // 0x01
	"ss", // 0x02
	"ds", // 0x03
	"fs", // 0x04
	"gs", // 0x05
	"flags", // 0x06
	"eflags", // 0x07
	"rflags", // 0x08
	"ip+ilen", // 0x09
	"eip+ilen", // 0x0A
	"rip+ilen", // 0x0B
	NULL, // 0x0C
	NULL, // 0x0D
	NULL, // 0x0E
	NULL, // 0x0F

	// Test
	"tr0", // 0x10
	"tr1", // 0x11
	"tr2", // 0x12
	"tr3", // 0x13
	"tr4", // 0x14
	"tr5", // 0x15
	"tr6", // 0x16
	"tr7", // 0x17
	"tr8", // 0x18
	"tr9", // 0x19
	"tr10", // 0x1A
	"tr11", // 0x1B
	"tr12", // 0x1C
	"tr13", // 0x1D
	"tr14", // 0x1E
	"tr15", // 0x1F

	// Control
	"cr0", // 0x20
	"cr1", // 0x21
	"cr2", // 0x22
	"cr3", // 0x23
	"cr4", // 0x24
	"cr5", // 0x25
	"cr6", // 0x26
	"cr7", // 0x27
	"cr8", // 0x18
	"cr9", // 0x19
	"cr10", // 0x1A
	"cr11", // 0x1B
	"cr12", // 0x1C
	"cr13", // 0x1D
	"cr14", // 0x1E
	"cr15", // 0x1F

	// Debug
	"dr0", // 0x30
	"dr1", // 0x31
	"dr2", // 0x32
	"dr3", // 0x33
	"dr4", // 0x34
	"dr5", // 0x35
	"dr6", // 0x36
	"dr7", // 0x37
	"dr8", // 0x38
	"dr9", // 0x39
	"dr10", // 0x3A
	"dr11", // 0x3B
	"dr12", // 0x3C
	"dr13", // 0x3D
	"dr14", // 0x3E
	"dr15", // 0x3F

	// FPU
	"st(0)", // 0x40
	"st(1)", // 0x41
	"st(2)", // 0x42
	"st(3)", // 0x43
	"st(4)", // 0x44
	"st(5)", // 0x45
	"st(6)", // 0x46
	"st(7)", // 0x47
	NULL, // 0x48
	NULL, // 0x49
	NULL, // 0x4A
	NULL, // 0x4B
	NULL, // 0x4C
	NULL, // 0x4D
	NULL, // 0x4E
	NULL, // 0x4F

	// MMX
	"mm0", // 0x50
	"mm1", // 
	"mm2",
	"mm3",
	"mm4",
	"mm5",
	"mm6",
	"mm7",
	NULL, // 0x58
	NULL, // 0x59
	NULL, // 0x5A
	NULL, // 0x5B
	NULL, // 0x5C
	NULL, // 0x5D
	NULL, // 0x5E
	NULL, // 0x5F

	// XMM
	"xmm0", // 0x60
	"xmm1", // 0x61
	"xmm2", // 0x62
	"xmm3", // 0x63
	"xmm4", // 0x64
	"xmm5", // 0x65
	"xmm6", // 0x66
	"xmm7", // 0x67
	"xmm8", // 0x68
	"xmm9", // 0x69
	"xmm10", // 0x6a
	"xmm11", // 0x6b
	"xmm12", // 0x6c
	"xmm13", // 0x6d
	"xmm14", // 0x6e
	"xmm15", // 0x6f

	// 8-bit
	"al", // 0x70
	"cl", // 0x71
	"dl", // 0x72
	"bl", // 0x73
	"ah", // 0x74
	"ch", // 0x75
	"dh", // 0x76
	"bh", // 0x77
 	NULL, // 0x78
	NULL, // 0x79
	NULL, // 0x7A
	NULL, // 0x7B
	NULL, // 0x7C
	NULL, // 0x7D
	NULL, // 0x7E
	NULL, // 0x7F

	// 16-bit
	"ax", // 0x80
	"cx", // 0x81
	"dx", // 0x82
	"bx", // 0x83
	"sp", // 0x84
	"bp", // 0x85
	"si", // 0x86
	"di", // 0x87
	NULL, // 0x88
	NULL, // 0x89
	NULL, // 0x8A
	NULL, // 0x8B
	NULL, // 0x8C
	NULL, // 0x8D
	NULL, // 0x8E
	NULL, // 0x8F

	// 32-bit
	"eax", // 0x90
	"ecx", // 0x91
	"edx", // 0x92
	"ebx", // 0x93
	"esp", // 0x94
	"ebp", // 0x95
	"esi", // 0x96
	"edi", // 0x97
	NULL, // 0x98
	NULL, // 0x99
	NULL, // 0x9A
	NULL, // 0x9B
	NULL, // 0x9C
	NULL, // 0x9D
	NULL, // 0x9E
	NULL, // 0x9F

	// X86-64 8-bit register
	"al", // 0xA0
	"cl", // 0xA1
	"dl", // 0xA2
	"bl", // 0xA3
	"spl", // 0xA4
	"bpl", // 0xA5
	"sil", // 0xA6
	"dil", // 0xA7
	"r8b", // 0xA8
	"r9b", // 0xA9
	"r10b", // 0xAA
	"r11b", // 0xAB
	"r12b", // 0xAC
	"r13b", // 0xAD
	"r14b", // 0xAE
	"r15b", // 0xAF

	// X86-64 16-bit register	
	"ax", // 0xB0
	"cx", // 0xB1
	"dx", // 0xB2
	"bx", // 0xB3
	"sp", // 0xB4
	"bp", // 0xB5
	"si", // 0xB6
	"di", // 0xB7
	"r8w", // 0xB8
	"r9w", // 0xB9
	"r10w", // 0xBA
	"r11w", // 0xBB
	"r12w", // 0xBC
	"r13w", // 0xBD
	"r14w", // 0xBE
	"r15w", // 0xBF

	// X86-64 32-bit register
	"eax", // 0xC0
	"ecx", // 0xC1
	"edx", // 0xC2
	"ebx", // 0xC3
	"esp", // 0xC4
	"ebp", // 0xC5
	"esi", // 0xC6
	"edi", // 0xC7
	"r8d", // 0xC8
	"r9d", // 0xC9
	"r10d", // 0xCA
	"r11d", // 0xCB
	"r12d", // 0xCC
	"r13d", // 0xCD
	"r14d", // 0xCE
	"r15d", // 0xCF

	// X86-64 64-bit register	
	"rax", // 0xD0
	"rcx", // 0xD1
	"rdx", // 0xD2
	"rbx", // 0xD3
	"rsp", // 0xD4
	"rbp", // 0xD5
	"rsi", // 0xD6
	"rdi", // 0xD7
	"r8", // 0xD8
	"r9", // 0xD9
	"r10", // 0xDA
	"r11", // 0xDB
	"r12", // 0xDC
	"r13", // 0xDD
	"r14", // 0xDE
	"r15" // 0xDF
};

#ifdef __cplusplus
extern "C" {
#endif

// NOTE: the processor may actually accept less than this amount (officially 15)
// #define AMD64_MAX_INSTRUCTION_LEN 15 // theoretical max 25=5+2+1+1+8+8
#define AMD64_MAX_PREFIX_LENGTH 5 // 4 legacy + 1 rex
#define AMD64_MAX_ADDRESS_LENGTH 18 // modrm + sib + 8 byte displacement + 8 byte immediate value

// NOTE: the processor may actually accept less than this amount (officially 15)
#define X86_MAX_INSTRUCTION_LEN 15 // theoretical 16=4+2+1+1+4+4
#define X86_MAX_PREFIX_LENGTH 4
#define X86_MAX_OPCODE_LENGTH 3 // third byte is either a suffix or prefix
#define X86_MAX_ADDRESS_LENGTH 10 // modrm + sib + 4 byte displacement + 4 byte immediate value
#define X86_MAX_OPERANDS 3

#define X86_PREFIX(a) ((a)->MnemonicFlags == ITYPE_EXT_PREFIX)
#define X86_SPECIAL_EXTENSION(a) ((a)->MnemonicFlags & (ITYPE_EXT_MODRM|ITYPE_EXT_FPU|ITYPE_EXT_SUFFIX|ITYPE_EXT_64))
#define X86_EXTENDED_OPCODE(a) ((a)->Table)
#define X86_INVALID(a) (!(a)->MnemonicFlags && !(a)->Table)
#define X86_OPERAND_COUNT(a) ((a)->OperandFlags[0] ? ((a)->OperandFlags[1] ? ((a)->OperandFlags[2] ? 3 : 2) : 1) : 0)
#define X86_GET_CATEGORY(p) ((p)->MnemonicFlags & ITYPE_GROUP_MASK)
#define X86_GET_TYPE(p) ((p)->MnemonicFlags & ITYPE_TYPE_MASK)

// Various instructions being specially decoded
#define X86_TWO_BYTE_OPCODE 0x0f
#define PREFIX_SEGMENT_OVERRIDE_ES 0x26
#define PREFIX_SEGMENT_OVERRIDE_CS 0x2e
#define PREFIX_BRANCH_NOT_TAKEN 0x2e // used only with conditional jumps
#define PREFIX_SEGMENT_OVERRIDE_SS 0x36
#define PREFIX_SEGMENT_OVERRIDE_DS 0x3e
#define PREFIX_BRANCH_TAKEN 0x3e // used only with conditional jumps
#define PREFIX_SEGMENT_OVERRIDE_FS 0x64
#define PREFIX_SEGMENT_OVERRIDE_GS 0x65
#define PREFIX_OPERAND_SIZE 0x66
#define PREFIX_ADDRESS_SIZE 0x67
#define PREFIX_LOCK 0xf0
#define PREFIX_REPNE 0xf2
#define PREFIX_REP 0xf3

//////////////////////////////////////////////////////////////////
// Implicit operand handling
//////////////////////////////////////////////////////////////////

#define X86_AMODE_MASK   0x00FF0000 // bits 16-23 (AMODE_*)
#define X86_OPFLAGS_MASK 0x0000FF80 // bits 7-15 (OPTYPE_*)
#define X86_OPTYPE_MASK  0xFF0000FF // bits 0-7 (OPTYPE_* below + OP_REG) and 24-31 (OPTYPE_* above)

#define OPTYPE_0   0x01
#define OPTYPE_1   0x02
#define OPTYPE_FF  0x03
//...
#define OPTYPE_CS  0x10
#define OPTYPE_DS  0x11
#define OPTYPE_ES  0x12
#define OPTYPE_FS  0x13
#define OPTYPE_GS  0x14
#define OPTYPE_SS  0x15
#define OPTYPE_CR0 0x16
#define OPTYPE_TSC 0x17 // time stamp counter
//...
#define OPTYPE_FLAGS  0x20
#define OPTYPE_xFLAGS 0x21 // RFLAGS/EFLAGS (depending on operand size)
#define OPTYPE_xCX_HI_xBX_LO 0x22 // represented by 2 registers CX:BX or ECX:EBX (depending on operand size)
#define OPTYPE_xDX_HI_xAX_LO 0x23 // DX:AX or EDX:EAX (depending on operand size)
#define OPTYPE_EDX_HI_EAX_LO 0x24 // DX:AX or EDX:EAX (depending on operand size)
#define OPTYPE_EDX_ECX_EBX_EAX 0x25 // all registers are set
//...
#define OPTYPE_STx 0x30
#define OPTYPE_ST0 0x31
#define OPTYPE_ST1 0x32
#define OPTYPE_FPU_STATUS  0x33
#define OPTYPE_FPU_CONTROL 0x34
#define OPTYPE_FPU_TAG 0x35
#define OPTYPE_FLDZ   0x36 // 0
#define OPTYPE_FLD1   0x37 // 1
#define OPTYPE_FLDPI  0x38 // pi
#define OPTYPE_FLDL2T 0x39 // lg 10
#define OPTYPE_FLDL2E 0x3A // lg e
#define OPTYPE_FLDLG2 0x3B // log_10 2
#define OPTYPE_FLDLN2 0x3C // log_e 2
//...
#define OPTYPE_CS_MSR 0x40
#define OPTYPE_EIP_MSR 0x41
#define OPTYPE_ESP_MSR 0x42
#define OPTYPE_KERNELBASE_MSR 0x43
#define OPTYPE_FMASK_MSR 0x44
#define OPTYPE_STAR_MSR 0x45
#define OPTYPE_CSTAR_MSR 0x46 // 32-bit mode
#define OPTYPE_LSTAR_MSR 0x47 // 64-bit mode


// NOTE: OPTYPES >= 0x80 reserved for registers (OP_REG+XX)
#define OPTYPE_REG_AL OP_REG+0x01
#define OPTYPE_REG_CL OP_REG+0x02
#define OPTYPE_REG_AH OP_REG+0x03
#define OPTYPE_REG_AX OP_REG+0x04
#define OPTYPE_REG_DX OP_REG+0x05
#define OPTYPE_REG_ECX OP_REG+0x06
#define OPTYPE_REG8 OP_REG+0x07

// If address size is 2, use BP
// If address size is 4, use EBP
// If address size is 8, use RBP
#define OPTYPE_REG_xBP OP_REG+0x08

// If address size is 2, use BP
// If address size is 4, use EBP
// If address size is 8, use RBP
#define OPTYPE_REG_xSP OP_REG+0x09

// If operand size is 2, take 8-bit register
// If operand size is 4, take 16-bit register
// If operand size is 8, take 32-bit register
#define OPTYPE_REG_xAX_SMALL OP_REG+0x0a

// If operand size is 2, take 16-bit register
// If operand size is 4, take 32-bit register
// If operand size is 8, take 64-bit register
#define OPTYPE_REG_xAX_BIG OP_REG+0x0b

typedef enum _CPU_TYPE
{
	CPU_UNKNOWN=0,

	///////////////////////////////////////
	// 1st generation
	///////////////////////////////////////
	// 1978
	//CPU_8086 = 1MB address limit, 16-bit registers
	// 1982
	//CPU_i186

	///////////////////////////////////////
	// 2nd generation
	///////////////////////////////////////
	// 1982
	//CPU_I286 // 16MB limit, 16-bit registers, added protected mode
	CPU_I287, // CPU_I286 + math coprocessor

	///////////////////////////////////////
	// 3rd generation
	///////////////////////////////////////
	// 1985
	CPU_I386, // 32-bit registers, 4GB memory limit
	// 1988
	CPU_I387, // CPU_I386 + math coprocessor

	///////////////////////////////////////
	// 4th generation (1989)
	///////////////////////////////////////
	CPU_I486,

	///////////////////////////////////////
	// 5th generation
	///////////////////////////////////////
	// 1993
	CPU_PENTIUM, // superscalar architecture
	// 1997
	//CPU_PENTIUM_MMX
	
	///////////////////////////////////////
	// 6th generation (1995)
	///////////////////////////////////////
	CPU_PENTIUM_PRO, // P6 architecture, no MMX, out-of-order execution, speculative execution
	//CPU_CYRIX_6X86,
	//CPU_AMD_K5 // RISC processor
	// 1997
	CPU_PENTIUM2, // Pentium Pro architecture + MMX
	//CPU_AMD_K6,
	//CPU_CYRIX_6X86MX, // Cyrix 6x86 + MMX
	// 1998
	CPU_AMD_K6_2, // added 3DNow! (MMX)
	// 1999
	// CPU_AMD_K6_3 // added SSE

	///////////////////////////////////////
	// 7th generation
	///////////////////////////////////////
	// 1999
	CPU_PENTIUM3, // introduced SSE
	// CPU_AMD_K7 // aka Athlon
	// 2000
	CPU_PENTIUM4, // introduced SSE2 and hyperthreading

	// 2004? 2005?
	CPU_PRESCOTT, // introduced SSE3

	///////////////////////////////////////
	// 8th generation (X86-64)
	// IA32 instruction set with 64-bit extensions, >4GB RAM
	///////////////////////////////////////

	// 2003
	CPU_AMD64, // includes Athlon 64 and Opteron aka X86-64

	// 2004?
	//CPU_EMD64 // Intel's version of AMD64
	CPU_IA64 // aka Itanium: new instruction set -- adds JMPE to IA32 mode to return to IA64 native code

} CPU_TYPE;

//////////////////////////////////////////////////////////////////
// Conditions (these can be OR'd)
//////////////////////////////////////////////////////////////////

// Used for Flags.Preconditions
#define COND_O   (1<<0)  // overflow (signed)
#define COND_C   (1<<1)  // below (unsigned)
#define COND_Z   (1<<2)  // equal (unsigned)
#define COND_S   (1<<3)  // sign set (signed)
#define COND_P   (1<<4)  // parity even
#define COND_BE  (1<<5)  // CF or ZF is set (unsigned)
#define COND_L   (1<<6)  // (SF && !OF) || (OF && !SF)
#define COND_LE  (1<<7)  // ZF || (SF && !OF) || (OF && !SF) (signed)
#define COND_NO  (1<<8)  // !O
#define COND_NC  (1<<9)  // !C (not below, above or equal to)
#define COND_NZ  (1<<10) // !Z (not equal)
#define COND_NS  (1<<11) // !S
#define COND_NP  (1<<12) // !P (parity odd)
#define COND_NL  (1<<13) // (!SF && !OF) || (SF && OF)
#define COND_G   (1<<14) // !ZF && ((!SF && !OF) || (SF && OF))
#define COND_D   (1<<15) // DF
#define COND_REG_xCX_BIG_Z  (1<<16) // CX/ECX/RCX (depending on address size) == 0
#define COND_REG_xCX_BIG_NZ (1<<17) // CX/ECX/RCX (depending on address size) != 0
#define COND_OP1_EQ_OP2 (1<<18)
#define COND_OP1_EQ_OP3 (1<<19)
#define COND_B   COND_C
#define COND_NAE COND_C
#define COND_E   COND_Z
#define COND_NA  COND_BE
#define COND_PE  COND_P
#define COND_U   COND_P
#define COND_NGE COND_L
#define COND_NG  COND_LE
#define COND_PO  COND_NP
#define COND_NU  COND_NP
#define COND_NE  COND_NZ
#define COND_NB  COND_NC
#define COND_AE  COND_NC
#define COND_NE  COND_NZ
#define COND_A   (COND_NC|COND_NZ)
#define COND_NBE COND_A
#define COND_GE COND_NL
#define COND_NLE COND_G

// Used for Opcode.FlagsChanged
#define FLAG_CF_SET (1<<0)
#define FLAG_DF_SET (1<<1)
#define FLAG_IF_SET (1<<2)
#define FLAG_SET_MASK (FLAG_CF_SET|FLAG_DF_SET|FLAG_IF_SET)

#define FLAG_SF_CLR (1<<3)
#define FLAG_ZF_CLR (1<<4)
#define FLAG_AF_CLR (1<<5)
#define FLAG_CF_CLR (1<<6)
#define FLAG_DF_CLR (1<<7)
#define FLAG_IF_CLR (1<<8)
#define FLAG_OF_CLR (1<<9)
#define FPU_C0_CLR (1<<19)
#define FPU_C1_CLR (1<<20)
#define FPU_C2_CLR (1<<21)
#define FPU_C3_CLR (1<<22)
#define FPU_ALL_CLR (FPU_C0_CLR|FPU_C1_CLR|FPU_C2_CLR|FPU_C3_CLR)
#define FLAG_CLR_MASK (FLAG_SF_CLR|FLAG_ZF_CLR|FLAG_AF_CLR|FLAG_CF_CLR|FLAG_DF_CLR|FLAG_IF_CLR|FLAG_OF_CLR|FPU_ALL_CLR)

#define FLAG_OF_MOD (1<<10)
#define FLAG_SF_MOD (1<<11)
#define FLAG_ZF_MOD (1<<12)
#define FLAG_AF_MOD (1<<13)
#define FLAG_PF_MOD (1<<14)
#define FLAG_CF_MOD (1<<15)
#define FLAG_DF_MOD (1<<16)
#define FLAG_IF_MOD (1<<17)
#define FLAG_ALL_MOD (FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD|FLAG_CF_MOD|FLAG_DF_MOD|FLAG_IF_MOD)
#define FLAG_COMMON_MOD (FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD|FLAG_CF_MOD)
#define FPU_C0_MOD (1<<23)
#define FPU_C1_MOD (1<<24)
#define FPU_C2_MOD (1<<25)
#define FPU_C3_MOD (1<<26)
#define FPU_ALL_MOD (FPU_C0_MOD|FPU_C1_MOD|FPU_C2_MOD|FPU_C3_MOD)
#define FLAG_MOD_MASK (FLAG_ALL_MOD|FPU_ALL_MOD)

#define FLAG_CF_TOG (1<<18)
#define FLAG_TOG_MASK FLAG_CF_TOG

// Used for Opcode.ResultsIfTrue and Opcode.ResultsIfFalse
#define OP1_DST         (1<<0)
#define OP2_DST         (1<<1)
#define OP3_DST         (1<<2)
#define OP1_SRC         (1<<3)
#define OP2_SRC         (1<<4)
#define OP3_SRC         (1<<5)
#define FPU_STACK_INC   (1<<6)
#define FPU_STACK_INC2  (1<<7)
#define FPU_STACK_DEC   (1<<8)
#define SERIALIZE_WRITE (1<<9)
#define SERIALIZE_READ  (1<<10)
#define xCX_DEC         (1<<11)
#define xCX_REP_DEC     (1<<12)
#define xDI_DEC         (1<<13)
#define xDI_INC         (1<<14)
#define xSI_DEC         (1<<15)
#define xSI_INC         (1<<16)
#define xDI_DECx        (1<<17)
#define xDI_INCx        (1<<18)
#define xSI_DECx        (1<<19)
#define xSI_INCx        (1<<20)
#define FPU_STACK_PUSH FPU_STACK_DEC
#define FPU_STACK_POP  FPU_STACK_INC
#define FPU_STACK_POP2 FPU_STACK_INC2
#define SERIALIZE_ALL (SERIALIZE_WRITE|SERIALIZE_READ)

#define X86_SEGMENT_OFFSET 0x00
#define X86_TEST_OFFSET    0x10
#define X86_CONTROL_OFFSET 0x20
#define X86_DEBUG_OFFSET   0x30
#define X86_FPU_OFFSET     0x40
#define X86_MMX_OFFSET     0x50
#define X86_XMM_OFFSET     0x60
#define X86_8BIT_OFFSET    0x70
#define X86_16BIT_OFFSET   0x80
#define X86_32BIT_OFFSET   0x90
#define AMD64_8BIT_OFFSET  0xA0
#define AMD64_16BIT_OFFSET 0xB0
#define AMD64_32BIT_OFFSET 0xC0
#define AMD64_64BIT_OFFSET 0xD0

typedef enum _X86_REGISTER
{
	// Segments
	X86_SEG_ES = X86_SEGMENT_OFFSET,
	X86_SEG_CS,
	X86_SEG_SS,
	X86_SEG_DS,
	X86_SEG_FS,
	X86_SEG_GS,

	// Miscellaneous
	X86_REG_FLAGS,
	X86_REG_EFLAGS,
	AMD64_REG_RFLAGS,
	X86_REG_IP,
	X86_REG_EIP,
	AMD64_REG_RIP,

	// Test registers
	X86_REG_TR0 = X86_TEST_OFFSET,
	X86_REG_TR1,
	X86_REG_TR2,
	X86_REG_TR3,
	X86_REG_TR4,
	X86_REG_TR5,
	X86_REG_TR6,
	X86_REG_TR7,
	X86_REG_TR8,
	X86_REG_TR9,
	X86_REG_TR10,
	X86_REG_TR11,
	X86_REG_TR12,
	X86_REG_TR13,
	X86_REG_TR14,
	X86_REG_TR15,

	// Control registers
	X86_REG_CR0=X86_CONTROL_OFFSET,
	X86_REG_CR1,
	X86_REG_CR2,
	X86_REG_CR3,
	X86_REG_CR4,
	X86_REG_CR5,
	X86_REG_CR6,
	X86_REG_CR7,
	X86_REG_CR8,
	X86_REG_CR9,
	X86_REG_CR10,
	X86_REG_CR11,
	X86_REG_CR12,
	X86_REG_CR13,
	X86_REG_CR14,
	X86_REG_CR15,

	// Debug registers
	X86_REG_DR0=X86_DEBUG_OFFSET,
	X86_REG_DR1,
	X86_REG_DR2,
	X86_REG_DR3,
	X86_REG_DR4,
	X86_REG_DR5,
	X86_REG_DR6,
	X86_REG_DR7,
	X86_REG_DR8,
	X86_REG_DR9,
	X86_REG_DR10,
	X86_REG_DR11,
	X86_REG_DR12,
	X86_REG_DR13,
	X86_REG_DR14,
	X86_REG_DR15,

	// FPU registers
	X86_REG_ST0=X86_FPU_OFFSET,
	X86_REG_ST1,
	X86_REG_ST2,
	X86_REG_ST3,
	X86_REG_ST4,
	X86_REG_ST5,
	X86_REG_ST6,
	X86_REG_ST7,

	// MMX registers
	X86_REG_MM0=X86_MMX_OFFSET,
	X86_REG_MM1,
	X86_REG_MM2,
	X86_REG_MM3,
	X86_REG_MM4,
	X86_REG_MM5,
	X86_REG_MM6,
	X86_REG_MM7,

	// XMM registers
	X86_REG_XMM0=X86_XMM_OFFSET,
	X86_REG_XMM1,
	X86_REG_XMM2,
	X86_REG_XMM3,
	X86_REG_XMM4,
	X86_REG_XMM5,
	X86_REG_XMM6,
	X86_REG_XMM7,

	// 8-bit registers
	X86_REG_AL=X86_8BIT_OFFSET,
	X86_REG_CL,
	X86_REG_DL,
	X86_REG_BL,
	X86_REG_AH,
	X86_REG_CH,
	X86_REG_DH,
	X86_REG_BH,

	// 16-bit registers
	X86_REG_AX=X86_16BIT_OFFSET,
	X86_REG_CX,
	X86_REG_DX,
	X86_REG_BX,
	X86_REG_SP,
	X86_REG_BP,
	X86_REG_SI,
	X86_REG_DI,

	// 32-bit registers
	X86_REG_EAX=X86_32BIT_OFFSET,
	X86_REG_ECX,
	X86_REG_EDX,
	X86_REG_EBX,
	X86_REG_ESP,
	X86_REG_EBP,
	X86_REG_ESI,
	X86_REG_EDI,

	// AMD64 8-bit registers
	AMD64_REG_AL=AMD64_8BIT_OFFSET,
	AMD64_REG_CL,
	AMD64_REG_DL,
	AMD64_REG_BL,
	AMD64_REG_SPL,
	AMD64_REG_BPL,
	AMD64_REG_SIL,
	AMD64_REG_DIL,
	AMD64_REG_R8B,
	AMD64_REG_R9B,
	AMD64_REG_R10B,
	AMD64_REG_R11B,
	AMD64_REG_R12B,
	AMD64_REG_R13B,
	AMD64_REG_R14B,
	AMD64_REG_R15B,

	// AMD64 16-bit registers
	AMD64_REG_AX=AMD64_16BIT_OFFSET,
	AMD64_REG_CX,
	AMD64_REG_DX,
	AMD64_REG_BX,
	AMD64_REG_SP,
	AMD64_REG_BP,
	AMD64_REG_SI,
	AMD64_REG_DI,
	AMD64_REG_R8W,
	AMD64_REG_R9W,
	AMD64_REG_R10W,
	AMD64_REG_R11W,
	AMD64_REG_R12W,
	AMD64_REG_R13W,
	AMD64_REG_R14W,
	AMD64_REG_R15W,

	// AMD64 32-bit registers
	AMD64_REG_EAX=AMD64_32BIT_OFFSET,
	AMD64_REG_ECX,
	AMD64_REG_EDX,
	AMD64_REG_EBX,
	AMD64_REG_ESP,
	AMD64_REG_EBP,
	AMD64_REG_ESI,
	AMD64_REG_EDI,
	AMD64_REG_R8D,
	AMD64_REG_R9D,
	AMD64_REG_R10D,
	AMD64_REG_R11D,
	AMD64_REG_R12D,
	AMD64_REG_R13D,
	AMD64_REG_R14D,
	AMD64_REG_R15D,

	// AMD64 64-bit registers
	AMD64_REG_RAX=AMD64_64BIT_OFFSET,
	AMD64_REG_RCX,
	AMD64_REG_RDX,
	AMD64_REG_RBX,
	AMD64_REG_RSP,
	AMD64_REG_RBP,
	AMD64_REG_RSI,
	AMD64_REG_RDI,
	AMD64_REG_R8,
	AMD64_REG_R9,
	AMD64_REG_R10,
	AMD64_REG_R11,
	AMD64_REG_R12,
	AMD64_REG_R13,
	AMD64_REG_R14,
	AMD64_REG_R15
} X86_REGISTER;

typedef enum _X86_TEST_REGISTER
{
	REG_TR0=0,
	REG_TR1,
	REG_TR2,
	REG_TR3,
	REG_TR4,
	REG_TR5,
	REG_TR6,
	REG_TR7,
	REG_TR8,
	REG_TR9,
	REG_TR10,
	REG_TR11,
	REG_TR12,
	REG_TR13,
	REG_TR14,
	REG_TR15
} X86_TEST_REGISTER;

typedef enum _X86_CONTROL_REGISTER
{
	REG_CR0,
	REG_CR1,
	REG_CR2,
	REG_CR3,
	REG_CR4,
	REG_CR5,
	REG_CR6,
	REG_CR7,
	REG_CR8,
	REG_CR9,
	REG_CR10,
	REG_CR11,
	REG_CR12,
	REG_CR13,
	REG_CR14,
	REG_CR15
} X86_CONTROL_REGISTER;

typedef enum _X86_DEBUG_REGISTER
{
	REG_DR0,
	REG_DR1,
	REG_DR2,
	REG_DR3,
	REG_DR4,
	REG_DR5,
	REG_DR6,
	REG_DR7,
	REG_DR8,
	REG_DR9,
	REG_DR10,
	REG_DR11,
	REG_DR12,
	REG_DR13,
	REG_DR14,
	REG_DR15
} X86_DEBUG_REGISTER;

typedef enum _X86_MMX_REGISTER
{
	REG_MM0=0,
	REG_MM1=1,
	REG_MM2=2,
	REG_MM3=3,
	REG_MM4=4,
	REG_MM5=5,
	REG_MM6=6,
	REG_MM7=7
} X86_MMX_REGISTER;

typedef enum _X86_SSE_REGISTER
{
	REG_XMM0=0,
	REG_XMM1=1,
	REG_XMM2=2,
	REG_XMM3=3,
	REG_XMM4=4,
	REG_XMM5=5,
	REG_XMM6=6,
	REG_XMM7=7
} X86_SSE_REGISTER;

typedef enum _X86_FPU_REGISTER
{
	REG_ST0=0,
	REG_ST1=1,
	REG_ST2=2,
	REG_ST3=3,
	REG_ST4=4,
	REG_ST5=5,
	REG_ST6=6,
	REG_ST7=7
} X86_FPU_REGISTER;

typedef enum _X86_8BIT_REGISTER
{
	REG_AL = 0,
	REG_CL = 1,
	REG_DL = 2,
	REG_BL = 3,
	REG_AH = 4,
	REG_CH = 5,
	REG_DH = 6,
	REG_BH = 7
} X86_8BIT_REGISTER;

typedef enum _X86_16BIT_REGISTER
{
	REG_AX = 0,
	REG_CX = 1,
	REG_DX = 2,
	REG_BX = 3,
	REG_SP = 4,
	REG_BP = 5,
	REG_SI = 6,
	REG_DI = 7
} X86_16BIT_REGISTER;

typedef enum _X86_32BIT_REGISTER
{
	REG_EAX = 0,
	REG_ECX = 1,
	REG_EDX = 2,
	REG_EBX = 3,
	REG_ESP = 4,
	REG_EBP = 5,
	REG_ESI = 6,
	REG_EDI = 7
} X86_32BIT_REGISTER;

typedef enum _X86_SEGMENT
{
	SEG_ES = 0,
	SEG_CS = 1,
	SEG_SS = 2,
	SEG_DS = 3,
	SEG_FS = 4,
	SEG_GS = 5,
	SEG_MAX = 6
} X86_SEGMENT;

extern char *X86_Registers[];

#pragma pack(push,1)
typedef struct _MODRM
{
	U8 mod : 2;
	U8 reg : 3;
	U8 rm : 3;
} MODRM;
typedef struct _SIB
{
	U8 scale : 2;
	U8 index : 3;
	U8 base : 3;
} SIB;
typedef struct _REX
{
	U8 unused : 4; // bits 4,5,6,7
	U8 w : 1; // bit 3
	U8 r : 1; // bit 2
	U8 x : 1; // bit 1
	U8 b : 1; // bit 0
} REX;
typedef struct _REX_MODRM
{
	U8 reg : 4;
	U8 rm : 4;
} REX_MODRM;
typedef struct _REX_SIB
{
	U8 index : 4;
	U8 base : 4;
} REX_SIB;
#pragma pack(pop)

//
// Properties:
// If an operand is OP_COND_EXEC, it means that it is executed only if the pre-conditions are met.
//
// If if an instruction has one or more OP_COND_DST operands, then the actions are determined by
// whether the Opcode.Preconditions are met or not. If all the COND_* flags in Opcode.Preconditions 
// are true, then the results are determined by ResultsIfTrue. If the preconditions are not met, then
// the results are determined by ResultsIfFalse.
//
// If Preconditions == NOCOND, then results in ResultsIfTrue are unconditional and ResultsIfFalse
// is ignored
//
typedef struct _X86_OPCODE
{
	struct _X86_OPCODE *Table;
	CPU_TYPE CPU; // minimum CPU (starting with i386)
	U32 MnemonicFlags;
	char Mnemonic[X86_MAX_INSTRUCTION_LEN+1];
	U32 OperandFlags[X86_MAX_OPERANDS];
	U32 Preconditions;
	U32 FlagsChanged; // changes in flags
	U32 ResultsIfTrue; // results if Preconditions are met
	U32 ResultsIfFalse; // results if Preconditions are not met
} X86_OPCODE;

typedef struct _X86_INSTRUCTION
{
	struct _INSTRUCTION *Instruction; // the generic instruction format representing this instruction

	X86_OPCODE Opcode;

	U8 sib_b;
	U8 modrm_b;
	MODRM modrm;
	SIB sib;
	U8 rex_b;
	REX rex;
	REX_MODRM rex_modrm;
	REX_SIB rex_sib;

	X86_SEGMENT DstSegment;
	union
	{
		X86_SEGMENT Segment;
		DWORD Selector;
	};

	// NOTE: these are for internal use, use Instruction->Operands[]
	//
	// If DstRegAddressing or SrcRegAddressing = TRUE then BaseRegister is the base register
	// It is the operand represented by SIBOperand
	//
	// The operand indices of the destination operands is in DstOpIndex[0 to DstOpCount-1]
	// The operand indices of the source operands is in SrcOpIndex[0 to SrcOpCount-1]
	//
	// These are used both for instructions like xadd/xchg (where both operands are source/destination)
	// and to represent implicit registers (e.g., cmpxchg)

	U8 SrcOpIndex[3];
	U8 DstOpIndex[3];

	// Addressing mode:
	// If DstRegAddressing = TRUE, then these apply to DstReg
	// If SrcRegAddressing = TRUE, then this applies to SrcReg[AddressIndex]
	// If both are false, then SrcReg and DstReg are not addresses
	X86_REGISTER BaseRegister;
	X86_REGISTER IndexRegister;
	
	U8 Scale;
	U8 HasDefault64Operand : 1;
	U8 HasOperandSizePrefix : 1;
	U8 HasAddressSizePrefix : 1;
	U8 HasSegmentOverridePrefix : 1;
	U8 HasLockPrefix : 1;
	U8 HasRepeatWhileEqualPrefix : 1;
	U8 HasRepeatWhileNotEqualPrefix : 1;
	U8 HasBranchTakenPrefix : 1;
	U8 HasBranchNotTakenPrefix : 1;
	U8 HasDstAddressing : 1;
	U8 HasSrcAddressing : 1; 
	U8 HasModRM : 1;
	U8 HasBaseRegister : 1;
	U8 HasIndexRegister : 1;
	U8 HasFullDisplacement : 1;
	U8 HasDstSegment : 1; // used for ins/cmps/scas/movs/etc which have 2 segments
	U8 DstAddressIndex : 2; // DstOpIndex[DstAddressIndex]
	U8 SrcAddressIndex : 2; // SrcOpIndex[SrcAddressIndex]
	U8 DstOpCount : 2;
	U8 SrcOpCount : 2;
	U8 OperandSize : 4;
	U8 AddressSize : 4;
	U8 Relative : 1;
	U8 HasSelector : 1; // segment is actually a selector
	U8 Group : 5;

	S64 Displacement;

} X86_INSTRUCTION;

#define OPTYPE_a    0x01000000
#define OPTYPE_b    0x02000000
#define OPTYPE_d    0x03000000
#define OPTYPE_p    0x04000000
#define OPTYPE_q    0x05000000
#define OPTYPE_dt   0x06000000
#define OPTYPE_v    0x07000000
#define OPTYPE_w    0x08000000
#define OPTYPE_ps   0x09000000 // packed 128-bit single real
#define OPTYPE_pd   0x0A000000 // packed 128-bit double real
#define OPTYPE_pb	  0x0B000000 // packed BCD (10 bytes, 18-bit precision)
#define OPTYPE_ss   0x0C000000 // scalar single real
#define OPTYPE_sd   0x0D000000 // scalar double real
#define OPTYPE_se   0x0E000000 // scalar extended real
#define OPTYPE_fev  0x0F000000 // FPU environment (28 bytes if 32-bit modes, 14 bytes in 16-bit mode)
#define OPTYPE_fst1 0x10000000 // FPU state (108 bytes in 32-bit modes, 94 bytes in 16-bit real mode)
#define OPTYPE_fst2 0x11000000 // FPU/MMX/XMM/MXCSR state (512 bytes)
#define OPTYPE_z	  0x12000000
#define OPTYPE_o	  0x13000000
#define OPTYPE_dq   0x14000000 // OPTYPE_d or OPTYPE_o
#define OPTYPE_mw   0x15000000 // word if memory, register size otherwise
#define OPTYPE_sso  0x16000000 // OPTYPE_ss or OPTYPE_o
#define OPTYPE_sdo  0x17000000 // OPTYPE_ss or OPTYPE_o
#define OPTYPE_cpu  0x18000000 // pointer to CPU state structure
#define OPTYPE_lea  0x19000000 // size set by other operand

//////////////////////////////////////////////////////////
// Instruction setup
//////////////////////////////////////////////////////////

#define APPLY_OFFSET(addr) \
{ \
	switch (X86Instruction->OperandSize) \
	{ \
		case 8: addr = ((U64)(addr + Instruction->VirtualAddressDelta)); break; \
		case 4: addr = (U64)((U32)(addr + Instruction->VirtualAddressDelta)); break; \
		case 2: addr = (U64)((U8)(addr + Instruction->VirtualAddressDelta)); break; \
		default: assert(0); break; \
	} \
}

////////////////////////////////////////////////////////////
// Formatting
// You can change these to whatever you prefer
////////////////////////////////////////////////////////////

#define X86_WRITE_OPFLAGS() \
	if (Flags & DISASM_SHOWFLAGS) \
	{ \
		APPENDB('{'); \
		assert(Operand->Flags & (OP_EXEC|OP_SRC|OP_DST)); \
		if (Operand->Flags & OP_IPREL) APPENDB('r'); \
		if (Operand->Flags & OP_FAR) APPENDB('f'); \
		if (Operand->Flags & OP_CONDR) APPENDB('c'); \
		if (Operand->Flags & OP_EXEC) APPENDB('X'); \
		else if (Operand->Flags & OP_SRC) APPENDB('R'); \
		if (Operand->Flags & OP_CONDW) APPENDB('c'); \
		if (Operand->Flags & OP_DST) APPENDB('W'); \
		if (Operand->Flags & OP_SYS) APPENDB('S'); \
		if (Operand->Flags & OP_ADDRESS) APPENDB('A'); \
		if (Operand->Flags & OP_PARAM) APPENDB('P'); \
		if (Operand->Flags & OP_LOCAL) APPENDB('L'); \
		if (Operand->Flags & OP_GLOBAL) APPENDB('G'); \
		APPENDB('}'); \
	}

#define X86_WRITE_IMMEDIATE() \
{ \
	switch (Operand->Length) \
	{ \
		case 8: \
			APPEND(OPCSTR, SIZE_LEFT, "0x%02I64X=", Operand->Value_U64); \
			if (Operand->Value_S64 >= 0 || !(Operand->Flags & OP_SIGNED)) APPEND(OPCSTR, SIZE_LEFT, "%I64u", Operand->Value_U64); \
			/*else APPEND(OPCSTR, SIZE_LEFT, "-0x%02I64X=%I64d", -Operand->Value_S64, Operand->Value_S64);*/ \
			else APPEND(OPCSTR, SIZE_LEFT, "%I64d", Operand->Value_S64); \
			break; \
		case 4: \
			APPEND(OPCSTR, SIZE_LEFT, "0x%02lX=", (U32)Operand->Value_U64); \
			if (Operand->Value_S64 >= 0 || !(Operand->Flags & OP_SIGNED)) APPEND(OPCSTR, SIZE_LEFT, "%lu", (U32)Operand->Value_U64); \
			/*else APPEND(OPCSTR, SIZE_LEFT, "-0x%02lX=%ld", (U32)-Operand->Value_S64, (S32)Operand->Value_S64);*/ \
			else APPEND(OPCSTR, SIZE_LEFT, "%ld", (S32)Operand->Value_S64); \
			break; \
		case 2: \
			APPEND(OPCSTR, SIZE_LEFT, "0x%02X=", (U16)Operand->Value_U64); \
			if (Operand->Value_S64 >= 0 || !(Operand->Flags & OP_SIGNED)) APPEND(OPCSTR, SIZE_LEFT, "%u", (U16)Operand->Value_U64); \
			/*else APPEND(OPCSTR, SIZE_LEFT, "-0x%02X=%d", (U16)-Operand->Value_S64, (S16)Operand->Value_S64);*/ \
			else APPEND(OPCSTR, SIZE_LEFT, "%d", (S16)Operand->Value_S64); \
			break; \
		case 1: \
			APPEND(OPCSTR, SIZE_LEFT, "0x%02X=", (U8)Operand->Value_U64); \
			if (Operand->Value_S64 >= 0 || !(Operand->Flags & OP_SIGNED)) APPEND(OPCSTR, SIZE_LEFT, "%u", (U8)Operand->Value_U64); \
			/*else APPEND(OPCSTR, SIZE_LEFT, "-0x%02X=%d", (U8)-Operand->Value_S64, (S8)Operand->Value_S64);*/ \
			else APPEND(OPCSTR, SIZE_LEFT, "%d", (S8)Operand->Value_S64); \
			break; \
		default: assert(0); break; \
	} \
}

#define X86_WRITE_ABSOLUTE_DISPLACEMENT() \
{  \
	switch (X86Instruction->AddressSize) \
	{ \
		case 8: \
			APPEND(OPCSTR, SIZE_LEFT, "0x%04I64X", X86Instruction->Displacement); \
			break; \
		case 4: \
			APPEND(OPCSTR, SIZE_LEFT, "0x%04lX", (U32)X86Instruction->Displacement); \
			break; \
		case 2: \
			APPEND(OPCSTR, SIZE_LEFT, "0x%04X", (U16)X86Instruction->Displacement); \
			break; \
		default: assert(0); break; \
	} \
}

#define X86_WRITE_RELATIVE_DISPLACEMENT64() \
	if (X86Instruction->Displacement >= 0) APPEND(OPCSTR, SIZE_LEFT, "+0x%02I64X", X86Instruction->Displacement); \
	else APPEND(OPCSTR, SIZE_LEFT, "-0x%02I64X", -X86Instruction->Displacement);

#define X86_WRITE_RELATIVE_DISPLACEMENT32() \
	if (X86Instruction->Displacement >= 0) APPEND(OPCSTR, SIZE_LEFT, "+0x%02lX", (U32)X86Instruction->Displacement); \
	else APPEND(OPCSTR, SIZE_LEFT, "-0x%02lX", (U32)-X86Instruction->Displacement);

#define X86_WRITE_RELATIVE_DISPLACEMENT16() \
	if (X86Instruction->Displacement >= 0) APPEND(OPCSTR, SIZE_LEFT, "+0x%02X", (U16)X86Instruction->Displacement); \
	else APPEND(OPCSTR, SIZE_LEFT, "-0x%02X", (U16)-X86Instruction->Displacement);

#define X86_WRITE_RELATIVE_DISPLACEMENT() \
{  \
	switch (X86Instruction->AddressSize) \
	{ \
		case 8: \
			X86_WRITE_RELATIVE_DISPLACEMENT64() \
			break; \
		case 4: \
			X86_WRITE_RELATIVE_DISPLACEMENT32() \
			break; \
		case 2: \
			X86_WRITE_RELATIVE_DISPLACEMENT16() \
			break; \
		default: assert(0); break; \
	} \
}

#define X86_WRITE_IP_OFFSET(op) \
{ \
	switch (X86Instruction->OperandSize) \
	{ \
		case 8: \
			APPENDS("[rip+ilen"); \
			assert((op)->TargetAddress); \
			X86_WRITE_RELATIVE_DISPLACEMENT64() \
			APPEND(OPCSTR, SIZE_LEFT, "]=0x%04I64X", (op)->TargetAddress+Instruction->VirtualAddressDelta); \
			break; \
		case 4: \
			APPENDS("[eip+ilen"); \
			assert((op)->TargetAddress); \
			X86_WRITE_RELATIVE_DISPLACEMENT32() \
			APPEND(OPCSTR, SIZE_LEFT, "]=0x%04lX", (U32)((op)->TargetAddress+Instruction->VirtualAddressDelta)); \
			break; \
		case 2: \
			APPENDS("[ip+ilen"); \
			X86_WRITE_RELATIVE_DISPLACEMENT16() \
			APPEND(OPCSTR, SIZE_LEFT, "]=0x%04X", (U16)((op)->TargetAddress+Instruction->VirtualAddressDelta)); \
			break; \
		default: assert(0); break; \
	} \
}

#define X86_WRITE_OFFSET(op) \
{ \
	assert((op)->Length <= 8); \
	if (X86Instruction->HasSelector) \
	{ \
		assert((op)->Flags & OP_FAR); \
		APPEND(OPCSTR, SIZE_LEFT, "%s 0x%02X:[", DataSizes[((op)->Length >> 1)], X86Instruction->Selector); \
	} \
	else \
	{ \
		assert(!((op)->Flags & OP_FAR)); \
		assert(X86Instruction->Segment < SEG_MAX) ; \
		APPEND(OPCSTR, SIZE_LEFT, "%s %s:[", DataSizes[((op)->Length >> 1)], Segments[X86Instruction->Segment]); \
	} \
	X86_WRITE_ABSOLUTE_DISPLACEMENT() \
	APPENDB(']'); \
}
////////////////////////////////////////////////////////////
// Prologue support
////////////////////////////////////////////////////////////

typedef struct _PROLOGUE
{
	char *Data;
	U32 Length;
} PROLOGUE;

PROLOGUE StandardPrologues[] =
{
	{ "\x55\x8b\xec", 3 },
	{ "\x55\x89\xe5", 3 },
	{ "\x83\xec", 2 },
	{ "\x81\xec", 2 },
	// TODO: add AMD64 prologues
	// TODO: add VS2003/VS2003 prologues
	// TODO: add any unique prologues from other compilers
	{ NULL, 0 }
};

typedef struct DECLSPEC_ALIGN(16) _S128
{
    U64 Low;
    S64 High;
} S128;
typedef struct DECLSPEC_ALIGN(16) _U128
{
    U64 Low;
    U64 High;
} U128;

typedef struct _INSTRUCTION_OPERAND
{
	U32 Flags;
	U8 Type : 6;
	U8 Unused : 2;
	U16 Length;
	

	// If non-NULL, this indicates the target address of the instruction (e.g., a branch or
	// a displacement with no base register). However, this address is only reliable if the
	// image is mapped correctly (e.g., the executable is mapped as an image and fixups have
	// been applied if it is not at its preferred image base).
	//
	// If disassembling a 16-bit DOS application, TargetAddress is in the context of 
	// X86Instruction->Segment. For example, if TargetAddress is the address of a code branch, 
	// it is in the CS segment (unless X86Instruction->HasSegmentOverridePrefix is set). If 
	// TargetAddress is a data pointer, it is in the DS segment (unless 
	// X86Instruction->HasSegmentOverridePrefix is set)
	U64 TargetAddress;
	U32 Register;

	union
	{
		// All 8/16/32-bit operands are extended to 64-bits automatically
		// If you want to downcast, check whether Flags & OP_SIGNED is set
		// Like this:
		// U32 GetOperand32(OPERAND *Operand)
		// {
		//	if (Operand->Flags & OP_SIGNED) return (S32)Operand->Value_S64;
		//	else return (U32)Operand->Value_U64;
		//}
		U64 Value_U64;
		S64 Value_S64;
		U128 Value_U128;
		U128 Float128;
		U8 Float80[80];
		U8 BCD[10];
	};
} INSTRUCTION_OPERAND;

typedef struct _INSTRUCTION
{
	U32 Initialized;
	struct _DISASSEMBLER *Disassembler;

	char String[MAX_OPCODE_DESCRIPTION];
	U8 StringIndex;
	U64 VirtualAddressDelta;

	U32 Groups; // ITYPE_EXEC, ITYPE_ARITH, etc. -- NOTE groups can be OR'd together
	INSTRUCTION_TYPE Type; // ITYPE_ADD, ITYPE_RET, etc. -- NOTE there is only one possible type

	U8 *Address;
	U8 *OpcodeAddress;
	U32 Length;

	U8 Prefixes[MAX_PREFIX_LENGTH];
	U32 PrefixCount;

	U8 LastOpcode; // last byte of opcode
	U8 OpcodeBytes[MAX_OPCODE_LENGTH];
	U32 OpcodeLength; // excludes any operands and prefixes

	INSTRUCTION_OPERAND Operands[MAX_OPERAND_COUNT];
	U32 OperandCount;

	X86_INSTRUCTION X86;

	DATA_REFERENCE DataSrc;
	DATA_REFERENCE DataDst;
	CODE_BRANCH CodeBranch;

	// Direction depends on which direction the stack grows
	// For example, on x86 a push results in StackChange < 0 since the stack grows down
	// This is only relevant if (Group & ITYPE_STACK) is true
	//
	// If Groups & ITYPE_STACK is set but StackChange = 0, it means that the change
	// couldn't be determined (non-constant)
	LONG StackChange;

	// Used to assist in debugging
	// If set, the current instruction is doing something that requires special handling
	// For example, popf can cause tracing to be disabled

	U8 StringAligned : 1; // internal only
	U8 NeedsEmulation : 1; // instruction does something that re
	U8 Repeat : 1; // instruction repeats until some condition is met (e.g., REP prefix on X86)
	U8 ErrorOccurred : 1; // set if instruction is invalid
	U8 AnomalyOccurred : 1; // set if instruction is anomalous
	U8 LastInstruction : 1; // tells the iterator callback it is the last instruction
	U8 CodeBlockFirst: 1;
	U8 CodeBlockLast : 1;
} INSTRUCTION;

typedef struct _DISASSEMBLER
{
	U32 Initialized;
	ARCHITECTURE_TYPE ArchType;
	ARCHITECTURE_FORMAT_FUNCTIONS *Functions;
	INSTRUCTION Instruction;
	U32 Stage1Count; // GetInstruction called
	U32 Stage2Count; // Opcode fully decoded
	U32 Stage3CountNoDecode;   // made it through all checks when DISASM_DECODE is not set
	U32 Stage3CountWithDecode; // made it through all checks when DISASM_DECODE is set
} DISASSEMBLER;

#define DISASM_DISASSEMBLE         (1<<1)
#define DISASM_DECODE              (1<<2)
#define DISASM_SUPPRESSERRORS      (1<<3)
#define DISASM_SHOWFLAGS           (1<<4)
#define DISASM_ALIGNOUTPUT         (1<<5)
#define DISASM_DISASSEMBLE_MASK (DISASM_ALIGNOUTPUT|DISASM_SHOWBYTES|DISASM_DISASSEMBLE)

//////////////////////////////////////////////////////////////////////
// Global variables
//////////////////////////////////////////////////////////////////////
extern ARCHITECTURE_FORMAT_FUNCTIONS X86;
ARCHITECTURE_FORMAT SupportedArchitectures[] =
{
	{ ARCH_X86,	&X86 },
	{ ARCH_X86_16, &X86 },
	{ ARCH_X64,	&X86 },
	{ ARCH_UNKNOWN, NULL }
};

typedef struct _DISASM_ARG_INFO
{
	INSTRUCTION *MatchedInstruction;
	BOOL MatchPrefix;
	U8 *Opcode;
	U32 OpcodeLength;
	INSTRUCTION_TYPE InstructionType;
	U32 Count;
} DISASM_ARG_INFO;
#include "disasm_x86_tables.h"
//////////////////////////////////////////////////////////////////////
// Function prototypes
//////////////////////////////////////////////////////////////////////
BOOL InitInstruction(INSTRUCTION *Instruction, DISASSEMBLER *Disassembler);
struct _ARCHITECTURE_FORMAT *GetArchitectureFormat(ARCHITECTURE_TYPE Type);
////////////////////////////////////////////////////////////////////////////////////
// Exported functions
////////////////////////////////////////////////////////////////////////////////////

// Instruction setup
BOOL X86_InitInstruction(struct _INSTRUCTION *Instruction);
void X86_CloseInstruction(struct _INSTRUCTION *Instruction);

// Instruction translator
BOOL X86_TranslateInstruction(struct _INSTRUCTION *Instruction, BOOL Verbose);

// Instruction decoder
BOOL X86_GetInstruction(struct _INSTRUCTION *Instruction, U8 *Address, DWORD Flags);

// Function finding
U8 *X86_FindFunctionByPrologue(struct _INSTRUCTION *Instruction, U8 *StartAddress, U8 *EndAddress, DWORD Flags);

ARCHITECTURE_FORMAT_FUNCTIONS X86 = 
{
	X86_InitInstruction, 
	NULL,
	X86_GetInstruction,
	X86_FindFunctionByPrologue 
};

void OutputBounds(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
void OutputGeneral(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
void OutputDescriptor(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
void OutputSegOffset(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
void OutputPackedReal(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
void OutputPackedBCD(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
void OutputScalarReal(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
void OutputScalarGeneral(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
void OutputFPUEnvironment(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
void OutputFPUState(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
void OutputCPUState(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);


void OutputAddress(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
//void OutputBounds(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
//void OutputGeneral(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
//void OutputDescriptor(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
//void OutputPackedReal(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
//void OutputPackedBCD(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
//void OutputScalarReal(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
//void OutputScalarGeneral(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
//void OutputFPUEnvironment(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
//void OutputFPUState(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
//void OutputCPUState(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
//void OutputSegOffset(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);


////////////////////////////////////////////////////////////////////////
// Internal functions
////////////////////////////////////////////////////////////////////////

#ifdef TEST_DISASM // TODO: remove
U32 X86_GetLength(INSTRUCTION *Instruction, U8 *Address);
#endif

INTERNAL BOOL IsValidLockPrefix(X86_INSTRUCTION *Instruction, U8 Opcode, U32 OpcodeLength, U8 Group, U8 OpcodeExtension);
INTERNAL U8 *SetOperands(INSTRUCTION *Instruction, U8 *Address, U32 Flags);
INTERNAL U8 *SetModRM32(INSTRUCTION *Instruction, U8 *Address, INSTRUCTION_OPERAND *Operand, U32 OperandIndex, BOOL SuppressErrors);
INTERNAL U8 *SetModRM16(INSTRUCTION *Instruction, U8 *Address, INSTRUCTION_OPERAND *Operand, U32 OperandIndex, BOOL SuppressErrors);
INTERNAL U8 *SetSIB(INSTRUCTION *Instruction, U8 *Address, INSTRUCTION_OPERAND *Operand, U32 OperandIndex, BOOL SuppressErrors);
INTERNAL U64 ApplyDisplacement(U64 Address, INSTRUCTION *Instruction);


typedef void (*OUTPUT_OPTYPE)(INSTRUCTION *Instruction, INSTRUCTION_OPERAND *Operand, U32 OperandIndex);
#define OPTYPE_SHIFT 24
#define MAX_OPTYPE_INDEX 26
OUTPUT_OPTYPE OptypeHandlers[] =
{
	NULL,
	OutputBounds,         // 01 OPTYPE_a
	OutputGeneral,        // 02 OPTYPE_b
	OutputGeneral,        // 03 OPTYPE_d
	OutputSegOffset,      // 04 OPTYPE_p
	OutputGeneral,        // 05 OPTYPE_q
	OutputDescriptor,     // 06 OPTYPE_dt
	OutputGeneral,        // 07 OPTYPE_v
	OutputGeneral,        // 08 OPTYPE_w
	OutputPackedReal,     // 09 OPTYPE_ps
	OutputPackedReal,     // 0A OPTYPE_pd
	OutputPackedBCD,      // 0B OPTYPE_pb
	OutputScalarReal,     // 0C OPTYPE_ss
	OutputScalarReal,     // 0D OPTYPE_sd
	OutputScalarReal,     // 0E OPTYPE_se
	OutputFPUEnvironment, // 0F OPTYPE_fev
	OutputFPUState,       // 10 OPTYPE_fst1
	OutputFPUState,       // 11 OPTYPE_fst2
	OutputGeneral,        // 12 OPTYPE_z
	OutputGeneral,        // 13 OPTYPE_o
	OutputGeneral,        // 14 OPTYPE_dq
	OutputGeneral,        // 15 OPTYPE_mw
	OutputScalarGeneral,  // 16 OPTYPE_sso
	OutputScalarGeneral,  // 17 OPTYPE_sdo
	OutputCPUState,       // 18 OPTYPE_cpu
	OutputGeneral,        // 19 OPTYPE_lea
};


BOOL InitDisassembler(DISASSEMBLER *Disassembler, ARCHITECTURE_TYPE Architecture);
void CloseDisassembler(DISASSEMBLER *Disassembler);
INSTRUCTION *GetInstruction(DISASSEMBLER *Disassembler, U64 VirtualAddress, U8 *Address, U32 Flags);
#ifdef __cplusplus
}
#endif
#endif // DISASM_H