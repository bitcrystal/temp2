#ifndef WINDOWS_DEFS_C
#define WINDOWS_DEFS_C
#include "windows_defs.h"
#ifdef CO_MODE
#include "linux_defs.c"
#endif
#ifndef OS_WIN
#include "linux_os_2.h"
#include "linux_os_2.c"
#include "linux_os_3.h"
#include "linux_os_3.c"
#include <stdio.h>
static int my___find(const char * search, const char * string, unsigned long long * size_search_, unsigned long long * size_string_)
	{
		unsigned long long size_search = size_search_==NULL?0:(*size_search_);
		unsigned long long size_string = size_string_==NULL?0:(*size_string_);
		
		unsigned long long i=0;
		if(size_search==0)
		{
			while(1)
			{
				if(search[i]!=0)
				{
					size_search++;
				} else {
					break;
				}
			}
			*size_search_=size_search;
		}
		if(size_string==0)
		{
			while(1)
			{
				if(string[i]!=0)
				{
					size_string++;
				} else {
					break;
				}
			}
			*size_string_=size_string;
		}
		if(size_search>size_string)
		{
			return 0;
		} else if (size_search==size_string)
		{
			for(i=0; i < size_search; i++)
			{
				if(string[i]!=search[i])
				{
					return 0;
				}
			}
			return 1;
		}
		char string_target[size_search];
		unsigned long long string_target_i=0;
		for(i=0;i<size_string;i++)
		{
			if(string_target_i==size_search)
			{
				return 1;
			} else if(string[i]==search[string_target_i])
			{
				string_target_i++;
			} else {
				string_target_i=0;
			}
		}
		if(string_target_i==size_search)
		{
			return 1;
		} else {
			return 0;
		}
	}
	
static int my___grep(const char * filename, const char * keyword, const char * grepfile)
	{
		size_t s = 450;
		char line[500];
		char * r = NULL;
		unsigned long long cv = (unsigned long long)(&line[0]);
                r = (char*)cv;
		FILE * stream = NULL;
		FILE * stream2 = NULL;
		stream = fopen(filename,"r");
		if(stream==NULL)
			return 0;
		memset(line,0,500);
		stream2 = fopen(grepfile,"wb");
		if(stream2==NULL)
			return 0;
		memset(line,0,500);
		int x = getline(&r, &s, stream);
		unsigned long long ss;
		unsigned long long ss_;
		unsigned long long counter = 0;
		while(x>0)
		{
			if((my___find(keyword,r,&ss,&ss_))==1)
			{
					line[ss_++]='\r';
					line[ss_++]='\n';
					line[ss_++]=0;
					fwrite(line,1,(ss_),stream2);
					ss_=0;
					counter++;
			}
			x = getline(&r, &s, stream);
		}
		fclose(stream);
		fclose(stream2);
		return counter;
	}
#endif

unsigned long long MY_GET_SYSTEM_PAGE_SIZE()
{
	static unsigned long long pagesize = 0;
	if(pagesize==0)
	{
		#ifdef OS_UNIX_STRUCT
			long s=sysconf(_SC_PAGESIZE);
			if((s<=-1))
			{
                              return 0;
			}
			pagesize=(unsigned long long)s;
		#elif defined(OS_WIN)
			SYSTEM_INFO system_info;
  			GetSystemInfo (&system_info);
  			pagesize=(unsigned long long)system_info.dwPageSize; 
		#else
			pagesize=(unsigned long long)4096;
		#endif
	}
	return pagesize;
}

BOOL MY_HAS_PAGE_SIZE()
{
	if(MY_GET_SYSTEM_PAGE_SIZE()==0)
	{
		return FALSE;
	}
	return TRUE;
}

static int SET_X(int t)
{
	static int s = 0;
	//printf("%d\n",s);
	if(s==0)
	{
	//	printf("init s\n");
		s=t;
	}
	//printf("%d\n",s);
}

BOOL MY_IS_PAGE_SIZE_ALIGNED(unsigned long long value)
{
	static  unsigned long long pagesize = 0;
	if(pagesize==0)
	{
                pagesize=MY_GET_SYSTEM_PAGE_SIZE();
		if(pagesize==0)
			return FALSE;
	}
	if(((unsigned long long)(value%pagesize))==0)
	{
		return TRUE;
	} else {
		return FALSE;
	}
}

unsigned long long MY_ROUND_UP_PAGE_SIZE_MY_ALGORITHM(unsigned long long value)
{
	static  unsigned long long pagesize = 0;
	if(pagesize==0)
	{
		pagesize=MY_GET_SYSTEM_PAGE_SIZE();
		if(pagesize==0)
			return value;
	}
	unsigned long long nsize_ = (unsigned long long)(value / pagesize);
	if(((unsigned long long)(value % pagesize))!=0)
	{
		nsize_++;
	}
	nsize_=((unsigned long long)(nsize_*pagesize));
	if(nsize_<pagesize)
		nsize_=pagesize;
	return nsize_;
}

unsigned long long MY_ROUND_DOWN_PAGE_SIZE_MY_ALGORITHM(unsigned long long value)
{
        static  unsigned long long pagesize = 0;
        if(pagesize==0)
	{
                pagesize=MY_GET_SYSTEM_PAGE_SIZE();
		if(pagesize==0)
			return value;
	}
        unsigned long long nsize_ = (unsigned long long)(value / pagesize);
        return ((unsigned long long)(nsize_*pagesize));
}

unsigned long long MY_ROUND_UP_PAGE_SIZE_DEFAULT_ALGORITHM(unsigned long long value)
{
        static  unsigned long long pagesize = 0;
        if(pagesize==0)
	{
                pagesize=MY_GET_SYSTEM_PAGE_SIZE();
		if(pagesize==0)
			return value;
	}
        unsigned long long nsize_ = (unsigned long long)(value & (~(pagesize-1)));
	while(nsize_<value)
	{
		nsize_+=pagesize;
	}
        nsize_=((unsigned long long)(nsize_));
	if(nsize_<pagesize)
		nsize_=pagesize;
	return nsize_;
}


unsigned long long MY_ROUND_DOWN_PAGE_SIZE_DEFAULT_ALGORITHM(unsigned long long value)
{
        static  unsigned long long pagesize = 0;
        if(pagesize==0)
	{
                pagesize=MY_GET_SYSTEM_PAGE_SIZE();
		if(pagesize==0)
			return value;
	}
        unsigned long long nsize_ = (unsigned long long)(value & (~(pagesize-1)));
        return ((unsigned long long)(nsize_));
}

unsigned long long MY_ROUND_UP_PAGE_SIZE(unsigned long long value)
{
	#ifndef USE_OTHER_ROUND_UP_PAGE_SIZE_ALGORITHM
                return MY_ROUND_UP_PAGE_SIZE_DEFAULT_ALGORITHM(value);
        #else
                return MY_ROUND_UP_PAGE_SIZE_MY_ALGORITHM(value);
        #endif
}

unsigned long long MY_ROUND_DOWN_PAGE_SIZE(unsigned long long value)
{
	#ifndef USE_OTHER_ROUND_DOWN_PAGE_SIZE_ALGORITHM
        	return MY_ROUND_DOWN_PAGE_SIZE_DEFAULT_ALGORITHM(value);
	#else
		return MY_ROUND_DOWN_PAGE_SIZE_MY_ALGORITHM(value);
	#endif
}

void * GET_VIRTUAL_END_ADDRESS()
{
	static void * last_address = NULL;
	#if defined(OS_UNIX_STRUCT) && !defined(OS_UNIX_DEPRECATED_MODE)
		if(last_address==NULL)
		{
			struct rlimit rlim;
			if(getrlimit(RLIMIT_DATA, &rlim)==0)
			{
				last_address = (void*)(((unsigned long long)get_etext_ex())+((unsigned long long)rlim.rlim_max));
			}
			return last_address;
		} else {
			return last_address;
		}
	#else
		return last_address;
	#endif
}

void * __SBRK__WRAPPER(int increment)
{
	#if defined(OS_UNIX_STRUCT) && !defined(OS_UNIX_DEPRECATED_MODE) 
		return (void*)sbrk(increment);
	#else
		return (void*)-1;
	#endif
}

int __BRK__WRAPPER(void * p)
{
        #if defined(OS_UNIX_STRUCT) && !defined(OS_UNIX_DEPRECATED_MODE) 
                return (int)brk(p);
        #else
                return (int)-1;
        #endif
}



int vma_iterate_func(void *data,unsigned long long start, unsigned long long end,unsigned int flags){
	printf("start - %016x - end %016x\n",start,end);
	vma_it_func * x = (vma_it_func*)data;
        if((x->start_address>=start))
	{
		x->base_start_address=start;
		x->base_end_address=end;
		x->base_flags=flags;
		x->ret=1;
		return 0;
	} else {
		return 1;
	}
}

#if defined(OS_UNIX_STRUCT) && defined(USE_UNIX_IMAGE_EXTENSION_VQ)
int vma_iterate_image_func(void *data,unsigned long long start, unsigned long long end,unsigned int flags){
	vma_it_func * x = (vma_it_func*)data;
	if(x->ret==2)
	{
		if(x->end_address==start&&start<end)
		{
			x->end_address=end;
			x->base_flags |= flags;
			return 0;
		} else {
			x->ret=1;
			return 1;
		}
	}
 	
	if((x->start_address>=start))
	{
		x->base_start_address=start;
		x->base_end_address=end;
		x->base_flags=flags;
		x->ret=2;
		return 1;
	} else {
		return 0;
	}
}
#endif

int vma_iterate_full_addressing_func(void *data,unsigned long long start, unsigned long long end,unsigned int flags){
	
	 vma_it_func * x = (vma_it_func*)data;
        if((x->start_address>=start)&&(end < x->end_address))
        {
                x->base_start_address=start;
                x->base_end_address=end;
                x->base_flags=flags;
                x->ret=1;
                return 0;
        } else {
                return 1;
        }
}
#ifndef OS_WIN

       #ifndef FREE_REGION_MANAGE_SIZE
                #define FREE_REGION_MANAGE_SIZE 64
        #endif
        #ifdef FREE_REGION_MANAGE_SIZE_TYPE
                #undef FREE_REGION_MANAGE_SIZE_TYPE
        #endif
        #if FREE_REGION_MANAGE_SIZE<=0
                #undef FREE_REGION_MANAGE_SIZE
                #define FREE_REGION_MANAGE_SIZE 64
                #define FREE_REGION_MANAGE_SIZE_TYPE unsigned char
        #elif FREE_REGION_MANAGE_SIZE<256
                #define FREE_REGION_MANAGE_SIZE_TYPE unsigned char
        #elif FREE_REGION_MANAGE_SIZE<4294967296
                #define FREE_REGION_MANAGE_SIZE_TYPE unsigned long
        #else
                #define FREE_REGION_MANAGE_SIZE_TYPE unsigned long long
        #endif

        #ifndef FREE_REGION_MANAGE_OVERRIDE_FLAG
                #define FREE_REGION_MANAGE_OVERRIDE_FLAG 128
        #endif

        #ifndef FREE_REGION_MANAGE_IS_FREE_FLAG
                #define FREE_REGION_MANAGE_IS_FREE_FLAG 64
        #endif

	#ifndef FREE_REGION_MANAGE_EXTENDED_OVERRIDE_FLAG
		#define FREE_REGION_MANAGE_EXTENDED_OVERRIDE_FLAG 32
	#endif

	#ifndef FREE_REGION_MANAGE_EX_DEFINED
		#define FREE_REGION_MANAGE_EX_DEFINED
		#define FREE_REGION_MANAGE_EX(address,size,action,search_action,is_found,fp) (FREE_REGION_MANAGE((address),(size),((action)|FREE_REGION_MANAGE_OVERRIDE_FLAG|FREE_REGION_MANAGE_EXTENDED_OVERRIDE_FLAG),(search_action),(is_found),(fp),(0),(0)))
		#define FREE_REGION_MANAGE_EX_RESET() (FREE_REGION_MANAGE(0,0,0,0,1,1,0,0))
		#define FREE_REGION_MANAGE_EX_GET_POINTERS(fps,free_pointers_size_p) (FREE_REGION_MANAGE(0,0,0,0,1,1,(fps),(free_pointers_size_p)))
	#endif

	#ifdef FRM_SEARCH_FREE
		#undef FRM_SEARCH_FREE
	#endif
	#ifndef FRM_SEARCH_FREE
		#define FRM_SEARCH_FREE 1
	#endif

	#ifdef FRM_SEARCH_RESERVED
                #undef FRM_SEARCH_RESERVED
        #endif
        #ifndef FRM_SEARCH_RESERVED
                #define FRM_SEARCH_RESERVED 2
        #endif

	 #ifdef FRM_SEARCH_ADDRESS
                #undef FRM_SEARCH_ADDRESS
        #endif
        #ifndef FRM_SEARCH_ADDRESS
                #define FRM_SEARCH_ADDRESS 4
        #endif

	 #ifdef FRM_SEARCH_SIZE
                #undef FRM_SEARCH_SIZE
        #endif
        #ifndef FRM_SEARCH_SIZE
                #define FRM_SEARCH_SIZE 8
        #endif

	#ifdef FRM_SEARCH_SIZE_ADDRESS
                #undef FRM_SEARCH_SIZE_ADDRESS
        #endif

	#ifndef FRM_SEARCH_SIZE_ADDRESS
                #define FRM_SEARCH_SIZE_ADDRESS (FRM_SEARCH_ADDRESS|FRM_SEARCH_SIZE)
        #endif


	#ifdef FRM_SEARCH_ITERATE
		#undef FRM_SEARCH_ITERATE
	#endif

	#ifndef FRM_SEARCH_ITERATE
		#define FRM_SEARCH_ITERATE 16
	#endif

        #ifndef FRM_SEARCH_REMOVE
                #define FRM_SEARCH_REMOVE 32
        #endif

static int FREE_REGION_MANAGE(unsigned long long address, unsigned long long size, FREE_REGION_MANAGE_SIZE_TYPE action,unsigned char search_action,unsigned char *is_found,fpi_s ** fp,fpi_s ** fps, FREE_REGION_MANAGE_SIZE_TYPE * free_pointers_size_p)
{	
	static fpi_s free_pointers[FREE_REGION_MANAGE_SIZE+1];
        static fpi_s fpr;
        static FREE_REGION_MANAGE_SIZE_TYPE free_pointers_size=0;
        static FREE_REGION_MANAGE_SIZE_TYPE last_i=0;

	if(fps!=0&&free_pointers_size_p!=0)
	{
		*fps=&free_pointers[0];
		*free_pointers_size_p=free_pointers_size;
		return 1;
	}
        if(is_found==0||fp==0)
	{
                return 0;
	}
	if(address==0)
	{
		last_i=0;
		return 0;
	}
        FREE_REGION_MANAGE_SIZE_TYPE i = 0;
	unsigned char found=0;
	unsigned char search_free=(search_action&FRM_SEARCH_FREE)?1:0;
	unsigned char search_reserved=(search_action&FRM_SEARCH_RESERVED)?1:0;
	unsigned char search_address=(search_action&FRM_SEARCH_ADDRESS)?1:0;
	unsigned char search_size=(search_action&FRM_SEARCH_SIZE)?1:0;
	unsigned char search_iterate=(search_action&FRM_SEARCH_ITERATE)?1:0;
	unsigned char search_remove=(search_action&FRM_SEARCH_REMOVE)?1:0;
	unsigned char ex_override=(action&FREE_REGION_MANAGE_EXTENDED_OVERRIDE_FLAG)?1:0;
	action &= ~FREE_REGION_MANAGE_EXTENDED_OVERRIDE_FLAG;
	fpi_s * found_fpi=0;
	if(search_free==1&&search_reserved==1)
		search_reserved=0;
        for(i=(search_iterate==1)?last_i:0;i<free_pointers_size;i++)
     	{
	    if(((search_free==1&&free_pointers[i].is_free==0)||(search_reserved==1&&free_pointers[i].is_free==1)))
			continue;
                if(search_address==1&&search_size==1)
     		{
                        if(free_pointers[i].address==address&&free_pointers[i].size==size)
			{
				found = 1;
				found_fpi=&free_pointers[i];
				break;
			}
                } else if(search_address==1)
		{
			if(free_pointers[i].address==address)
			{
				found=1;
				found_fpi=&free_pointers[i];
				break;
			}
		} else if(search_size==1)
		{
			if(free_pointers[i].size==size)
			{
				found=1;
				found_fpi=&free_pointers[i];
				break;
			}
		}
        }
        *is_found=found;
	if(found_fpi!=0)
	{
		memcpy(&fpr,found_fpi,sizeof(fpi_s));
		*fp=&fpr;
		last_i=i+1;
	} else {
		*fp=0;
		last_i=0;
	}
	if((action&FREE_REGION_MANAGE_OVERRIDE_FLAG))
	{
		action &= ~FREE_REGION_MANAGE_OVERRIDE_FLAG;
		unsigned char is_free=0;
		if((action&FREE_REGION_MANAGE_IS_FREE_FLAG))
		{
			action&=~FREE_REGION_MANAGE_IS_FREE_FLAG;
			is_free=1;
		} else {
			is_free=0;
		}
		if(found==0&&i>=FREE_REGION_MANAGE_SIZE&&free_pointers_size>=FREE_REGION_MANAGE_SIZE)
		{
			free_pointers_size=FREE_REGION_MANAGE_SIZE;
			if(ex_override==0)
			{
				for(i=0;i<free_pointers_size;i++)
				{
					if(free_pointers[i].is_free==is_free)
					{
						break;
					}
				}
			} else if(ex_override==1)
			{
				for(i=0;i<free_pointers_size;i++)
				{
					if((free_pointers[i].address==0&&free_pointers[i].size==0&&free_pointers[i].is_free==0)||(free_pointers[i].is_free==3||free_pointers[i].is_free==4))
					{
						break;
					}
				}
			}
			i = i % FREE_REGION_MANAGE_SIZE;
			found=action==1||action==2?1:0;
		}
	}
        if(action==1)
        {
                if(i<FREE_REGION_MANAGE_SIZE)
                {
			if(search_iterate==1&&found==0)
			{
				return 0;
			}
			if(search_remove==1)
			{
                        	free_pointers[i].address=address;
                        	free_pointers[i].size=size;
			}
                        free_pointers[i].is_free=1;
			free_pointers[i].is_readed=0;
			free_pointers[FREE_REGION_MANAGE_SIZE].is_readed=0;
                        if(found==0)
                        {
                                i++;
                                free_pointers_size=i;
                        }
			//printf("cool ffff\n");
                        return 1;
		}
                return 0;
        } else if ( action == 2)
        {
                if(found==1)
                {
			if(search_remove==1)
			{
				free_pointers[i].address=address;
				free_pointers[i].size=size;
			}
                        free_pointers[i].is_free=0;
			free_pointers[i].is_readed=0;
			free_pointers[FREE_REGION_MANAGE_SIZE].is_readed=0;
                        return 1;
                }
                return 0;
        } else if ( action == 3)
        {
                return found==1?1:0;
        }
        return 0;
}

LPVOID WINAPI VirtualAlloc(LPVOID lpAddress,SIZE_T dwSize,DWORD flAllocationType,DWORD flProtect)
{
	#ifdef OS_UNIX_STRUCT
		static BOOL HAVE_PAGE_SIZE = FALSE;
		if(HAVE_PAGE_SIZE == FALSE)
		{
			HAVE_PAGE_SIZE = MY_HAS_PAGE_SIZE();
			if(HAVE_PAGE_SIZE==FALSE)
			{
				return NULL;
			}
		}
		void * address = (void*)lpAddress;
		size_t size = (size_t)dwSize;
		int prot=0;
		 switch (flProtect & ~(PAGE_GUARD|PAGE_NOCACHE))
              {
              case PAGE_READONLY:
                prot = PROT_READ;
                break;
              case PAGE_READWRITE:
              case PAGE_WRITECOPY:
                prot = PROT_READ | PROT_WRITE;
                break;
              case PAGE_EXECUTE:
               prot = PROT_EXEC;
                break;
              case PAGE_EXECUTE_READ:
                prot = PROT_READ | PROT_EXEC;
                break;
              case PAGE_EXECUTE_READWRITE:
              case PAGE_EXECUTE_WRITECOPY:
				prot = PROT_READ | PROT_WRITE | PROT_EXEC;
                break;
              case PAGE_NOACCESS:
              default:
                prot = PROT_NONE;
                break;
              }
		if(address!=NULL)
		{
			address=(void*)MY_ROUND_DOWN_PAGE_SIZE((unsigned long long)address);
			size=(size_t)MY_ROUND_UP_PAGE_SIZE((unsigned long long)size);
		}
		
		FREE_REGION_MANAGE_SIZE_TYPE xyz=0;
		FREE_REGION_MANAGE_SIZE_TYPE xy=0;
		fpi_s *fps=0;
		FREE_REGION_MANAGE_EX_GET_POINTERS(&fps,&xy);
		if(fps!=0)
		{
			for(xyz=0;xyz<xy;xyz++)
			{
				printf("va my_address: %016x\nmy_size: %u\nmy_is_free: %u\n",fps[xyz].address,fps[xyz].size,fps[xyz].is_free);
			}
		}
	        unsigned char is_found=1;
                fpi_s * fp;
		FREE_REGION_MANAGE_EX_RESET();
                while(is_found==1)
                {
                        FREE_REGION_MANAGE_EX((unsigned long long)address,(unsigned long long)size,3,(FRM_SEARCH_ADDRESS|FRM_SEARCH_FREE|FRM_SEARCH_ITERATE|FRM_SEARCH_REMOVE),&is_found,&fp);
                        if(fp!=0)
                        {
                                if(fp->size>((unsigned long long)size))
                                {
					size=(size_t)fp->size;
                                }
                        }
                }

		
		if((flAllocationType&(MEM_RESERVE|MEM_COMMIT))==(MEM_RESERVE|MEM_COMMIT))
		{
			flAllocationType=MEM_COMMIT;
		}
		if(flAllocationType&MEM_RESERVE)
		{
			address=__VOID__POINTER__PROTECTOR((mmap(address, size, PROT_NONE, ((address!=NULL)?(MAP_FIXED):(0)) | MAP_PRIVATE|MAP_ANON, -1, 0)));
			if(address==NULL)
				return NULL;
			msync(address, size, MS_SYNC|MS_INVALIDATE);
		}
		if(flAllocationType&MEM_COMMIT)
		{
			address=__VOID__POINTER__PROTECTOR((mmap(address, size, PROT_READ|PROT_WRITE, ((address!=NULL)?(MAP_FIXED):(0))|MAP_SHARED|MAP_ANON, -1, 0)));
			if(address==NULL)
				return NULL;
			msync(address, size, MS_SYNC|MS_INVALIDATE);
		}

		if(address==NULL)
		{
			return NULL;
		}

		int r = mprotect(address,size,prot);
		if(r==-1)
			return NULL;
		FREE_REGION_MANAGE_EX_RESET();
		is_found=1;
                while(is_found==1)
                {
                        FREE_REGION_MANAGE_EX((unsigned long long)address,(unsigned long long)size,2,(FRM_SEARCH_ADDRESS|FRM_SEARCH_FREE|FRM_SEARCH_ITERATE|FRM_SEARCH_REMOVE),&fp,&is_found);
                }
		return address;
		
	#endif
	return NULL;
}
BOOL WINAPI VirtualFree(LPVOID lpAddress,SIZE_T dwSize,DWORD  dwFreeType)
{
	#ifdef OS_UNIX_STRUCT
		static BOOL HAVE_PAGE_SIZE = FALSE;
		if(HAVE_PAGE_SIZE==FALSE)
		{
			HAVE_PAGE_SIZE=MY_HAS_PAGE_SIZE();
			if(HAVE_PAGE_SIZE==FALSE)
				return FALSE;
		}
		void * address=(void*)lpAddress;
		size_t size=(size_t)dwSize;
		if(address!=NULL)
		{
			address=(void*)MY_ROUND_DOWN_PAGE_SIZE((unsigned long long)address);
			size=(size_t)MY_ROUND_UP_PAGE_SIZE((unsigned long long)size);
		}
		unsigned char is_found=1;
		fpi_s * fp;
		FREE_REGION_MANAGE_EX_RESET();
		while(is_found==1)
		{
			FREE_REGION_MANAGE_EX((unsigned long long)address,(unsigned long long)size,3,(FRM_SEARCH_ADDRESS|FRM_SEARCH_RESERVED|FRM_SEARCH_ITERATE|FRM_SEARCH_REMOVE),&is_found,&fp);
			if(fp!=0)
			{
				if(fp->size>((unsigned long long)size))
				{
					size=(size_t)fp->size;
				}
			}
		}

		if((dwFreeType&(MEM_DECOMMIT|MEM_RELEASE))==(MEM_DECOMMIT|MEM_RELEASE))
		{
			dwFreeType=MEM_RELEASE;
		}
		if(dwFreeType&MEM_DECOMMIT)
		{
			mmap(address, size, PROT_NONE, MAP_FIXED|MAP_PRIVATE|MAP_ANON, -1, 0);
			msync(address, size, MS_SYNC|MS_INVALIDATE);
		}
		if(dwFreeType&MEM_RELEASE)
		{
			 msync(address, size, MS_SYNC);
			munmap(address, size);
		}
		FREE_REGION_MANAGE_EX_RESET();
		is_found=1;
		while(is_found==1)
		{
               	 	FREE_REGION_MANAGE_EX((unsigned long long)address,(unsigned long long)size,1,(FRM_SEARCH_ADDRESS|FRM_SEARCH_RESERVED|FRM_SEARCH_ITERATE|FRM_SEARCH_REMOVE),&is_found,&fp);
		}
		FREE_REGION_MANAGE_EX((unsigned long long)address,(unsigned long long)size,1,(FRM_SEARCH_ADDRESS|FRM_SEARCH_SIZE|FRM_SEARCH_REMOVE),&is_found,&fp);
		FREE_REGION_MANAGE_SIZE_TYPE xyz=0;
                FREE_REGION_MANAGE_SIZE_TYPE xy=0;
                fpi_s *fps=0;
                FREE_REGION_MANAGE_EX_GET_POINTERS(&fps,&xy);
                if(fps!=0)
                {
                        for(xyz=0;xyz<xy;xyz++)
                        {
                                printf("vf my_address: %016x\nmy_size: %u\nmy_is_free: %u\n",fps[xyz].address,fps[xyz].size,fps[xyz].is_free);
                        }
                }

		return TRUE;
	#endif
	return FALSE;
}

BOOL WINAPI VirtualProtect(LPVOID lpAddress,SIZE_T dwSize,DWORD  flNewProtect,PDWORD lpflOldProtect) {
	#ifdef OS_UNIX_STRUCT
		static BOOL HAVE_PAGE_SIZE = FALSE;
		if(HAVE_PAGE_SIZE==FALSE)
		{
			HAVE_PAGE_SIZE=MY_HAS_PAGE_SIZE();
			if(HAVE_PAGE_SIZE==FALSE)
				return FALSE;
		}
		if(lpAddress==NULL||dwSize<=0||flNewProtect<=0)
			return FALSE;
		if(lpflOldProtect==NULL)
			return FALSE;
		MEMORY_BASIC_INFORMATION lpBuffer;
		SIZE_T my_ret = VirtualQuery(lpAddress,&lpBuffer,dwSize);
		if(my_ret==0||lpBuffer.State==MEM_FREE)
			return FALSE;
		*lpflOldProtect=lpBuffer.AllocationProtect;
		void * address = (void*)lpAddress;
		size_t size = (size_t)dwSize;
		int prot=0;
		 switch (flNewProtect & ~(PAGE_GUARD|PAGE_NOCACHE))
              {
              case PAGE_READONLY:
                prot = MY_PAGE_READONLY;
                break;
              case PAGE_READWRITE:
              case PAGE_WRITECOPY:
                prot = MY_PAGE_READWRITE;
                break;
              case PAGE_EXECUTE:
               prot = MY_PAGE_EXECUTE;
                break;
              case PAGE_EXECUTE_READ:
                prot = MY_PAGE_EXECUTE_READ;
                break;
              case PAGE_EXECUTE_READWRITE:
              case PAGE_EXECUTE_WRITECOPY:
				prot = MY_PAGE_EXECUTE_READWRITE;
                break;
              case PAGE_NOACCESS:
              default:
                prot = MY_PAGE_NOACCESS;
                break;
              }
		if(address!=NULL)
		{
			address=(void*)MY_ROUND_DOWN_PAGE_SIZE((unsigned long long)address);
			size=(size_t)MY_ROUND_UP_PAGE_SIZE((unsigned long long)size);
		}
		
		int ret = mprotect(address, size, prot);
		if(ret!=-1)
		{
			return TRUE;
		}
		return FALSE;
	#endif
	return FALSE;
}
/*SIZE_T WINAPI VirtualQuery(LPCVOID lpAddress,PMEMORY_BASIC_INFORMATION lpBuffer,SIZE_T dwLength) {
	#ifdef IS_LINUX_OS_DEFINED
		vector_c free_blocks;
		int r = GET_FREE_BLOCKS_EX(lpAddress, 0, (&free_blocks));
		if(r==0)
		{
			return (SIZE_T)r;
		}
		VECTOR_C_CAP_TYPE total = VECTOR_TOTAL_C(free_blocks);
		VECTOR_C_CAP_TYPE i;
		struct_saver * ss;
		my_memory_block * mem;
		for(i=0; i < total; i++)
		{
			ss = VECTOR_GET_C_STRUCT(free_blocks,i);
			mem = (my_memory_block*)ss->item;
			
		}
		my_memory_block blockx;
		r = TO_ONE_BLOCK_EX((&free_blocks),(&blockx));
		if(r==0)
		{
			return (SIZE_T)r;
		}
		
	#endif
	return (SIZE_T)0;
}*/
#endif
SIZE_T WINAPI VirtualQueryUnix(LPCVOID lpAddress,PMEMORY_BASIC_INFORMATION lpBuffer,SIZE_T dwLength)
{
	#ifndef OS_UNIX_STRUCT
		return (SIZE_T)VirtualQuery(lpAddress,lpBuffer,dwLength);
	#else
		if(lpBuffer==NULL||lpBuffer->State!=MEM_FREE||dwLength<=0)
		{
			return (SIZE_T)VirtualQuery(lpAddress,lpBuffer,dwLength);
		}
		SIZE_T xy=VirtualQuery(lpAddress,lpBuffer,dwLength);
		if(xy==0)
		{
			return (SIZE_T)0;
		}
		SIZE_T ia = 40;
		if(lpBuffer->State!=MEM_FREE&&lpBuffer->RegionSize>0&&lpBuffer->RegionSize>(dwLength+(ia*2)))
		{
			if(lpBuffer->AllocationProtect==PAGE_READWRITE||lpBuffer->AllocationProtect==PAGE_EXECUTE_READWRITE)
			{
				SIZE_T ix=0;
				SIZE_T rs=lpBuffer->RegionSize;
				SIZE_T ixp=0;
				SIZE_T rsp=dwLength+(ia*2);
				char * lxx = (char*)lpBuffer->BaseAddress;
				BOOL allok=FALSE;
				for(ix=0; ix < rs; ix++)
				{
					if(ixp>=rsp)
					{
						allok=TRUE;	
						break;
					}
					if(lxx[ix]==0)
					{
						ixp++;
					} else {
						ixp=0;
					}
				}
				//char * nb=(char*)&lxx[(ix-ixp)];
				//SIZE_T nrs=old_basic.RegionSize;
				//memcpy((void*)&nb[64],(void*)lpBuffer,sizeof(MEMORY_BASIC_INFORMATION));
				//memcpy((void*)&old_basic,(void*)lpBuffer,sizeof(MEMORY_BASIC_INFORMATION));
				//old_basic.BaseAddress = (void*)&nb[128];
				//old_basic.RegionSize = nrs;
				//memset((void*)lpBuffer,0,sizeof(MEMORY_BASIC_INFORMATION));
				//memcpy((void*)lpBuffer,(void*)&old_basic,sizeof(MEMORY_BASIC_INFORMATION));
				if(allok==TRUE)
				{
					lpBuffer->BaseAddress=(LPVOID)&lxx[((ix-ixp)+ia)];
				}
				return xy;
			} else {
				return xy;
			}
		} else {
			return xy;
		}
	#endif
	return (SIZE_T)VirtualQuery(lpAddress,lpBuffer,dwLength);
}
BOOL WINAPI VirtualQueryUnixAdjustment(PMEMORY_BASIC_INFORMATION lpBuffer,PMEMORY_BASIC_INFORMATION newBuffer,SIZE_T dwLength)
{
	if(lpBuffer==NULL||newBuffer==NULL)
	{
		return FALSE;
	}
	memcpy((void*)newBuffer,(void*)lpBuffer,sizeof(MEMORY_BASIC_INFORMATION));
#ifdef OS_UNIX_STRUCT
	if(lpBuffer->BaseAddress!=lpBuffer->AllocationBase&&lpBuffer->AllocationProtect==PAGE_READWRITE||lpBuffer->AllocationProtect==PAGE_EXECUTE_READWRITE)
	{
		newBuffer->State=MEM_FREE;
		newBuffer->AllocationBase=newBuffer->BaseAddress;
		newBuffer->RegionSize=dwLength;
		return TRUE;
	} else {
		return FALSE;
	}
#else
	return FALSE;
#endif
}

BOOL WINAPI VirtualQueryUnixGetFreeMemoryRegion(PMEMORY_BASIC_INFORMATION lpBuffer,PMEMORY_BASIC_INFORMATION newBuffer,SIZE_T dwLength)
{

	return VirtualQueryUnixAdjustment(lpBuffer,newBuffer,dwLength);
}

SIZE_T WINAPI VirtualQueryUnixX(LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength,PMEMORY_BASIC_INFORMATION newBuffer,BOOL*sset)
{
	SIZE_T xy = VirtualQueryUnix(lpAddress,lpBuffer,dwLength);
	if(sset!=NULL)
	{
		*sset = VirtualQueryUnixAdjustment(lpBuffer,newBuffer,dwLength);
	}
	return xy;
}

BOOL WINAPI VirtualProtectUnixX(LPVOID lpAddress,SIZE_T dwSize,DWORD  flNewProtect,PDWORD lpflOldProtect,PMEMORY_BASIC_INFORMATION newBuffer,BOOL * sset)
{
	if(lpAddress==NULL||dwSize<=0||newBuffer==NULL||sset==NULL||*sset==FALSE||newBuffer->BaseAddress!=lpAddress||dwSize!=newBuffer->RegionSize)
	{
		return VirtualProtect(lpAddress,dwSize,flNewProtect,lpflOldProtect);
	}
	return TRUE;
}

LPVOID WINAPI VirtualAllocUnixX(LPVOID lpAddress,SIZE_T dwSize,DWORD flAllocationType,DWORD flProtect,PMEMORY_BASIC_INFORMATION newBuffer, BOOL * sset)
{
	if(lpAddress==NULL||dwSize<=0||newBuffer==NULL||sset==NULL||*sset==FALSE||newBuffer->BaseAddress!=lpAddress||dwSize!=newBuffer->RegionSize)
	{
		return VirtualAlloc(lpAddress,dwSize,flAllocationType,flProtect);
	}
	return TRUE;
}
BOOL WINAPI VirtualFreeUnixX(LPVOID lpAddress,SIZE_T dwSize,DWORD dwFreeType,PMEMORY_BASIC_INFORMATION newBuffer,BOOL * sset)
{
	if(lpAddress==NULL||dwSize<=0||newBuffer==NULL||sset==NULL||*sset==FALSE||newBuffer->BaseAddress!=lpAddress||dwSize!=newBuffer->RegionSize)
	{
		return VirtualFree(lpAddress,dwSize,dwFreeType);
	}
	memset((void*)newBuffer->BaseAddress,0,dwSize);
	return TRUE;
}

#ifndef OS_WIN

static void vma_it_func_reset(vma_it_func * zz)
{
	if(zz==0)
		return;
	memset(zz,0,sizeof(vma_it_func));
}

static void vma_it_func_partwise_reset(vma_it_func * zz)
{
	if(zz==0)
		return;
	zz->ret=0;
	//zz->rest=zz->last_rest=0;
}
static int vma_resolve_func(vma_it_func * zz)
{
	if(zz==0)
	{
		return 0;
	}
	fpi_s * fps=0;
        FREE_REGION_MANAGE_SIZE_TYPE free_pointers_size;
        FREE_REGION_MANAGE_EX_GET_POINTERS(&fps,&free_pointers_size);
       	if(fps!=0)
        {
		zz->ret=0;
		if(fps[FREE_REGION_MANAGE_SIZE].is_readed==1)
		{
			return 0;
		}

		FREE_REGION_MANAGE_SIZE_TYPE fi=0;
 		unsigned long long current_address=0xFFFFFFFFFFFFFFFF;
        	unsigned long long current_size=zz->size;
        	FREE_REGION_MANAGE_SIZE_TYPE cfi=0;
        	unsigned char set=0;
	        unsigned char sset=1;

        	for(fi=0;fi<free_pointers_size;fi++)
                {
		    if(fps[fi].is_readed==1)
		    {
			continue;
		    } else {
			sset=0;
		    }
		    	
                    if(fps[fi].is_free==1&&fps[fi].address>0&&fps[fi].address>=zz->start_address&&fps[fi].address<=current_address&&fps[fi].size>=zz->size)
                    {
			current_address=fps[fi].address;
			current_size=fps[fi].size;
			cfi=fi;
			set=1;
                    }
                 }
		 if(sset==1)
			fps[FREE_REGION_MANAGE_SIZE].is_readed=1;
		if(set==0)
		{
			return 0;
		}
		zz->complete_free_region=fps[cfi].is_free;
                zz->start_address=fps[cfi].address;
                zz->base_start_address=fps[cfi].address;
                zz->base_end_address=fps[cfi].address+fps[cfi].size-1;
                zz->base_flags=0;
                zz->size=fps[cfi].size;
                zz->page_alignment_size=fps[cfi].size;
                zz->ret=1;
                fps[cfi].is_readed=1;
		return 1;
        }
	zz->ret=0;
	return 0;
}

static int vma_last_resolve(unsigned long long address,unsigned long long size,vma_it_func*zz)
{
	if(zz==0)
		return 0;
	zz->start_address=(unsigned long long)address;
        zz->size=(unsigned long long)size;
        return vma_resolve_func(zz);
}
                                                      

SIZE_T WINAPI VirtualQuery(LPCVOID lpAddress,PMEMORY_BASIC_INFORMATION lpBuffer,SIZE_T dwLength) 
{
	#ifdef OS_UNIX_STRUCT
                SIZE_T returnValue=0;
		static BOOL HAVE_PAGE_SIZE = FALSE;
#if defined(USE_NEW_SBRK_ADDRESS_ALGO) && defined(OS_UNIX_STRUCT)
		static void * LAST_END_ADDRESS = NULL;
		if(LAST_END_ADDRESS == NULL)
		{
			LAST_END_ADDRESS = GET_VIRTUAL_END_ADDRESS();
		}
#endif
		if(HAVE_PAGE_SIZE == FALSE)
		{
			HAVE_PAGE_SIZE = MY_HAS_PAGE_SIZE();
			if(HAVE_PAGE_SIZE==FALSE) {
				returnValue=0;
				return returnValue;
			}
		}
		if(lpAddress==NULL&&lpBuffer==NULL||dwLength<=0)
		{
			returnValue=0;
			return returnValue;
		}
#if defined(OS_UNIX_STRUCT) && defined(USE_UNIX_IMAGE_EXTENSION_VQ)
		BOOL is_imaged = (lpBuffer->Type==MEM_IMAGE)?(TRUE):(FALSE);
#endif
		unsigned char free_mem=(lpBuffer->State==MEM_FREE&&lpBuffer->AllocationProtect==PAGE_WRITECOPY)?1:0;
		memset((void*)lpBuffer,0,sizeof(MEMORY_BASIC_INFORMATION));
                unsigned long long pagesize=MY_GET_SYSTEM_PAGE_SIZE();
		vma_it_func zz;
		int x=0;
		int y=0;
		int errno_ = 0;
		void * address=NULL;
		void * test_address=NULL;
		unsigned long long nsize_ = MY_ROUND_UP_PAGE_SIZE((unsigned long long)dwLength);
		unsigned  long long psp = 0;
		unsigned  long long rpsp = 0;
		address = (void*)MY_ROUND_DOWN_PAGE_SIZE((unsigned long long)lpAddress);
		if(free_mem==1&&vma_last_resolve((unsigned long long)address,(unsigned long long)nsize_,&zz)==1)
                {
                        goto VMA_RESOLVE_FUNC_LABEL;
                }
                void * testx_address = NULL;
		BOOL reassign_address = FALSE;
#if defined(USE_NEW_SBRK_ADDRESS_ALGO) && defined(OS_UNIX_STRUCT)
		testx_address=__VOID__POINTER__PROTECTOR((__SBRK__WRAPPER(0)));
		if(LAST_END_ADDRESS!=NULL&&testx_address!=NULL)
		{
			if((((unsigned long long)(testx_address))+((unsigned long long)nsize_)<((unsigned long long)(LAST_END_ADDRESS)))
			{
				reassign_address=TRUE;
			}
		}
#endif
		if(reassign_address==TRUE)
		{
			zz.complete_free_region=1;
		} else {
			x = msync(address, (size_t)(nsize_), 0);
			errno_=errno;
			if(x==-1&&errno_==ENOMEM)
			{
				zz.complete_free_region=1;
			} else {
				zz.complete_free_region=0;
			}
		}
		if(zz.complete_free_region==0)
		{
			x = 0;
			zz.start_address=(unsigned long long)lpAddress;
			zz.end_address=(unsigned long long)lpAddress;
			zz.base_start_address=0;
			zz.base_end_address=0;
			zz.ret=0;
#if defined(OS_UNIX_STRUCT) && defined(USE_UNIX_IMAGE_EXTENSION_VQ)
			if(is_imaged==TRUE)
			{
				vma_iterate(((vma_iterate_callback_fn)&vma_iterate_image_func),(void*)&zz);
				if(zz.ret!=2&&zz.ret!=1)
				{
					zz.ret=0;
				} else {
					zz.ret=1;
				}
			}
		
			if(zz.ret==0)
			{
#endif
			vma_iterate(((vma_iterate_callback_fn)&vma_iterate_func),(void*)&zz);
			if(zz.ret!=1)
			{
				zz.ret=0;
				vma_iterate(((vma_iterate_callback_fn)&vma_iterate_full_addressing_func),(void*)&zz);
				if(zz.ret!=0)
				{
						returnValue=0;
						return returnValue;
				}
				x=1;
				
			}
#if defined(OS_UNIX_STRUCT) && defined(USE_UNIX_IMAGE_EXTENSION_VQ)

			}
#endif
			if(x==1)
			{
				zz.start_address=(unsigned long long)address;
				zz.end_address=(unsigned long long)(address+nsize_-1);
				zz.size=(unsigned long long)dwLength;
				zz.page_size=pagesize;
				zz.page_alignment_size=nsize_;
				zz.free_pages=0;
				zz.free_pages_size=0;
				zz.reserved_pages_size=nsize_;
				zz.reserved_pages=(zz.reserved_pages_size/zz.page_size);
			} else {
				/*
			    test_address=(void*)zz.base_start_address;
				for(rpsp = 0; rpsp < nsize_; rpsp+=pagesize)
				{
					x = msync(test_address, (size_t)(rpsp+pagesize), 0);
					errno_=errno;
					if(x==-1&&errno_=ENOMEM)
					{
						break;
					} else {
						continue;
					}
				}
				test_address=(void*)(((unsigned long long)test_address)+rpsp);
				for(psp = 0; psp < nsize_; psp+=pagesize)
				{
					x = msync(test_address, (size_t)(psp+pagesize), 0);
					errno_=errno;
					if(x=-1&&errno_=ENOMEM)
					{
						continue;
					} else {
						break;
					}
				}
			}
			
				int partwise_free=0;
				if(rpsp==0)
				{
					partwise_free=1;
				} else {
					partwise_free=0;
				}*/
				if(zz.base_end_address<=zz.base_start_address)
				{
					returnValue=0;
					return returnValue;
				}
				psp=(zz.base_end_address-zz.base_start_address);
				psp=MY_ROUND_DOWN_PAGE_SIZE((unsigned long long)psp);
				//vma_it_func zz;
				zz.start_address=(unsigned long long)(zz.base_start_address);
				zz.end_address=(unsigned long long)(zz.base_end_address-1);
				zz.size=(unsigned long long)dwLength;
				zz.page_size=pagesize;
				zz.page_alignment_size=psp;
				zz.free_pages=0;
				zz.free_pages_size=0;
				zz.reserved_pages_size=psp;
				zz.reserved_pages=(zz.reserved_pages_size/zz.page_size);
			}
		} else {
			zz.start_address=(unsigned long long)address;
			zz.end_address=(unsigned long long)(address+nsize_-1);
			zz.size=(unsigned long long)dwLength;
			zz.page_alignment_size=nsize_;
VMA_RESOLVE_FUNC_LABEL: ;
			zz.page_size=pagesize;
			zz.free_pages=(zz.page_alignment_size/zz.page_size);
			zz.free_pages_size=zz.page_alignment_size;
			zz.reserved_pages_size=0;
			zz.reserved_pages=0;
		}
		/*int y = vma_iterate((void*)&zz,&vma_iterate_func);
		x = msync(address, (size_t)(nsize_), 0);
		errno_=errno;
		if(x==-1&&errno_=ENOMEM)
		{
			zz.complete_free_region=1;
		} else {
			zz.complete_free_region=0;
		}
		if(zz.complete_free_region==1)
		{
			lpBuffer->BaseAddress=zz.base_start_address;
			lpBuffer->RegionSize=nsize_;
			lpBuffer->State=MEM_FREE;
			
		} else if(free_pages_size>0)
		{
			
		}*/
		int flags=zz.base_flags;
		int cflags=VMA_GET_PERMS(flags);
		unsigned char is_found=0;
		fpi_s * fp;
		if(zz.complete_free_region==1)
		{
			lpBuffer->BaseAddress=(void*)zz.start_address;
			lpBuffer->AllocationBase=(void*)zz.start_address;
			lpBuffer->RegionSize=zz.page_alignment_size;
			lpBuffer->State=MEM_FREE;
			//FREE_REGION_MANAGE_EX((unsigned long long)lpBuffer->BaseAddress,(unsigned long long)lpBuffer->RegionSize,1,(FRM_SEARCH_ADDRESS|FRM_SEARCH_SIZE|FRM_SEARCH_REMOVE),&is_found,&fp);
			//FREE_REGION_MANAGE_EX((unsigned long long)lpBuffer->BaseAddress,(unsigned long long)lpBuffer->RegionSize,3,(FRM_SEARCH_FREE|FRM_SEARCH_ADDRESS|FRM_SEARCH_REMOVE),&is_found,&fp);
			//printf("fffcccc %d\n",is_found);
		} else {
			lpBuffer->BaseAddress=(void*)zz.start_address;
			lpBuffer->AllocationBase=(void*)zz.base_start_address;
			//int is_free=FREE_REGION_MANAGE_EX((unsigned long long)lpBuffer->BaseAddress,(unsigned long long)lpBuffer->RegionSize,3,(FRM_SEARCH_ADDRESS|FRM_SEARCH_SIZE),&is_found,&fp);
			//if(is_found==1&&is_free==1)
			//{
			//	lpBuffer->AllocationBase=(void*)zz.start_address;
			//	lpBuffer->State=MEM_FREE;
			//	return sizeof(MEMORY_BASIC_INFORMATION);
			//}
			if(VMA_PAGE_EXECUTE_READWRITE==cflags)
			{
				lpBuffer->AllocationProtect=PAGE_EXECUTE_READWRITE;
			} else if(VMA_PAGE_EXECUTE_READ==cflags)
			{
				lpBuffer->AllocationProtect=PAGE_EXECUTE_READ;
			} else if(VMA_PAGE_READWRITE==cflags)
			{
				lpBuffer->AllocationProtect=PAGE_READWRITE;
			} else if(VMA_PAGE_READONLY==cflags)
			{
				lpBuffer->AllocationProtect=PAGE_READONLY;
			} else if(cflags==VMA_PAGE_EXECUTE)
			{
				lpBuffer->AllocationProtect=PAGE_EXECUTE;
			} else if(flags > 0) {
				lpBuffer->AllocationProtect=PAGE_READWRITE;
			} else {
				lpBuffer->AllocationProtect=PAGE_NOACCESS;
			}
			lpBuffer->RegionSize=zz.page_alignment_size;
			lpBuffer->State=(flags&VMA_PRIVATE)?MEM_RESERVE:MEM_COMMIT;
			lpBuffer->Type=(flags&VMA_PRIVATE)?MEM_PRIVATE:MEM_MAPPED;
#if defined(OS_UNIX_STRUCT) && defined(USE_UNIX_IMAGE_EXTENSION_VQ)
			if(is_imaged==TRUE)
			{
				int image_flags=0;
				if(lpBuffer->AllocationProtect==PAGE_EXECUTE_READRITE)
					image_flags = MY_PAGE_EXECUTE_READWRITE;
				else if(lpBuffer->AllocationProtect==PAGE_EXECUTE_READ)
					image_flags = MY_PAGE_EXECUTE_READ;
				else if(lpBuffer->AllocationProtect==PAGE_READWRITE)
					image_flags = MY_PAGE_READWRITE;
				else if(lpBuffer->AllocationProtect==PAGE_EXECUTE)
					image_flags = MY_PAGE_EXECUTE;
				else if(lpBuffer->AllocationProtect==PAGE_READONLY)
					image_flags = MY_PAGE_READONLY;
				else if(lpBuffer->AllocationProtect==PAGE_NOACCESS)
					image_flags = MY_PAGE_NOACCESS;
                        	int image_ret = mprotect((void*)lpBuffer->AllocationBase,(size_t)lpBuffer->RegionSize,image_flags);
				if(image_ret==-1)
				{
					returnValue=0;
					return returnValue;
				}
			}
#endif
		}
		returnValue=(SIZE_T)sizeof(MEMORY_BASIC_INFORMATION);
		return returnValue;
	#endif
}
BOOL WINAPI FlushInstructionCache(HANDLE  hProcess, LPCVOID lpBaseAddress, SIZE_T  dwSize)
{
	#ifdef OS_UNIX_STRUCT
		int xyz___=sizeof(int);
		if(xyz___==2)
		{
			xyz___=0x7FFF;
		} else if(xyz___==4)
		{
			xyz___=0x7FFFFFFF;
		} else if(xyz___==8) {
			xyz___=0x7FFFFFFFFFFFFFFF;
		} else {
			return FALSE;
		}
		unsigned long long x=(dwSize/xyz___);
		int y=(dwSize%xyz___);
		BOOL z = FALSE;
		char * pointer=(char*)lpBaseAddress;
		for(;x>0;x--)
		{
			z = (cacheflush(pointer, xyz___,  ICACHE) == 0);
			if((x-1)!=0)
			{
				pointer = (char*)(((unsigned long long)pointer)+((unsigned long long)xyz___));
			}
		}
		if(y>0)
		{
			z = (cacheflush(pointer, y,  ICACHE) == 0);
		}
		return z;
	#endif
	return FALSE;
}
HANDLE WINAPI GetCurrentProcess()
{
	#ifdef OS_UNIX_STRUCT
	#endif
	return NULL;
}
#endif
#ifdef _WINDOWS_HELPER_TO_BOOL_IS_DEFINED
#endif
#ifdef _WINDOWS_HELPER_TO_HEX_IS_DEFINED
char * _WINDOWS_HELPER_TO_HEX_STRING(unsigned long long x)
{
	static char res[17];
        res[16] = '\0';
        res[0] = _WINDOWS_HELPER_TO_HEX(((x & ((unsigned long long)0xF0000000000000000)) >> 60));   
        res[1] = _WINDOWS_HELPER_TO_HEX(((x & ((unsigned long long)0xF000000000000000)) >> 56));   
        res[2] = _WINDOWS_HELPER_TO_HEX(((x & ((unsigned long long)0xF0000000000000)) >> 52));   
        res[3] = _WINDOWS_HELPER_TO_HEX(((x & ((unsigned long long)0xF000000000000)) >> 48));   
        res[4] = _WINDOWS_HELPER_TO_HEX(((x & ((unsigned long long)0xF00000000000)) >> 44));   
        res[5] = _WINDOWS_HELPER_TO_HEX(((x & ((unsigned long long)0xF0000000000)) >> 40));   
        res[6] = _WINDOWS_HELPER_TO_HEX(((x & ((unsigned long long)0xF000000000)) >> 36));   
        res[7] = _WINDOWS_HELPER_TO_HEX(((x & ((unsigned long long)0xF00000000)) >> 32));   
        res[8] = _WINDOWS_HELPER_TO_HEX(((x & ((unsigned long long)0xF0000000)) >> 28));   
        res[9] = _WINDOWS_HELPER_TO_HEX(((x & ((unsigned long long)0xF000000)) >> 24));   
        res[10] = _WINDOWS_HELPER_TO_HEX(((x & ((unsigned long long)0xF00000)) >> 20));   
        res[11] = _WINDOWS_HELPER_TO_HEX(((x & ((unsigned long long)0xF0000)) >> 16));   
    	res[12] = _WINDOWS_HELPER_TO_HEX(((x & ((unsigned long long)0xF000)) >> 12));   
    	res[13] = _WINDOWS_HELPER_TO_HEX(((x & ((unsigned long long)0x0F00)) >> 8));
    	res[14] = _WINDOWS_HELPER_TO_HEX(((x & ((unsigned long long)0x00F0)) >> 4));
        res[15] = _WINDOWS_HELPER_TO_HEX((x & ((unsigned long long)0x000F)));
	char * p = (char*)&res[0];
	while(*p!=0)
	{
		if(*p!='0')
		{
			break;
		}
		p++;
	}
	if(*p==0)
	{
		p--;
	}
        return p;
}
#endif
#endif
