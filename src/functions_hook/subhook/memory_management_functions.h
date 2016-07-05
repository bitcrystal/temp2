#include "my_predef.h"
#ifndef MEMORY_MANAGEMENT_FUNCTIONS_H
#define MEMORY_MANAGEMENT_FUNCTIONS_H
#include <stdlib.h>
#define NULL 0
#define MY_VMA_READ 1
#define MY_VMA_WRITE 2
#define MY_VMA_EXEC 5
#define MY_VMA_NO_ACCESS 0
#define MY_VMA_ALL_ACCESS 7
#if (defined(OS_UNIX) || defined(OS_LINUX)) 
	#define MY_STATIC_MALLOC_UNIX
	#include <sys/types.h>
	#include <sys/mman.h>
	#ifdef MAP_ANONYMOUS
		#ifndef MAP_ANON
			#define MAP_ANON MAP_ANONYMOUS
		#endif
	#else
		#define MAP_ANONYMOUS MAP_ANON
	#endif
#elif (defined(OS_WIN))
	#include <windows.h>
#endif
typedef struct __my_mmap
{
	void * addr;
	unsigned long long size;
	unsigned long long page_size;
	unsigned long long page_offset;
	unsigned long long fixxed_page_offset;
	unsigned int flags;
	unsigned int extraflags;
	void * extraflags;
	unsigned long long extra_size;
	long long fd;
	unsigned long long offset;
	unsigned char use_fd;
	unsigned char use_offset;
	unsigned char use_fixxed_page_offset;
	char calculate_only_parameters;
	char calculate_only_common_parameters;
	char use_standard_flags;
	char use_standard_extra_flags;	
} _my_mmap;

typedef struct _my_memory_block_mmap
{
	void * allocated_memory;
	void * allocated_memory_pointer;
	unsigned long long allocated_memory_offset;
	unsigned long long allocated_memory_size;
	unsigned long long allocated_memory_reserved_size;
	unsigned char ret;
} my_memory_block_mmap;
#endif