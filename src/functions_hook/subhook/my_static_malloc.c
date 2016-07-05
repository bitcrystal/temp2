#include "my_static_malloc.h"
#include "malloc_redesign.c"
void * my_mmap(_my_mmap * my)
{
	if(my==NULL)
		return NULL;
	int VMA_N_READ=(my->flags&MY_VMA_READ);
	int VMA_N_WRITE=(my->flags&MY_VMA_WRITE);
	int VMA_N_EXEC=(my->flags&MY_VMA_EXEC);
	int can_read = VMA_N_READ==MY_VMA_READ ? 1 : 0;
	int can_write = VMA_N_WRITE==MY_VMA_WRITE ? 1 : 0;
	int can_exec = VMA_N_EXEC==MY_VMA_EXEC ? 1 : 0;
	if(my->calculate_only_common_parameters>=1)
	{
		return NULL;
	}
	#ifdef MY_STATIC_MALLOC_UNIX
		if(my->size==0) {
			return NULL;
		}
		long long sz = (long long)sysconf(_SC_PAGESIZE);
		if(sz<=0)
		{
			return NULL;
		}
		unsigned long long szz = (unsigned long long)sz;
		my->page_offset = (my->size / szz);
		if((my->size % szz) !=0)
		{
			my->page_offset+=1;
		}
		my->page_size=szz;
		my->fixxed_page_offset=my->page_offset*my->page_size;
		if(my->use_standard_flags<=0)
		{
			if(can_read==1&&can_write==1&&can_exec==1)
			{
				my->flags=MY_VMA_ALL_ACCESS;
			} else if (can_read==0&&can_write==0&&can_exec==0)
			{
				my->flags=MY_VMA_NO_ACCESS;
			}
			if(my->flags>=MY_VMA_ALL_ACCESS) {
				my->flags=(PROT_READ|PROT_WRITE|PROT_EXEC);
			} else if(my->flags==MY_VMA_NO_ACCESS) {
				my->flags=(PROT_NONE);
			} else {
				my->flags=VMA_NO_ACCESS;
				if(can_read==1)
					my->flags|=(PROT_READ);
				if(can_write==1)
					my->flags|=(PROT_WRITE);
				if(can_exec==1)
					my->flags|=(PROT_EXEC);
			}
		}
		if(my->use_standard_extra_flags<=0)
		{
			if(my->extraflags==MY_VMA_NO_ACCESS||my->extraflags>=MY_VMA_ALL_ACCESS) {
				my->extraflags=(MAP_PRIVATE | MAP_ANONYMOUS);
			}
		}
		if(my->use_fd<=0)
		{
			my->fd=-1;
		}
		if(my->use_offset<=0)
		{
			my->offset=0;
		}
		if(my->calculate_only_paramaters>=1)
		{
			return NULL;
		}
		void * xxx = (void*)mmap((void*)my->addr, (size_t)my->size, my->flags, my->extraflags, my->fd, my->offset);
		if(xxx==NULL)
		{
			return NULL;
		} else {
			return xxx;
		}
	#elif defined(OS_WIN)
		if(my->size==0) {
			return NULL;
		}
		if(my->use_standard_flags<=0)
		{
			if(can_read==1&&can_write==1&&can_exec==1)
			{
				my->flags=MY_VMA_ALL_ACCESS;
			} else if (can_read==0&&can_write==0&&can_exec==0)
			{
				my->flags=MY_VMA_NO_ACCESS;
			}
			if(my->flags>=MY_VMA_ALL_ACCESS) {
				my->flags=(PAGE_READWRITE);
			} else if(my->flags==MY_VMA_NO_ACCESS) {
				my->flags=(PAGE_NOACCESS);
			} else {
				my->flags=VMA_NO_ACCESS;
				if(can_read==1)
					my->flags|=(PAGE_READONLY);
				if(can_write==1)
					my->flags|=(PAGE_READWRITE);
				if(can_exec==1)
					my->flags|=(PAGE_READWRITE);
			}
		}
		if(my->use_standard_extra_flags<=0)
		{
			if(can_write==1) {
				my->extraflags=(MEM_COMMIT|MEM_RESERVE);
			} else {
				my->extraflags=(MEM_RESERVE);
			}
		}
		if(my->use_fd<=0)
		{
			my->fd=-1;
		}
		if(my->use_offset<=0)
		{
			my->offset=0;
		}
		SYSTEM_INFO systemInfo;
		GetSystemInfo((LPSYSTEM_INFO)&systemInfo);
		SIZE_T sz = (systemInfo.dwPageSize);
		if(sz<=0)
		{
			return NULL;
		}
		unsigned long long szz = (unsigned long long)sz;
		my->page_offset = (my->size / szz);
		if((my->size % szz) !=0)
		{
			my->page_offset+=1;
		}
		my->page_size=szz;
		my->fixxed_page_offset=my->page_offset*my->page_size;
		if(my->calculate_only_paramaters>=1)
		{
			return NULL;
		}
		void *xxx = (void*)VirtualAlloc((LPVOID)my->addr, (SIZE_T)my->size, my->extraflags, my->flags);
		ir(xxx==NULL)
		{
			return NULL;
		} else {
			return xxx;
		}
	#else
		return NULL;
	#endif
}

void * lite_my_mmap(void * addr, unsigned long long size)
{
	_my_mmap my;
	my.addr=addr;
	my.size=size;
	my.extra=NULL;
	my.extra_size=0;
	my.flags=0;
	my.extraflags=0;
	my.use_fd=0;
	my.use_offset=0;
	my.use_fixxed_offset=0;
	my.calculate_only_parameters=0;
	my.calculate_only_common_parameters=0;
	my.use_standard_flags=0;
	my.use_standard_extra_flags=0;
	return my_mmap(&my);
}

void * lite_my_mmap_ex(_my_mmap * x)
{
	if(x==NULL
		return NULL;
	return lite_my_mmap(x->addr,x->size);
}

void * my_memory_alloc(unsigned long long size)
{
	return lite_my_mmap(NULL,size);
}

void * my_memory_alloc_ex(_my_mmap * x)
{
	if(x==NULL)
		return NULL;
	return my_memory_alloc(x->size);
}

int my_memory_free_ex(_my_mmap * x)
{
	if(x==NULL)
		return 0;
	return my_memory_free(x->addr,x->size);
}

int my_memory_free(void * addr, unsigned long long size)
{
	#ifdef MY_STATIC_MALLOC_UNIX
		if(munmap(addr,(size_t)size)!=0)
		{
			return 0;
		}
	#elif defined(OS_WIN)
		return (int)VirtualFree((LPVOID)addr,(SIZE_T)0,MEM_RELEASE);
	#else
		return 0;
	#endif
	return 1;
}

int my____alloc(void * addr, unsigned long long size, my_memory_block_mmap * mm)
{
	if(mm==NULL)
	{
		return 0;
	}
	mm->ret=0;
	void * xx = lite_my_mmap(addr,size);
	if(xx==NULL)
		return 0;
	mm->allocated_memory = xx;
	mm->allocated_memory_reserved_size=size;
	mm->allocated_memory_offset=0;
	my->allocated_memory_pointer=NULL;
	my->allocated_memory_size=0;
	mm->ret=1;
	return 1;
}

int my____free(my_memory_block_mmap * mm)
{
	if(mm==NULL)
	{
		return 0;
	}
	mm->ret=0;
	if(mm->allocated_memory_reserved_size==0||mm->allocated_memory==NULL)
	{
		return 0;
	}
	int r = my_memory_free(mm->allocated_memory,mm->allocated_memory_size);
	if(r==1)
	{
		my->ret=1;
		my->allocated_memory=NULL;
		my->allocated_memory_size=0;
		my->allocated_memory_pointer=NULL;
		my->allocated_memory_reserved_size=0;
		my->allocated_memory_offset=0;
		return 1;
	}else {
		return 0;
	}
}
void * _walloc(unsigned long long size, my_memory_block_mmap * mm)
{
	if(mm==NULL)
	{
		return NULL;
	}
	if(((mm->allocated_memory_size+size)>=mm->allocated_memory_reserved_size))
		return NULL;
	mm->allocated_memory_size+=size;
	mm->allocated_memory_pointer=(mm->allocated_memory+mm->allocated_memory_offset);
	mm->allocated_memory_offset=mm->allocated_memory_size;
	return mm->allocated_memory_pointer;
}

void _wfree(my_memory_block_mmap * mm)
{
	my____free(mm);
	return;
}
#ifndef OS_WIN
#ifdef USE_MMAP_HOOK
int get_mmap_struct_of_params(void *addr, size_t len, int prot, int flags, int fd, off_t offset,_my_mmap * x)
{
	if(x==NULL)
		return 0;
	x->extra=NULL;
	x->extra_size=0;
	x->flags=0;
	x->use_fd=1;
	x->use_offset=1;
	x->use_fixxed_offset=1;
	x->calculate_only_parameters=0;
	x->calculate_only_common_parameters=0;
	x->use_standard_flags=1;
	x->use_standard_extra_flags=1;
	x->addr=addr;
	x->size=(unsigned long long)len;
	x->flags=prot;
	x->extraflags=flags;
	x->fd=fd;
	x->offset=(unsigned long long)offset;
	return 1;
	
}
void * mmap_hook(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	_my_mmap x;
	int xy= get_mmap_struct_of_params(addr,len,prot,flags,fd,offset,&x);
	if(xy==0)
		return NULL;
	return my_mmap(&x);	
}
#else
int get_mmap_struct_of_params(void *addr, size_t len, int prot, int flags, int fd, off_t offset,_my_mmap * x)
{
	return 0;
}
void * mmap_hook(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	return mmap(addr,len,prot,flags,offset);
}
#endif
int _create_memory_block(void * addr, unsigned long long size, my_memory_block_mmap * x)
{
	if(x==NULL)
		return 0;
	x->ret=0;
	void * xx = AllocateAddressSpace(addr, size);
	if(xx==NULL)
		return 0;
	xx=CommitMemory(addr,size);
	if(xx==NULL)
		return 0;
	x->allocated_memory=xx;
	x->allocated_memory_pointer=xx;
	x->allocated_memory_offset=0;
	x->allocated_memory_size=0;
	x->allocated_memory_reserved_size=0;
	x->ret=1;
	return 1;
}

int _destroy_memory_block(my_memory_block_mmap * x)
{
	if(x==NULL)
		return 0;
	if(x->ret!=1)
		return 0;
	DecommitMemory(x->allocated_memory,x->allocated_memory_reserved_size);
	FreeAddressSpace(x->allocated_memory, x->allocated_memory_reserved_size);
	x->allocated_memory=NULL;
	x->allocated_memory_pointer=NULL;
	x->allocated_memory_offset=0;
	x->allocated_memory_size=0;
	x->allocated_memory_reserved_size=0;
	x->ret=0;
	return 1;
}
#endif
