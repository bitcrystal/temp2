#ifndef MALLOC_REDESIGN_H
#define MALLOC_REDESIGN_H
#include "memory_management_functions.h"
void* AllocateAddressSpace(void * addr, unsigned long long size);
void * AllocateAddressSpaceEx(unsigned long long size);
void* CommitMemory(void* addr, unsigned long long size);
void DecommitMemory(void* addr, unsigned long long size);
void FreeAddressSpace(void* addr, unsigned long long size);
#endif