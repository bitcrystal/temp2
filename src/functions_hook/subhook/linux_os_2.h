/* Iteration over virtual memory areas.
   Copyright (C) 2011-2012 Free Software Foundation, Inc.
   Written by Bruno Haible <bruno@clisp.org>, 2011.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef LINUX_OS_2_H
#define LINUX_OS_2_H
#include "my_predef.h"

#include <stdint.h>

#ifdef __cplusplus
#ifndef MY_EXTERN_C_DEF
#define MY_EXTERN_C_DEF
extern "C" {
#endif
#endif


/* Bit mask for the FLAGS parameter of a vma_iterate callback function.  */
#define VMA_PROT_READ    (1<<0)
#define VMA_PROT_WRITE   (1<<1)
#define VMA_PROT_EXECUTE (1<<2)
#define VMA_PROT_NONE    0
#define VMA_PROT_PRIVATE (1<<3)
#define VMA_PROT_SHARED (1<<4)
#define VMA_PROT_EXEC VMA_PROT_EXECUTE
#define VMA_PRIVATE VMA_PROT_PRIVATE
#define VMA_SHARED VMA_PROT_SHARED
#define VMA_PAGE_READONLY VMA_PROT_READ
#define VMA_PAGE_READWRITE (VMA_PROT_READ | VMA_PROT_WRITE) 
#define VMA_PAGE_EXECUTE VMA_PROT_EXEC
#define VMA_PAGE_EXECUTE_READ (VMA_PROT_READ | VMA_PROT_EXEC)
#define VMA_PAGE_EXECUTE_READWRITE (VMA_PROT_READ | VMA_PROT_WRITE | VMA_PROT_EXEC)
#define VMA_PAGE_EXECUTE_WRITECOPY (VMA_PROT_READ | VMA_PROT_WRITE | VMA_PROT_EXEC)
#define VMA_PAGE_NOACCESS VMA_PROT_NONE
#define VMA_GET_PERMS(x) (((x)&~(VMA_PROT_PRIVATE | VMA_PROT_SHARED)))

typedef int (*vma_iterate_callback_fn) (void *data,
                                        unsigned long long start, unsigned long long end,
                                        unsigned int flags);

/* Iterate over the virtual memory areas of the current process.
   If such iteration is supported, the callback is called once for every
   virtual memory area, in ascending order, with the following arguments:
     - DATA is the same argument as passed to vma_iterate.
     - START is the address of the first byte in the area, page-aligned.
     - END is the address of the last byte in the area plus 1, page-aligned.
       Note that it may be 0 for the last area in the address space.
     - FLAGS is a combination of the VMA_* bits.
   If the callback returns 0, the iteration continues.  If it returns 1,
   the iteration terminates prematurely.
   This function may open file descriptors, but does not call malloc().  */
#if VMA_ITERATE_PID_EXT
extern char * pid_to_string(unsigned int pid,unsigned int * pid_size);
extern char * get_pid_maps_string(unsigned int pid);
extern void vma_iterate_pid(vma_iterate_callback_fn callback, unsigned int my_pid, void*data);
#endif
extern void vma_iterate (vma_iterate_callback_fn callback, void *data);
/*typedef struct
{
    void * start; 
	void * end;
	int flags;
	int reserved;
} my_memory_block;

#include "vector_c.h"

extern int get_reserved_blocks(void * start_address,void * end_address, vector_c * reserved_blocks, VECTOR_C_CAP_TYPE BYTES_RESERVED_SIZE);
extern int get_free_blocks(void * start_address,void * end_address, vector_c * free_blocks, VECTOR_C_CAP_TYPE BYTES_RESERVED_SIZE);
extern int to_one_block(vector_c * blocks, my_memory_block * blockx, VECTOR_C_CAP_TYPE BYTES_RESERVED_SIZE);

#define GET_RESERVED_BLOCKS(st,en,re) get_reserved_blocks((void*)&(st),(void*)&(en),(vector_c *)&(re),((VECTOR_C_CAP_TYPE)(1024*1024*50)))
#define GET_FREE_BLOCKS(st,en,fr) get_free_blocks((void*)&(st),(void*)&(en),(vector_c *)&(fr),((VECTOR_C_CAP_TYPE)(1024*1024*50)))
#define TO_ONE_BLOCK(bl,blo) to_one_block((vector_c*)&(bl),(my_memory_block*)&(blo),((VECTOR_C_CAP_TYPE)(1024*1024*50)))
#define GET_RESERVED_BLOCKS_EX(st,en,re) get_reserved_blocks((void*)(st),(void*)(en),(vector_c *)(re),((VECTOR_C_CAP_TYPE)(1024*1024*50)))
#define GET_FREE_BLOCKS_EX(st,en,fr) get_free_blocks((void*)(st),(void*)(en),(vector_c *)(fr),((VECTOR_C_CAP_TYPE)(1024*1024*50)))
#define TO_ONE_BLOCK_EX(bl,blo) to_one_block((vector_c*)(bl),(my_memory_block*)(blo),((VECTOR_C_CAP_TYPE)(1024*1024*50)))
*/
/* The macro VMA_ITERATE_SUPPORTED indicates that vma_iterate is supported on
   this platform.
   Note that even when this macro is defined, vma_iterate() may still fail to
   find any virtual memory area, for example if /proc is not mounted.  */
#if defined __linux__ || defined __FreeBSD__ || defined __NetBSD__ || defined __sgi || defined __osf__ || (defined __APPLE__ && defined __MACH__) || (defined _WIN32 || defined __WIN32__) || defined __CYGWIN__ || defined __BEOS__ || defined __HAIKU__ || HAVE_MQUERY
# define VMA_ITERATE_SUPPORTED 1
#endif


#ifdef __cplusplus
#ifndef MY_EXTERN_C_DEF_BRACE
#define MY_EXTERN_C_DEF_BRACE
}
#endif
#endif


#endif /* _VMA_ITER_H */
