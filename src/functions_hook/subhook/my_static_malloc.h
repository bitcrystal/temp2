#include "memory_management_functions.h"
#include "malloc_redesign.h"
#ifndef MY_STATIC_MALLOC
#define MY_STATIC_MALLOC
void * my_mmap(_my_mmap * my);
void * lite_my_mmap(void * addr, unsigned long long size);
void * my_memory_alloc(unsigned long long size);
int my_memory_free(void * addr, unsigned long long size);
void * lite_my_mmap_ex(_my_mmap * x);
void * my_memory_alloc_ex(_my_mmap * x);
int my_memory_free_ex(_my_mmap * x);
int my____alloc(void * addr, unsigned long long size, my_memory_block_mmap * mm);
int my____free(my_memory_block_mmap * mm);
void * _walloc(unsigned long long size, my_memory_block_mmap * mm);
void _wfree(my_memory_block_mmap * mm);
#ifndef OS_WIN
int get_mmap_struct_of_params(void *addr, size_t len, int prot, int flags, int fd, off_t offset,_my_mmap * x);
void * mmap_hook(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
#endif
int _create_memory_block(void * addr, unsigned long long size, my_memory_block_mmap * x);
int _destroy_memory_block(my_memory_block_mmap * x);
#endif