#include "malloc_redesign.h"

void* AllocateAddressSpace(void * addr, unsigned long long size)
{
    return VirtualAlloc(addr, (SIZE_T)size, MEM_RESERVE , PAGE_NOACCESS);
}

void * AllocateAddressSpaceEx(unsigned long long size)
{
	return AllocateAddressSpace(NULL,size);
}
 
void* CommitMemory(void* addr, unsigned long long size)
{
    return VirtualAlloc(addr, (SIZE_T)size, MEM_COMMIT, PAGE_READWRITE);
}
 
void DecommitMemory(void* addr, unsigned long long size)
{
    VirtualFree((void*)addr, (SIZE_T)size, MEM_DECOMMIT);
}
 
void FreeAddressSpace(void* addr, unsigned long long size)
{
    VirtualFree((void*)addr, 0, MEM_RELEASE)
}
#else
void* AllocateAddressSpace(void * addr, unsigned long long size)
{
    void * ptr = mmap(addr, (size_t)size, PROT_NONE, MAP_PRIVATE|MAP_ANON, -1, 0);
    msync(ptr, size, MS_SYNC|MS_INVALIDATE);
    return ptr;
}

void * AllocateAddressSpaceEx(unsigned long long size)
{
	return AllocateAddressSpace(NULL,size);
}
 
void* CommitMemory(void* addr, unsigned long long size)
{
    void * ptr = mmap(addr, (size_t)size, PROT_READ|PROT_WRITE, MAP_FIXED|MAP_SHARED|MAP_ANON, -1, 0);
    msync(addr, size, MS_SYNC|MS_INVALIDATE);
    return ptr;
}
 
void DecommitMemory(void* addr, unsigned long long size)
{
    // instead of unmapping the address, we're just gonna trick 
    // the TLB to mark this as a new mapped area which, due to 
    // demand paging, will not be committed until used.
 
    mmap(addr, (size_t)size, PROT_NONE, MAP_FIXED|MAP_PRIVATE|MAP_ANON, -1, 0);
    msync(addr, (size_t)size, MS_SYNC|MS_INVALIDATE);
}
 
void FreeAddressSpace(void* addr, unsigned long long size)
{
    msync(addr, (size_t)size, MS_SYNC);
    munmap(addr, (size_t)size);
}
#endif