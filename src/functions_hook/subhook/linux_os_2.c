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

//#include <config.h>

/* Specification.  */
#include "linux_os_2.h"

#include <errno.h> /* errno */
#include <stdlib.h> /* size_t */
#include <fcntl.h> /* open, O_RDONLY */
#include <unistd.h> /* getpagesize, read, close */

#if defined __sgi || defined __osf__ /* IRIX, OSF/1 */
# include <string.h> /* memcpy */
# include <sys/types.h>
# include <sys/mman.h> /* mmap, munmap */
# include <sys/procfs.h> /* PIOC*, prmap_t */
#endif

#if defined __APPLE__ && defined __MACH__ /* Mac OS X */
# include <mach/mach.h>
#endif

#if (defined _WIN32 || defined __WIN32__) || defined __CYGWIN__ /* Windows */
# include <windows.h>
#endif

#if defined __BEOS__ || defined __HAIKU__ /* BeOS, Haiku */
# include <OS.h>
#endif

#if defined HAVE_MQUERY /* OpenBSD */
# include <sys/types.h>
# include <sys/mman.h> /* mquery */
#endif


/* Support for reading text files in the /proc file system.  */

#if defined __linux__ || defined __FreeBSD__ || defined __NetBSD__ /* || defined __CYGWIN__ */

/* Buffered read-only streams.
   We cannot use <stdio.h> here, because fopen() calls malloc(), and a malloc()
   call may call mmap() and thus pre-allocate available memory.  */

struct rofile
  {
    int fd;
    size_t position;
    size_t filled;
    int eof_seen;
    char buffer[1024];
  };

/* Open a read-only file stream.  */
static int
rof_open (struct rofile *rof, const char *filename)
{
  int fd = open (filename, O_RDONLY);
  if (fd < 0)
    return -1;
  rof->fd = fd;
  rof->position = 0;
  rof->filled = 0;
  rof->eof_seen = 0;
  return 0;
}

/* Return the next byte from a read-only file stream without consuming it,
   or -1 at EOF.  */
static int
rof_peekchar (struct rofile *rof)
{
  if (rof->position == rof->filled)
    {
      if (rof->eof_seen)
        return -1;
      else
        for (;;)
          {
            int n = read (rof->fd, rof->buffer, sizeof (rof->buffer));
# ifdef EINTR
            if (n < 0 && errno == EINTR)
              continue;
# endif
            if (n <= 0)
              {
                rof->eof_seen = 1;
                return -1;
              }
            rof->filled = n;
            rof->position = 0;
            break;
          }
    }
  return (unsigned char) rof->buffer[rof->position];
}

/* Return the next byte from a read-only file stream, or -1 at EOF.  */
static int
rof_getchar (struct rofile *rof)
{
  int c = rof_peekchar (rof);
  if (c >= 0)
    rof->position++;
  return c;
}

/* Parse an unsigned hexadecimal number from a read-only file stream.  */
static int
rof_scanf_lx (struct rofile *rof, unsigned long long *valuep)
{
  unsigned long long value = 0;
  unsigned int numdigits = 0;
  for (;;)
    {
      int c = rof_peekchar (rof);
      if (c >= '0' && c <= '9')
        value = (value << 4) + (c - '0');
      else if (c >= 'A' && c <= 'F')
        value = (value << 4) + (c - 'A' + 10);
      else if (c >= 'a' && c <= 'f')
        value = (value << 4) + (c - 'a' + 10);
      else
        break;
      rof_getchar (rof);
      numdigits++;
    }
  if (numdigits == 0)
    return -1;
  *valuep = value;
  return 0;
}

/* Close a read-only file stream.  */
static void
rof_close (struct rofile *rof)
{
  close (rof->fd);
}


static char read_n_rof(struct rofile *rof)
{
	char x = 0;
	do
	{
		x = rof_getchar(rof);
	} while(x == '\t' || x == ' ');
	return x;
}

static int rof_contains_string(struct rofile * rof,const char * ss)
{
        unsigned int i = 0;
	printf("dddd\n");
        while(ss[i]!=0)
                i++;
        unsigned int length=i;
        i=0;
        int c=rof_getchar(rof);
	
        while(c!=-1&&c!='\n'&&i<length)
        {
                if(ss[i]==0)
                        continue;
                if(((int)ss[i])!=c)
                {
                        i=0;
                } else {
                        i++;
                }
		printf("%c",c);
		c=rof_getchar(rof);
        }
        return i==length?1:0;
}
#endif

#ifdef VMA_ITERATE_PID_EXT
char * pid_to_string(unsigned int pid,unsigned int * pid_size)
{
	if(pid_size==0)
		return (char*)0;
	static char pid_chars[21];
	unsigned int value=pid;
	unsigned int i = 20;
	pid_chars[i]=0;
	i--;
	do
	{
		pid_chars[i]=(value % 10) + '0';
		i--;
	} while(((value = value / 10) > 0)&&i>=0);
	i++;
	*pid_size=20-i;
	return (char*)&pid_chars[i];
} 

char * get_pid_maps_string(unsigned int pid)
{
	static char pid_str[40];
	static unsigned int pid_size;
	char * pid_string = pid_to_string(my_pid,&pid_size);
	char *fname = (char*)&pid_str[0];
	memset(fname,0,40);
	memcpy(fname,"/proc",6);
	memcpy(&fname[6],pid_string,pid_size);
	memcpy(&fname[6+pid_size],"/maps",5);
	return (char*)fname;
}

void vma_iterate_pid (vma_iterate_callback_fn callback, unsigned long long my_pid_, void *data)
{
#if defined __linux__ /* || defined __CYGWIN__ */
  struct rofile rof;
  int c;
	unsigned int my_pid = (unsigned int)my_pid_;
  /* Open the current process' maps file.  It describes one VMA per line.  */
  if (rof_open (&rof, get_pid_maps_string(my_pid)) < 0)
    return;

  for (;;)
    {
      unsigned long long start, end;
      unsigned int flags;

      /* Parse one line.  First start and end.  */
      if (!(rof_scanf_lx (&rof, &start) >= 0
            && rof_getchar (&rof) == '-'
            && rof_scanf_lx (&rof, &end) >= 0))
        break;
      /* Then the flags.  */
     do
        c = rof_getchar (&rof);
      while (c == ' ');
      flags = 0;
      if (c == 'r')
        flags |= VMA_PROT_READ;
      c = rof_getchar (&rof);
      if (c == 'w')
        flags |= VMA_PROT_WRITE;
      c = rof_getchar (&rof);
      if (c == 'x')
        flags |= VMA_PROT_EXECUTE;
          c = rof_getchar (&rof);
      if (c == 'p')
        flags |= VMA_PROT_PRIVATE;
          else if(c == 's')
       flags |= VMA_PROT_SHARED;

          if (callback (data, (unsigned long long)start, (unsigned long long)end, flags))
        break;

           while (c = rof_getchar (&rof), c != -1 && c != '\n')
        ;
    }
  rof_close (&rof);

#elif defined __FreeBSD__ || defined __NetBSD__
  unsigned int my_pid = (unsigned int)my_pid_;
  char fnamebuf[6+10+1];
  char *fname;
  fname = fnamebuf + sizeof (fnamebuf) - 1;
  *fname = '\0';
  {
    unsigned int value = my_pid;
    do
      *--fname = (value % 10) + '0';
    while ((value = value / 10) > 0);
  }
  fname -= 6;
  memcpy (fname, "/proc/", 6);
  struct rofile rof;
  int c;

  /* Open the current process' maps file.  It   if (rof_open (&rof, "/proc/curproc/map") < 0)
    return;

  for (;;)
    {
      unsigned long long start, end;
      unsigned int flags;
      unsigned int my_pid = (unsigned int)my_pid_;
	  
      /* Parse one line.  First start.  */
      if (!(rof_getchar (&rof) == '0'
            && rof_getchar (&rof) == 'x'
            && rof_scanf_lx (&rof, &start) >= 0))
        break;
      while (c = rof_peekchar (&rof), c == ' ' || c == '\t')
        rof_getchar (&rof);
      /* Then end.  */
describes one VMA per line.  */
   if (!(rof_getchar (&rof) == '0'
            && rof_getchar (&rof) == 'x'
            && rof_scanf_lx (&rof, &end) >= 0))
        break;
      /* Then the flags.  */
      do
        c = rof_getchar (&rof);
      while (c == ' ');
      flags = 0;
      if (c == 'r')
        flags |= VMA_PROT_READ;
      c = rof_getchar (&rof);
      if (c == 'w')
        flags |= VMA_PROT_WRITE;
      c = rof_getchar (&rof);
      if (c == 'x')
      flags |= VMA_PROT_EXECUTE;
          c = rof_getchar (&rof);
      if (c == 'p')
        flags |= VMA_PROT_PRIVATE;
          else if(c == 's')
        flags |= VMA_PROT_SHARED;       

          if (callback (data, (unsigned long long)start, (unsigned long long)end, flags))
        break;

      while (c = rof_getchar (&rof), c != -1 && c != '\n')
          ;
    }
  rof_close (&rof);

#elif defined __sgi || defined __osf__ /* IRIX, OSF/1 */
  size_t pagesize;
  char fnamebuf[6+10+1];
  char *fname;
  int fd;
  int nmaps;
  size_t memneed;
  unsigned int my_pid = (unsigned int)my_pid_;
# if HAVE_MAP_ANONYMOUS
#  define zero_fd -1
#  define map_flags MAP_ANONYMOUS
# else
  int zero_fd;
#  define map_flags 0
# endif
  void *auxmap;
  unsigned long long auxmap_start;
  unsigned long long auxmap_end;
  prmap_t* maps;
  prmap_t* mp;

  pagesize = getpagesize ();

  /* Construct fname = sprintf (fnamebuf+i, "/proc/%u", getpid ()).  */
  fname = fnamebuf + sizeof (fnamebuf) - 1;
  *fname = '\0';
  {
    unsigned int value = my_pid;
    do
      *--fname = (value % 10) + '0';
    while ((value = value / 10) > 0);
  }
  fname -= 6;
 memcpy (fname, "/proc/", 6);

  fd = open (fname, O_RDONLY);
  if (fd < 0)
    return;

  if (ioctl (fd, PIOCNMAP, &nmaps) < 0)
    goto fail2;

  memneed = (nmaps + 10) * sizeof (prmap_t);
  /* Allocate memneed bytes of memory.
     We cannot use alloca here, because not much stack space is guaranteed.
     We also cannot use malloc here, because a malloc() call may call mmap()
     and thus pre-allocate available memory.
     So use mmap(), and ignore the resulting VMA.  */
  memneed = ((memneed - 1) / pagesize + 1) * pagesize;
# if !HAVE_MAP_ANONYMOUS
  zero_fd = open ("/dev/zero", O_RDONLY, 0644);
  if (zero_fd < 0)
    goto fail2;
# endif
  auxmap = (void *) mmap ((void *) 0, memneed, PROT_READ | PROT_WRITE,
                          map_flags | MAP_PRIVATE, zero_fd, 0);
# if !HAVE_MAP_ANONYMOUS
  close (zero_fd);
# endif
  if (auxmap == (void *) -1)
    goto fail2;
  auxmap_start = (unsigned long long) auxmap;
  auxmap_end = auxmap_start + memneed;
  maps = (prmap_t *) auxmap;
  if (ioctl (fd, PIOCMAP, maps) < 0)
    goto fail1;

  for (mp = maps;;)
    {
      unsigned long long start, end;
      unsigned int flags;

      start = (unsigned long long) mp->pr_vaddr;
      end = start + mp->pr_size;
      if (start == 0 && end == 0)
        break;
      flags = 0;
      if (mp->pr_mflags & MA_READ)
        flags |= VMA_PROT_READ;
      if (mp->pr_mflags & MA_WRITE)
      flags |= VMA_PROT_WRITE;
      if (mp->pr_mflags & MA_EXEC)
        flags |= VMA_PROT_EXECUTE;
          if (mp->pr_mflags & MA_SHARED)
                flags |= VMA_PROT_SHARED;
          else
                flags |= VMA_PROT_PRIVATE;
      mp++;
      if (start <= auxmap_start && auxmap_end - 1 <= end - 1)
        {
          /* Consider [start,end-1] \ [auxmap_start,auxmap_end-1]
             = [start,auxmap_start-1] u [auxmap_end,end-1].  */
          if (start < auxmap_start)
            if (callback (data, (unsigned long long)start, (unsigned long long)auxmap_start, flags))
              break;
          if (auxmap_end - 1 < end - 1)
           if (callback (data, (unsigned long long)auxmap_end, (unsigned long long)end, flags))
              break;
        }
      else
        {
          if (callback (data, (unsigned long long)start, (unsigned long long)end, flags))
            break;
        }
    }
  munmap (auxmap, memneed);
  close (fd);
  return;

 fail1:
  munmap (auxmap, memneed);
 fail2:
 close (fd);
  return;

#elif defined __APPLE__ && defined __MACH__ /* Mac OS X */
	task_t task;
	kern_return_t kr;
	if (geteuid() != 0) {
		return;
	}
	
	unsigned int my_pid = (unsigned int)my_pid_;
	kr = task_for_pid(mach_task_self(), my_pid, (task_t*)&task);
	if (kr != KERN_SUCCESS) {
		return;
	}
	
  vm_address_t address;
  vm_size_t size;

  for (address = VM_MIN_ADDRESS;; address += size)
    {
      int more;
      mach_port_t object_name;
      unsigned int flags;
      /* In Mac OS X 10.5, the types vm_address_t, vm_offset_t, vm_size_t have
         32 bits in 32-bit processes and 64 bits in 64-bit processes. Whereas
     mach_vm_address_t and mach_vm_size_t are always 64 bits large.
         Mac OS X 10.5 has three vm_region like methods:
           - vm_region. It has arguments that depend on whether the current
             process is 32-bit or 64-bit. When linking dynamically, this
             function exists only in 32-bit processes. Therefore we use it only
             in 32-bit processes.
           - vm_region_64. It has arguments that depend on whether the current
             process is 32-bit or 64-bit. It interprets a flavor
             VM_REGION_BASIC_INFO as VM_REGION_BASIC_INFO_64, which is
             dangerous since 'struct vm_region_basic_info_64' is larger than
             'struct vm_region_basic_info'; therefore let's write
             VM_REGION_BASIC_INFO_64 explicitly.
           - mach_vm_region. It has arguments that are 64-bit always. This
             function is useful when you want to access the VM of a process
             other than the current process.
         In 64-bit processes, we could use vm_region_64 or mach_vm_region.
        I choose vm_region_64 because it uses the same types as vm_region,
         resulting in less conditional code.  */
# if defined __ppc64__ || defined __x86_64__
      struct vm_region_basic_info_64 info;
      mach_msg_type_number_t info_count = VM_REGION_BASIC_INFO_COUNT_64;

      more = (vm_region_64 (task, &address, &size, VM_REGION_BASIC_INFO_64,
                            (vm_region_info_t)&info, &info_count, &object_name)
              == KERN_SUCCESS);
# else
      struct vm_region_basic_info info;
      mach_msg_type_number_t info_count = VM_REGION_BASIC_INFO_COUNT;

      more = (vm_region (task, &address, &size, VM_REGION_BASIC_INFO,
                         (vm_region_info_t)&info, &info_count, &object_name)
              == KERN_SUCCESS);
# endif
      if (object_name != MACH_PORT_NULL)
        mach_port_deallocate ((ipc_space_t)task, object_name);
      if (!more)
        break;
      flags = 0;
      if (info.protection & VM_PROT_READ)
        flags |= VMA_PROT_READ;
      if (info.protection & VM_PROT_WRITE)
        flags |= VMA_PROT_WRITE;
      if (info.protection & VM_PROT_EXECUTE)
        flags |= VMA_PROT_EXECUTE;
          if (info.shared)
                flags |= VMA_PROT_SHARED;
          else
            flags |= VMA_PROT_PRIVATE;
        if (callback (data, (unsigned long long)address, (unsigned long long)(address+size), flags))
        break;
    }

#elif (defined _WIN32 || defined __WIN32__) || defined __CYGWIN__
  /* Windows platform.  Use the native Windows API.  */

  MEMORY_BASIC_INFORMATION info;
  unsigned long long address = 0;

  while (VirtualQueryEx ((HANDLE)my_pid_,(void*)address, &info, sizeof(info)) == sizeof(info))
    {
      if (info.State != MEM_FREE)
        /* Ignore areas where info.State has the value MEM_RESERVE or,
           equivalently, info.Protect has the undocumented value 0.
           This is needed, so that on Cygwin, areas used by malloc() are
          distinguished from areas reserved for future malloc().  */
        if (info.State != MEM_RESERVE)
          {
            unsigned long long start, end;
            unsigned int flags;

            start = (unsigned long long)info.BaseAddress;
            end = start + info.RegionSize;
            switch (info.Protect & ~(PAGE_GUARD|PAGE_NOCACHE))
              {
              case PAGE_READONLY:
                flags = VMA_PROT_READ;
                break;
              case PAGE_READWRITE:
              case PAGE_WRITECOPY:
                flags = VMA_PROT_READ | VMA_PROT_WRITE;
               break;
              case PAGE_EXECUTE:
                flags = VMA_PROT_EXECUTE;
                break;
              case PAGE_EXECUTE_READ:
                flags = VMA_PROT_READ | VMA_PROT_EXECUTE;
                break;
              case PAGE_EXECUTE_READWRITE:
              case PAGE_EXECUTE_WRITECOPY:
                flags = VMA_PROT_READ | VMA_PROT_WRITE | VMA_PROT_EXECUTE;
                break;
              case PAGE_NOACCESS:
              default:
                flags = 0;
                break;
              }
                     if(info.Type & MEM_PRIVATE)
                            flags |= VMA_PROT_PRIVATE;
                        else
                                flags |= VMA_PROT_SHARED;

            if (callback (data, (unsigned long long)start, (unsigned long long)end, flags))
                                break;
          }
      address = (unsigned long long)info.BaseAddress + info.RegionSize;
    }

#elif defined __BEOS__ || defined __HAIKU__
  /* Use the BeOS specific API.  */

  area_info info;
  int32 cookie;
  cookie = 0;
  area_id ar_id = (area_id)my_pid_;
  while (get_next_area_info (ar_id, &cookie, &info) == B_OK)
    {
      unsigned long long start, end;
      unsigned int flags;

      start = (unsigned long long) info.address;
      end = start + info.size;
      flags = 0;
      if (info.protection & B_READ_AREA)
        flags |= VMA_PROT_READ | VMA_PROT_EXECUTE;
      if (info.protection & B_WRITE_AREA)
        flags |= VMA_PROT_WRITE;

      if (callback (data, (unsigned long long)start, (unsigned long long)end, flags))
       break;
    }

#elif HAVE_MQUERY /* OpenBSD */

  uintptr_t pagesize;
  uintptr_t address;
  int /*bool*/ address_known_mapped;

  pagesize = getpagesize ();
  /* Avoid calling mquery with a NULL first argument, because this argument
     value has a specific meaning.  We know the NULL page is unmapped.  */
  address = pagesize;
  address_known_mapped = 0;
  for (;;)
    {
      /* Test whether the page at address is mapped.  */
      if (address_known_mapped
          || mquery ((void *) address, pagesize, 0, MAP_FIXED, -1, 0)
             == (void *) -1)
        {
          /* The page at address is mapped.
             This is the start of an interval.  */
          uintptr_t start = address;
          uintptr_t end;

          /* Find the end of the interval.  */
          end = (uintptr_t) mquery ((void *) address, pagesize, 0, 0, -1, 0);
          if (end == (uintptr_t) (void *) -1)
            end = 0; /* wrap around */
          address = end;
          /* It's too complicated to find out about the flags.  Just pass 0.  */
         if (callback (data, (unsigned long long)start, (unsigned long long)end, 0))
            break;

          if (address < pagesize) /* wrap around? */
            break;
        }
      /* Here we know that the page at address is unmapped.  */
      {
        uintptr_t query_size = pagesize;

        address += pagesize;

        /* Query larger and larger blocks, to get through the unmapped address
           range with few mquery() calls.  */
        for (;;)
          {
            if (2 * query_size > query_size)
              query_size = 2 * query_size;
            if (address + query_size - 1 < query_size) /* wrap around? */
              {
                address_known_mapped = 0;
                break;
              }
            if (mquery ((void *) address, query_size, 0, MAP_FIXED, -1, 0)
                == (void *) -1)
              {
                /* Not all the interval [address .. address + query_size - 1]
                   is unmapped.  */
                address_known_mapped = (query_size == pagesize);
                break;
              }
            /* The interval [address .. address + query_size - 1] is
              unmapped.  */
            address += query_size;
          }
        /* Reduce the query size again, to determine the precise size of the
           unmapped interval that starts at address.  */
        while (query_size > pagesize)
          {
            query_size = query_size / 2;
            if (address + query_size - 1 >= query_size)
              {
                if (mquery ((void *) address, query_size, 0, MAP_FIXED, -1, 0)
                    != (void *) -1)
                  {
                    /* The interval [address .. address + query_size - 1] is
                       unmapped.  */
                    address += query_size;
                 address_known_mapped = 0;
                  }
                else
                  address_known_mapped = (query_size == pagesize);
              }
          }
        /* Here again query_size = pagesize, and
           either address + pagesize - 1 < pagesize, or
           mquery ((void *) address, pagesize, 0, MAP_FIXED, -1, 0) fails.
           So, the unmapped area ends at address.  */
      }
      if (address + pagesize - 1 < pagesize) /* wrap around? */
        break;
    }

#endif
#endif

void vma_iterate (vma_iterate_callback_fn callback, void *data)
{
#if defined __linux__ /* || defined __CYGWIN__ */

  struct rofile rof;
  int c;

  /* Open the current process' maps file.  It describes one VMA per line.  */
  if (rof_open (&rof, "/proc/self/maps") < 0)
    return;

  for (;;)
    {
      unsigned long long start, end;
      unsigned int flags;

      /* Parse one line.  First start and end.  */
      if (!(rof_scanf_lx (&rof, &start) >= 0
            && rof_getchar (&rof) == '-'
            && rof_scanf_lx (&rof, &end) >= 0))
        break;
      /* Then the flags.  */
      do
        c = rof_getchar (&rof);
      while (c == ' ');
      flags = 0;
      if (c == 'r')
        flags |= VMA_PROT_READ;
      c = rof_getchar (&rof);
      if (c == 'w')
        flags |= VMA_PROT_WRITE;
      c = rof_getchar (&rof);
      if (c == 'x')
        flags |= VMA_PROT_EXECUTE;
	  c = rof_getchar (&rof);
      if (c == 'p')
        flags |= VMA_PROT_PRIVATE;
	  else if(c == 's')
        flags |= VMA_PROT_SHARED;		
	  if (callback (data, (unsigned long long)start, (unsigned long long)end, flags))
        break;

	   while (c = rof_getchar (&rof), c != -1 && c != '\n')
        ;
    }
  rof_close (&rof);

#elif defined __FreeBSD__ || defined __NetBSD__

  struct rofile rof;
  int c;

  /* Open the current process' maps file.  It describes one VMA per line.  */
  if (rof_open (&rof, "/proc/curproc/map") < 0)
    return;

  for (;;)
    {
      unsigned long long start, end;
      unsigned int flags;

      /* Parse one line.  First start.  */
      if (!(rof_getchar (&rof) == '0'
            && rof_getchar (&rof) == 'x'
            && rof_scanf_lx (&rof, &start) >= 0))
        break;
      while (c = rof_peekchar (&rof), c == ' ' || c == '\t')
        rof_getchar (&rof);
      /* Then end.  */
      if (!(rof_getchar (&rof) == '0'
            && rof_getchar (&rof) == 'x'
            && rof_scanf_lx (&rof, &end) >= 0))
        break;
      /* Then the flags.  */
      do
        c = rof_getchar (&rof);
      while (c == ' ');
      flags = 0;
      if (c == 'r')
        flags |= VMA_PROT_READ;
      c = rof_getchar (&rof);
      if (c == 'w')
        flags |= VMA_PROT_WRITE;
      c = rof_getchar (&rof);
      if (c == 'x')
        flags |= VMA_PROT_EXECUTE;
	  c = rof_getchar (&rof);
      if (c == 'p')
        flags |= VMA_PROT_PRIVATE;
	  else if(c == 's')
        flags |= VMA_PROT_SHARED;	
	  if (callback (data, (unsigned long long)start, (unsigned long long)end, flags))
        break;
		
      while (c = rof_getchar (&rof), c != -1 && c != '\n')
	  ;
    }
  rof_close (&rof);

#elif defined __sgi || defined __osf__ /* IRIX, OSF/1 */

  size_t pagesize;
  char fnamebuf[6+10+1];
  char *fname;
  int fd;
  int nmaps;
  size_t memneed;
# if HAVE_MAP_ANONYMOUS
#  define zero_fd -1
#  define map_flags MAP_ANONYMOUS
# else
  int zero_fd;
#  define map_flags 0
# endif
  void *auxmap;
  unsigned long long auxmap_start;
  unsigned long long auxmap_end;
  prmap_t* maps;
  prmap_t* mp;

  pagesize = getpagesize ();

  /* Construct fname = sprintf (fnamebuf+i, "/proc/%u", getpid ()).  */
  fname = fnamebuf + sizeof (fnamebuf) - 1;
  *fname = '\0';
  {
    unsigned int value = getpid ();
    do
      *--fname = (value % 10) + '0';
    while ((value = value / 10) > 0);
  }
  fname -= 6;
  memcpy (fname, "/proc/", 6);

  fd = open (fname, O_RDONLY);
  if (fd < 0)
    return;

  if (ioctl (fd, PIOCNMAP, &nmaps) < 0)
    goto fail2;

  memneed = (nmaps + 10) * sizeof (prmap_t);
  /* Allocate memneed bytes of memory.
     We cannot use alloca here, because not much stack space is guaranteed.
     We also cannot use malloc here, because a malloc() call may call mmap()
     and thus pre-allocate available memory.
     So use mmap(), and ignore the resulting VMA.  */
  memneed = ((memneed - 1) / pagesize + 1) * pagesize;
# if !HAVE_MAP_ANONYMOUS
  zero_fd = open ("/dev/zero", O_RDONLY, 0644);
  if (zero_fd < 0)
    goto fail2;
# endif
  auxmap = (void *) mmap ((void *) 0, memneed, PROT_READ | PROT_WRITE,
                          map_flags | MAP_PRIVATE, zero_fd, 0);
# if !HAVE_MAP_ANONYMOUS
  close (zero_fd);
# endif
  if (auxmap == (void *) -1)
    goto fail2;
  auxmap_start = (unsigned long long) auxmap;
  auxmap_end = auxmap_start + memneed;
  maps = (prmap_t *) auxmap;

  if (ioctl (fd, PIOCMAP, maps) < 0)
    goto fail1;

  for (mp = maps;;)
    {
      unsigned long long start, end;
      unsigned int flags;

      start = (unsigned long long) mp->pr_vaddr;
      end = start + mp->pr_size;
      if (start == 0 && end == 0)
        break;
      flags = 0;
      if (mp->pr_mflags & MA_READ)
        flags |= VMA_PROT_READ;
      if (mp->pr_mflags & MA_WRITE)
        flags |= VMA_PROT_WRITE;
      if (mp->pr_mflags & MA_EXEC)
        flags |= VMA_PROT_EXECUTE;
	  if (mp->pr_mflags & MA_SHARED)
		flags |= VMA_PROT_SHARED;
	  else
		flags |= VMA_PROT_PRIVATE;
      mp++;
      if (start <= auxmap_start && auxmap_end - 1 <= end - 1)
        {
          /* Consider [start,end-1] \ [auxmap_start,auxmap_end-1]
             = [start,auxmap_start-1] u [auxmap_end,end-1].  */
          if (start < auxmap_start)
            if (callback (data, (unsigned long long)start, (unsigned long long)auxmap_start, flags))
              break;
          if (auxmap_end - 1 < end - 1)
            if (callback (data, (unsigned long long)auxmap_end, (unsigned long long)end, flags))
              break;
        }
      else
        {
          if (callback (data, (unsigned long long)start, (unsigned long long)end, flags))
            break;
        }
    }
  munmap (auxmap, memneed);
  close (fd);
  return;

 fail1:
  munmap (auxmap, memneed);
 fail2:
  close (fd);
  return;

#elif defined __APPLE__ && defined __MACH__ /* Mac OS X */

  task_t task = mach_task_self ();
  vm_address_t address;
  vm_size_t size;

  for (address = VM_MIN_ADDRESS;; address += size)
    {
      int more;
      mach_port_t object_name;
      unsigned int flags;
      /* In Mac OS X 10.5, the types vm_address_t, vm_offset_t, vm_size_t have
         32 bits in 32-bit processes and 64 bits in 64-bit processes. Whereas
         mach_vm_address_t and mach_vm_size_t are always 64 bits large.
         Mac OS X 10.5 has three vm_region like methods:
           - vm_region. It has arguments that depend on whether the current
             process is 32-bit or 64-bit. When linking dynamically, this
             function exists only in 32-bit processes. Therefore we use it only
             in 32-bit processes.
           - vm_region_64. It has arguments that depend on whether the current
             process is 32-bit or 64-bit. It interprets a flavor
             VM_REGION_BASIC_INFO as VM_REGION_BASIC_INFO_64, which is
             dangerous since 'struct vm_region_basic_info_64' is larger than
             'struct vm_region_basic_info'; therefore let's write
             VM_REGION_BASIC_INFO_64 explicitly.
           - mach_vm_region. It has arguments that are 64-bit always. This
             function is useful when you want to access the VM of a process
             other than the current process.
         In 64-bit processes, we could use vm_region_64 or mach_vm_region.
         I choose vm_region_64 because it uses the same types as vm_region,
         resulting in less conditional code.  */
# if defined __ppc64__ || defined __x86_64__
      struct vm_region_basic_info_64 info;
      mach_msg_type_number_t info_count = VM_REGION_BASIC_INFO_COUNT_64;

      more = (vm_region_64 (task, &address, &size, VM_REGION_BASIC_INFO_64,
                            (vm_region_info_t)&info, &info_count, &object_name)
              == KERN_SUCCESS);
# else
      struct vm_region_basic_info info;
      mach_msg_type_number_t info_count = VM_REGION_BASIC_INFO_COUNT;

      more = (vm_region (task, &address, &size, VM_REGION_BASIC_INFO,
                         (vm_region_info_t)&info, &info_count, &object_name)
              == KERN_SUCCESS);
# endif
      if (object_name != MACH_PORT_NULL)
        mach_port_deallocate (mach_task_self (), object_name);
      if (!more)
        break;
      flags = 0;
      if (info.protection & VM_PROT_READ)
        flags |= VMA_PROT_READ;
      if (info.protection & VM_PROT_WRITE)
        flags |= VMA_PROT_WRITE;
      if (info.protection & VM_PROT_EXECUTE)
        flags |= VMA_PROT_EXECUTE;
	  if (info.shared)
		flags |= VMA_PROT_SHARED;
	  else
	    flags |= VMA_PROT_PRIVATE;
	  if (callback (data, (unsigned long long)address, (unsigned long long)(address+size), flags))
        break;
    }

#elif (defined _WIN32 || defined __WIN32__) || defined __CYGWIN__
  /* Windows platform.  Use the native Windows API.  */

  MEMORY_BASIC_INFORMATION info;
  unsigned long long address = 0;

  while (VirtualQuery ((void*)address, &info, sizeof(info)) == sizeof(info))
    {
      if (info.State != MEM_FREE)
        /* Ignore areas where info.State has the value MEM_RESERVE or,
           equivalently, info.Protect has the undocumented value 0.
           This is needed, so that on Cygwin, areas used by malloc() are
           distinguished from areas reserved for future malloc().  */
        if (info.State != MEM_RESERVE)
          {
            unsigned long long start, end;
            unsigned int flags;

            start = (unsigned long long)info.BaseAddress;
            end = start + info.RegionSize;
            switch (info.Protect & ~(PAGE_GUARD|PAGE_NOCACHE))
              {
              case PAGE_READONLY:
                flags = VMA_PROT_READ;
                break;
              case PAGE_READWRITE:
              case PAGE_WRITECOPY:
                flags = VMA_PROT_READ | VMA_PROT_WRITE;
                break;
              case PAGE_EXECUTE:
                flags = VMA_PROT_EXECUTE;
                break;
              case PAGE_EXECUTE_READ:
                flags = VMA_PROT_READ | VMA_PROT_EXECUTE;
                break;
              case PAGE_EXECUTE_READWRITE:
              case PAGE_EXECUTE_WRITECOPY:
                flags = VMA_PROT_READ | VMA_PROT_WRITE | VMA_PROT_EXECUTE;
                break;
              case PAGE_NOACCESS:
              default:
                flags = 0;
                break;
              }
			 if(info.Type & MEM_PRIVATE)
			    flags |= VMA_PROT_PRIVATE;
			else
				flags |= VMA_PROT_SHARED;

            if (callback (data, (unsigned long long)start, (unsigned long long)end, flags))
				break;
          }
      address = (unsigned long long)info.BaseAddress + info.RegionSize;
    }

#elif defined __BEOS__ || defined __HAIKU__
  /* Use the BeOS specific API.  */

  area_info info;
  int32 cookie;

  cookie = 0;
  while (get_next_area_info (0, &cookie, &info) == B_OK)
    {
      unsigned long long start, end;
      unsigned int flags;

      start = (unsigned long long) info.address;
      end = start + info.size;
      flags = 0;
      if (info.protection & B_READ_AREA)
        flags |= VMA_PROT_READ | VMA_PROT_EXECUTE;
      if (info.protection & B_WRITE_AREA)
        flags |= VMA_PROT_WRITE;

      if (callback (data, (unsigned long long)start, (unsigned long long)end, flags))
        break;
    }

#elif HAVE_MQUERY /* OpenBSD */

  uintptr_t pagesize;
  uintptr_t address;
  int /*bool*/ address_known_mapped;

  pagesize = getpagesize ();
  /* Avoid calling mquery with a NULL first argument, because this argument
     value has a specific meaning.  We know the NULL page is unmapped.  */
  address = pagesize;
  address_known_mapped = 0;
  for (;;)
    {
      /* Test whether the page at address is mapped.  */
      if (address_known_mapped
          || mquery ((void *) address, pagesize, 0, MAP_FIXED, -1, 0)
             == (void *) -1)
        {
          /* The page at address is mapped.
             This is the start of an interval.  */
          uintptr_t start = address;
          uintptr_t end;

          /* Find the end of the interval.  */
          end = (uintptr_t) mquery ((void *) address, pagesize, 0, 0, -1, 0);
          if (end == (uintptr_t) (void *) -1)
            end = 0; /* wrap around */
          address = end;
          /* It's too complicated to find out about the flags.  Just pass 0.  */
          if (callback (data, (unsigned long long)start, (unsigned long long)end, 0))
            break;

          if (address < pagesize) /* wrap around? */
            break;
        }
      /* Here we know that the page at address is unmapped.  */
      {
        uintptr_t query_size = pagesize;

        address += pagesize;

        /* Query larger and larger blocks, to get through the unmapped address
           range with few mquery() calls.  */
        for (;;)
          {
            if (2 * query_size > query_size)
              query_size = 2 * query_size;
            if (address + query_size - 1 < query_size) /* wrap around? */
              {
                address_known_mapped = 0;
                break;
              }
            if (mquery ((void *) address, query_size, 0, MAP_FIXED, -1, 0)
                == (void *) -1)
              {
                /* Not all the interval [address .. address + query_size - 1]
                   is unmapped.  */
                address_known_mapped = (query_size == pagesize);
                break;
              }
            /* The interval [address .. address + query_size - 1] is
               unmapped.  */
            address += query_size;
          }
        /* Reduce the query size again, to determine the precise size of the
           unmapped interval that starts at address.  */
        while (query_size > pagesize)
          {
            query_size = query_size / 2;
            if (address + query_size - 1 >= query_size)
              {
                if (mquery ((void *) address, query_size, 0, MAP_FIXED, -1, 0)
                    != (void *) -1)
                  {
                    /* The interval [address .. address + query_size - 1] is
                       unmapped.  */
                    address += query_size;
                    address_known_mapped = 0;
                  }
                else
                  address_known_mapped = (query_size == pagesize);
              }
          }
        /* Here again query_size = pagesize, and
           either address + pagesize - 1 < pagesize, or
           mquery ((void *) address, pagesize, 0, MAP_FIXED, -1, 0) fails.
           So, the unmapped area ends at address.  */
      }
      if (address + pagesize - 1 < pagesize) /* wrap around? */
        break;
    }

#endif
}
/*
int get_reserved_blocks(void * start_address,void * end_address, vector_c * reserved_blocks , VECTOR_C_CAP_TYPE BYTES_RESERVED_SIZE)
{
	int z = VECTOR_IS_GOOD_POINTER_C_EX(reserved_blocks);
	if(z==0)
		return 0;
	unsigned long long t1=(unsigned long long)start_address;
	unsigned long long t2=(unsigned long long)end_address;
	unsigned long long ttt=0;
	if(t1!=t2)
	{
		if(t1>t2)
		{
			ttt=t2;
			t2=t1;
			t1=ttt;
			start_address=(void*)t1;
			end_address=(void*)t2;
		}
	} else if(t1!=0) {
		return 0;
	}
VECTOR_INIT_C_EX(reserved_blocks);
VECTOR_RESERVE_CFS_EX(reserved_blocks,(BYTES_RESERVED_SIZE));
my_memory_block bloc;
#if defined __linux__ /* || defined __CYGWIN__ */
/*
  struct rofile rof;
  int c;

  /* Open the current process' maps file.  It describes one VMA per line.  */
  /*
  if (rof_open (&rof, "/proc/self/maps") < 0)
    return;

  for (;;)
    {
      unsigned long long start, end;
      unsigned int flags;

      /* Parse one line.  First start and end.  */
     /*
      if (!(rof_scanf_lx (&rof, &start) >= 0
            && rof_getchar (&rof) == '-'
            && rof_scanf_lx (&rof, &end) >= 0))
        break;
      /* Then the flags.  */
      /*
      do
        c = rof_getchar (&rof);
      while (c == ' ');
      flags = 0;
      if (c == 'r')
        flags |= VMA_PROT_READ;
      c = rof_getchar (&rof);
      if (c == 'w')
        flags |= VMA_PROT_WRITE;
      c = rof_getchar (&rof);
      if (c == 'x')
        flags |= VMA_PROT_EXECUTE;
	  int break_state = 0;
	  c=read_n_rof(&rof);
	  if (c == 'p')
	  {
		flags |= VMA_PROT_PRIVATE;
	  } else if (c == 's')
	  {
		flags |= VMA_PROT_SHARED;
	  } else if (c == -1 || c == '\n')
	  {
		break_state=1;
	  } else {
		break_state=0;
	  }
	  #define MY_MAX_LENGTH 1024
	  char dd[MY_MAX_LENGTH];
	  char de[MY_MAX_LENGTH+1];
	  unsigned long long xi=0;
	  memset(dd,0,MY_MAX_LENGTH);
	  if(break_state==0)
	  {
		c=read_n_rof(&rof);
		if(c=='\n'||c==-1)
		{
			break_state=1;
		} else {
			while (c!='\t'||c!=' '&&c!='\n'&&c!=-1)
			{
				if((xi+1)>=MY_MAX_LENGTH)
				{
					xi = MY_MAX_LENGTH-1;
				}
				dd[xi++]=c;
				c=rof_getchar(&rof);
			}
			if((xi+1)>=MY_MAX_LENGTH)
			{
				xi = MY_MAX_LENGTH-1;
			}
			dd[xi++]=0;
			if(c=='\n'||c==-1)
			{
				break_state=1;
			} else {
				c=read_n_rof(&rof);
				while (c != '\t'&&c!=' '&&c!='\n'&&c!=-1)
				{
					if((xi+1)>=MY_MAX_LENGTH)
					{
						xi = MY_MAX_LENGTH-1;
					}
					dd[xi++]=c;
					c = rof_getchar (&rof);
				}
				if((xi+1)>=MY_MAX_LENGTH)
				{
					xi = MY_MAX_LENGTH-1;
				}
				dd[xi++]=0;
				 //7fff881ff000-7fff88200000 r-xp 00000000 00:00 0                          [vdso]
				if(c=='\n'||c==-1)
				{
					break_state=1;
				} else {
					c=read_n_rof(&rof);
					while (c != '\t'&&c!=' '&&c!='\n'&&c!=-1)
					{
						if((xi+1)>=MY_MAX_LENGTH)
						{
							xi = MY_MAX_LENGTH-1;
						}
						dd[xi++]=c;
						c = rof_getchar (&rof);
					}
					if((xi+1)>=MY_MAX_LENGTH)
					{
						xi = MY_MAX_LENGTH-1;
					}		
					dd[xi++]=0;
					if(c=='\n'||c==-1)
					{
						break_state=1;
					} else {
						c=read_n_rof(&rof);
						while (c != '\t'&&c!=' '&&c!='\n'&&c!=-1)
						{
							if((xi+1)>=MY_MAX_LENGTH)
							{
								xi = MY_MAX_LENGTH-1;
							}
							dd[xi++]=c;
							c = rof_getchar (&rof);
						}
						if((xi+1)>=MY_MAX_LENGTH)
						{
							xi = MY_MAX_LENGTH-1;
						}
						dd[xi++]=0;
						if(c=='\n'||c==-1)
						{
							break_state=1;
						} else {
							c=read_n_rof(&rof);
							while (c != '\t'&&c!=' '&&c!='\n'&&c!=-1)
							{
								if((xi+1)>=MY_MAX_LENGTH)
								{
									xi = MY_MAX_LENGTH-1;
								}		
								dd[xi++]=c;
								c = rof_getchar (&rof);
							}
							if((xi+1)>=MY_MAX_LENGTH)
							{
								xi = MY_MAX_LENGTH-1;
							}							
							dd[xi++]=0;
							if(c=='\n'||c==-1)
							{
								break_state=1;
							} else {
								c=read_n_rof(&rof);
								while (c != '\t'&&c!=' '&&c!='\n'&&c!=-1)
								{
									if((xi+1)>=MY_MAX_LENGTH)
									{
										xi = MY_MAX_LENGTH-1;
									}
									dd[xi++]=c;
									c = rof_getchar (&rof);
								}
								if((xi+1)>=MY_MAX_LENGTH)
								{
									xi = MY_MAX_LENGTH-1;
								}
								dd[xi++]=0;
								if(c=='\n'||c==-1)
								{
									break_state=1;
								} else {
									c=read_n_rof(&rof);
									while (c != '\t'&&c!=' '&&c!='\n'&&c!=-1)
									{
										if((xi+1)>=MY_MAX_LENGTH)
										{
											xi = MY_MAX_LENGTH-1;
										}
										dd[xi++]=c;
										c = rof_getchar (&rof);
									}
									if((xi+1)>=MY_MAX_LENGTH)
									{
										xi = MY_MAX_LENGTH-1;
									}
									dd[xi++]=0;
									break_state=1;
								}
							}
						}
					}
				}
			}
		}
	  }
	  memcpy(de,dd,1024);
	  de[1024]=0;

      bloc.start = (void*)start; 
	  bloc.end = (void*)end;
	  bloc.flags = flags;
	  bloc.reserved = 1;
	  if((start_address==NULL&&end_address==NULL)||((((unsigned long long)bloc.start) >= ((unsigned long long)start_address)) && ((end_address==NULL) || (((unsigned long long)bloc.end) <= ((unsigned long long)end_address))))) {	   
	  {
		VECTOR_ADD_C_STRUCT((*reserved_blocks),bloc,temp);
	  }
    }
  rof_close (&rof);

#elif defined __FreeBSD__ || defined __NetBSD__

  struct rofile rof;
  int c;

  /* Open the current process' maps file.  It describes one VMA per line.  */
  /*
  if (rof_open (&rof, "/proc/curproc/map") < 0)
    return;

  for (;;)
    {
      unsigned long long start, end;
      unsigned int flags;

      /* Parse one line.  First start.  */
      /*
      if (!(rof_getchar (&rof) == '0'
            && rof_getchar (&rof) == 'x'
            && rof_scanf_lx (&rof, &start) >= 0))
        break;
      while (c = rof_peekchar (&rof), c == ' ' || c == '\t')
        rof_getchar (&rof);
      /* Then end.  */
      /*
      if (!(rof_getchar (&rof) == '0'
            && rof_getchar (&rof) == 'x'
            && rof_scanf_lx (&rof, &end) >= 0))
        break;
      /* Then the flags.  */
      /*
      do
        c = rof_getchar (&rof);
      while (c == ' ');
      flags = 0;
      if (c == 'r')
        flags |= VMA_PROT_READ;
      c = rof_getchar (&rof);
      if (c == 'w')
        flags |= VMA_PROT_WRITE;
      c = rof_getchar (&rof);
      if (c == 'x')
        flags |= VMA_PROT_EXECUTE;
	  int break_state = 0;
	  c=read_n_rof(&rof);
	  if (c == 'p')
	  {
		flags |= VMA_PROT_PRIVATE;
	  } else if (c == 's')
	  {
		flags |= VMA_PROT_SHARED;
	  } else if (c == -1 || c == '\n')
	  {
		break_state=1;
	  } else {
		break_state=0;
	  }
	  #define MY_MAX_LENGTH 1024
	  char dd[MY_MAX_LENGTH];
	  char de[MY_MAX_LENGTH+1];
	  unsigned long long xi=0;
	  memset(dd,0,MY_MAX_LENGTH);
	  if(break_state==0)
	  {
		c=read_n_rof(&rof);
		if(c=='\n'||c==-1)
		{
			break_state=1;
		} else {
			while (c!='\t'||c!=' '&&c!='\n'&&c!=-1)
			{
				if((xi+1)>=MY_MAX_LENGTH)
				{
					xi = MY_MAX_LENGTH-1;
				}
				dd[xi++]=c;
				c=rof_getchar(&rof);
			}
			if((xi+1)>=MY_MAX_LENGTH)
			{
				xi = MY_MAX_LENGTH-1;
			}
			dd[xi++]=0;
			if(c=='\n'||c==-1)
			{
				break_state=1;
			} else {
				c=read_n_rof(&rof);
				while (c != '\t'&&c!=' '&&c!='\n'&&c!=-1)
				{
					if((xi+1)>=MY_MAX_LENGTH)
					{
						xi = MY_MAX_LENGTH-1;
					}
					dd[xi++]=c;
					c = rof_getchar (&rof);
				}
				if((xi+1)>=MY_MAX_LENGTH)
				{
					xi = MY_MAX_LENGTH-1;
				}
				dd[xi++]=0;
				 //7fff881ff000-7fff88200000 r-xp 00000000 00:00 0                          [vdso]
				if(c=='\n'||c==-1)
				{
					break_state=1;
				} else {
					c=read_n_rof(&rof);
					while (c != '\t'&&c!=' '&&c!='\n'&&c!=-1)
					{
						if((xi+1)>=MY_MAX_LENGTH)
						{
							xi = MY_MAX_LENGTH-1;
						}
						dd[xi++]=c;
						c = rof_getchar (&rof);
					}
					if((xi+1)>=MY_MAX_LENGTH)
					{
						xi = MY_MAX_LENGTH-1;
					}		
					dd[xi++]=0;
					if(c=='\n'||c==-1)
					{
						break_state=1;
					} else {
						c=read_n_rof(&rof);
						while (c != '\t'&&c!=' '&&c!='\n'&&c!=-1)
						{
							if((xi+1)>=MY_MAX_LENGTH)
							{
								xi = MY_MAX_LENGTH-1;
							}
							dd[xi++]=c;
							c = rof_getchar (&rof);
						}
						if((xi+1)>=MY_MAX_LENGTH)
						{
							xi = MY_MAX_LENGTH-1;
						}
						dd[xi++]=0;
						if(c=='\n'||c==-1)
						{
							break_state=1;
						} else {
							c=read_n_rof(&rof);
							while (c != '\t'&&c!=' '&&c!='\n'&&c!=-1)
							{
								if((xi+1)>=MY_MAX_LENGTH)
								{
									xi = MY_MAX_LENGTH-1;
								}		
								dd[xi++]=c;
								c = rof_getchar (&rof);
							}
							if((xi+1)>=MY_MAX_LENGTH)
							{
								xi = MY_MAX_LENGTH-1;
							}							
							dd[xi++]=0;
							if(c=='\n'||c==-1)
							{
								break_state=1;
							} else {
								c=read_n_rof(&rof);
								while (c != '\t'&&c!=' '&&c!='\n'&&c!=-1)
								{
									if((xi+1)>=MY_MAX_LENGTH)
									{
										xi = MY_MAX_LENGTH-1;
									}
									dd[xi++]=c;
									c = rof_getchar (&rof);
								}
								if((xi+1)>=MY_MAX_LENGTH)
								{
									xi = MY_MAX_LENGTH-1;
								}
								dd[xi++]=0;
								if(c=='\n'||c==-1)
								{
									break_state=1;
								} else {
									c=read_n_rof(&rof);
									while (c != '\t'&&c!=' '&&c!='\n'&&c!=-1)
									{
										if((xi+1)>=MY_MAX_LENGTH)
										{
											xi = MY_MAX_LENGTH-1;
										}
										dd[xi++]=c;
										c = rof_getchar (&rof);
									}
									if((xi+1)>=MY_MAX_LENGTH)
									{
										xi = MY_MAX_LENGTH-1;
									}
									dd[xi++]=0;
									break_state=1;
								}
							}
						}
					}
				}
			}
		}
	  }
	  memcpy(de,dd,1024);
	  de[1024]=0;

      bloc.start = (void*)start; 
	  bloc.end = (void*)end;
	  bloc.flags = flags;
	  bloc.reserved = 1;
	  if((start_address==NULL&&end_address==NULL)||((((unsigned long long)bloc.start) >= ((unsigned long long)start_address)) && ((end_address==NULL) || (((unsigned long long)bloc.end) <= ((unsigned long long)end_address))))) {	   
	  {
		VECTOR_ADD_C_STRUCT((*reserved_blocks),bloc,temp);
	  }
    }
  rof_close (&rof);

#elif defined __sgi || defined __osf__ /* IRIX, OSF/1 */
/*
  size_t pagesize;
  char fnamebuf[6+10+1];
  char *fname;
  int fd;
  int nmaps;
  size_t memneed;
# if HAVE_MAP_ANONYMOUS
#  define zero_fd -1
#  define map_flags MAP_ANONYMOUS
# else
  int zero_fd;
#  define map_flags 0
# endif
  void *auxmap;
  unsigned long long auxmap_start;
  unsigned long long auxmap_end;
  prmap_t* maps;
  prmap_t* mp;

  pagesize = getpagesize ();

  /* Construct fname = sprintf (fnamebuf+i, "/proc/%u", getpid ()).  */
/*
  fname = fnamebuf + sizeof (fnamebuf) - 1;
  *fname = '\0';
  {
    unsigned int value = getpid ();
    do
      *--fname = (value % 10) + '0';
    while ((value = value / 10) > 0);
  }
  fname -= 6;
  memcpy (fname, "/proc/", 6);

  fd = open (fname, O_RDONLY);
  if (fd < 0)
    return;

  if (ioctl (fd, PIOCNMAP, &nmaps) < 0)
    goto fail2;

  memneed = (nmaps + 10) * sizeof (prmap_t);
  /* Allocate memneed bytes of memory.
     We cannot use alloca here, because not much stack space is guaranteed.
     We also cannot use malloc here, because a malloc() call may call mmap()
     and thus pre-allocate available memory.
     So use mmap(), and ignore the resulting VMA.  */
/*
  memneed = ((memneed - 1) / pagesize + 1) * pagesize;
# if !HAVE_MAP_ANONYMOUS
  zero_fd = open ("/dev/zero", O_RDONLY, 0644);
  if (zero_fd < 0)
    goto fail2;
# endif
  auxmap = (void *) mmap ((void *) 0, memneed, PROT_READ | PROT_WRITE,
                          map_flags | MAP_PRIVATE, zero_fd, 0);
# if !HAVE_MAP_ANONYMOUS
  close (zero_fd);
# endif
  if (auxmap == (void *) -1)
    goto fail2;
  auxmap_start = (unsigned long long) auxmap;
  auxmap_end = (unsigned long long)(auxmap_start + memneed);
  maps = (prmap_t *) auxmap;

  if (ioctl (fd, PIOCMAP, maps) < 0)
    goto fail1;

  for (mp = maps;;)
    {
      unsigned long long start, end;
      unsigned int flags;

      start = (unsigned long long) mp->pr_vaddr;
      end = (unsigned long long)(start + mp->pr_size);
      if (start == 0 && end == 0)
        break;
      flags = 0;
      if (mp->pr_mflags & MA_READ)
        flags |= VMA_PROT_READ;
      if (mp->pr_mflags & MA_WRITE)
        flags |= VMA_PROT_WRITE;
      if (mp->pr_mflags & MA_EXEC)
        flags |= VMA_PROT_EXECUTE;
      mp++;
      if (start <= auxmap_start && auxmap_end - 1 <= end - 1)
        {
          /* Consider [start,end-1] \ [auxmap_start,auxmap_end-1]
             = [start,auxmap_start-1] u [auxmap_end,end-1].  */
/*
          if (start < auxmap_start)
          {
			bloc.start = (void*)start; 
		    bloc.end = (void*)auxmap_start;
			bloc.flags = flags;
			bloc.reserved = 1;
			if((start_address==NULL&&end_address==NULL)||((((unsigned long long)bloc.start) >= ((unsigned long long)start_address)) && ((end_address==NULL) || (((unsigned long long)bloc.end) <= ((unsigned long long)end_address))))) {	   
			{
				VECTOR_ADD_C_STRUCT((*reserved_blocks),bloc,temp);
			}
		  }
          if (auxmap_end - 1 < end - 1)
		  {
		    bloc.start = (void*)auxmap_end; 
			bloc.end = (void*)end;
			bloc.flags = flags;
			bloc.reserved = 1;
			if((start_address==NULL&&end_address==NULL)||((((unsigned long long)bloc.start) >= ((unsigned long long)start_address)) && ((end_address==NULL) || (((unsigned long long)bloc.end) <= ((unsigned long long)end_address))))) {	   
			{
				VECTOR_ADD_C_STRUCT((*reserved_blocks),bloc,temp);
			}
	      }
        }
      else
        {
			bloc.start = (void*)start; 
			bloc.end = (void*)end;
			bloc.flags = flags;
			bloc.reserved = 1;
			if((start_address==NULL&&end_address==NULL)||((((unsigned long long)bloc.start) >= ((unsigned long long)start_address)) && ((end_address==NULL) || (((unsigned long long)bloc.end) <= ((unsigned long long)end_address))))) {	   
			{
				VECTOR_ADD_C_STRUCT((*reserved_blocks),bloc,temp);
			}
        }
    }
  munmap (auxmap, memneed);
  close (fd);
  return;

 fail1:
  munmap (auxmap, memneed);
 fail2:
  close (fd);
  return;

#elif defined __APPLE__ && defined __MACH__ /* Mac OS X */
/*
  task_t task = mach_task_self ();
  vm_address_t address;
  vm_size_t size;

  for (address = VM_MIN_ADDRESS;; address += size)
    {
      int more;
      mach_port_t object_name;
      unsigned int flags;
      /* In Mac OS X 10.5, the types vm_address_t, vm_offset_t, vm_size_t have
         32 bits in 32-bit processes and 64 bits in 64-bit processes. Whereas
         mach_vm_address_t and mach_vm_size_t are always 64 bits large.
         Mac OS X 10.5 has three vm_region like methods:
           - vm_region. It has arguments that depend on whether the current
             process is 32-bit or 64-bit. When linking dynamically, this
             function exists only in 32-bit processes. Therefore we use it only
             in 32-bit processes.
           - vm_region_64. It has arguments that depend on whether the current
             process is 32-bit or 64-bit. It interprets a flavor
             VM_REGION_BASIC_INFO as VM_REGION_BASIC_INFO_64, which is
             dangerous since 'struct vm_region_basic_info_64' is larger than
             'struct vm_region_basic_info'; therefore let's write
             VM_REGION_BASIC_INFO_64 explicitly.
           - mach_vm_region. It has arguments that are 64-bit always. This
             function is useful when you want to access the VM of a process
             other than the current process.
         In 64-bit processes, we could use vm_region_64 or mach_vm_region.
         I choose vm_region_64 because it uses the same types as vm_region,
         resulting in less conditional code.  */
/*
# if defined __ppc64__ || defined __x86_64__
      struct vm_region_basic_info_64 info;
      mach_msg_type_number_t info_count = VM_REGION_BASIC_INFO_COUNT_64;

      more = (vm_region_64 (task, &address, &size, VM_REGION_BASIC_INFO_64,
                            (vm_region_info_t)&info, &info_count, &object_name)
              == KERN_SUCCESS);
# else
      struct vm_region_basic_info info;
      mach_msg_type_number_t info_count = VM_REGION_BASIC_INFO_COUNT;

      more = (vm_region (task, &address, &size, VM_REGION_BASIC_INFO,
                         (vm_region_info_t)&info, &info_count, &object_name)
              == KERN_SUCCESS);
# endif
      if (object_name != MACH_PORT_NULL)
        mach_port_deallocate (mach_task_self (), object_name);
      if (!more)
        break;
      flags = 0;
      if (info.protection & VM_PROT_READ)
        flags |= VMA_PROT_READ;
      if (info.protection & VM_PROT_WRITE)
        flags |= VMA_PROT_WRITE;
      if (info.protection & VM_PROT_EXECUTE)
        flags |= VMA_PROT_EXECUTE;
	    bloc.start = (void*)address; 
		bloc.end = (void*)(address+size);
		bloc.flags = flags;
		bloc.reserved = 1;
	    if((start_address==NULL&&end_address==NULL)||((((unsigned long long)bloc.start) >= ((unsigned long long)start_address)) && ((end_address==NULL) || (((unsigned long long)bloc.end) <= ((unsigned long long)end_address))))) {	   
	    {
			VECTOR_ADD_C_STRUCT((*reserved_blocks),bloc,temp);
	    }
    }

#elif (defined _WIN32 || defined __WIN32__) || defined __CYGWIN__
  /* Windows platform.  Use the native Windows API.  */
/*
  MEMORY_BASIC_INFORMATION info;
  unsigned long long address = 0;

  while (VirtualQuery ((void*)address, &info, sizeof(info)) == sizeof(info))
    {
      if (info.State != MEM_FREE)
        /* Ignore areas where info.State has the value MEM_RESERVE or,
           equivalently, info.Protect has the undocumented value 0.
           This is needed, so that on Cygwin, areas used by malloc() are
           distinguished from areas reserved for future malloc().  */
/*
		if (info.State == MEM_RESERVE)
          {
			unsigned long long start, end;
            unsigned int flags;

            start = (unsigned long long)info.BaseAddress;
            end = (unsigned long long)(start + info.RegionSize);
			bloc.start = (void*)start; 
			bloc.end = (void*)end;
			bloc.flags = -1;
			bloc.reserved = 1;
			if((start_address==NULL&&end_address==NULL)||((((unsigned long long)bloc.start) >= ((unsigned long long)start_address)) && ((end_address==NULL) || (((unsigned long long)bloc.end) <= ((unsigned long long)end_address))))) {	   
			{
				VECTOR_ADD_C_STRUCT((*reserved_blocks),bloc,temp);
			}
		  } else {
            unsigned long long start, end;
            unsigned int flags;

            start = (unsigned long long)info.BaseAddress;
            end = (unsigned long long)(start + info.RegionSize -1);
            switch (info.Protect & ~(PAGE_GUARD|PAGE_NOCACHE))
              {
              case PAGE_READONLY:
                flags = VMA_PROT_READ;
                break;
              case PAGE_READWRITE:
              case PAGE_WRITECOPY:
                flags = VMA_PROT_READ | VMA_PROT_WRITE;
                break;
              case PAGE_EXECUTE:
                flags = VMA_PROT_EXECUTE;
                break;
              case PAGE_EXECUTE_READ:
                flags = VMA_PROT_READ | VMA_PROT_EXECUTE;
                break;
              case PAGE_EXECUTE_READWRITE:
              case PAGE_EXECUTE_WRITECOPY:
                flags = VMA_PROT_READ | VMA_PROT_WRITE | VMA_PROT_EXECUTE;
                break;
              case PAGE_NOACCESS:
              default:
                flags = 0;
                break;
              }

			  bloc.start = (void*)start; 
			  bloc.end = (void*)end;
			  bloc.flags = flags;
			  bloc.reserved = 1;
		      if((start_address==NULL&&end_address==NULL)||((((unsigned long long)bloc.start) >= ((unsigned long long)start_address)) && ((end_address==NULL) || (((unsigned long long)bloc.end) <= ((unsigned long long)end_address))))) {	   
	          {
				VECTOR_ADD_C_STRUCT((*reserved_blocks),bloc,temp);
			  }
          }
      address = (unsigned long long)(info.BaseAddress + info.RegionSize);
    }

#elif defined __BEOS__ || defined __HAIKU__
  /* Use the BeOS specific API.  */
/*
  area_info info;
  int32 cookie;

  cookie = 0;
  while (get_next_area_info (0, &cookie, &info) == B_OK)
    {
      unsigned long long start, end;
      unsigned int flags;

      start = (unsigned long long) info.address;
      end = start + info.size;
      flags = 0;
      if (info.protection & B_READ_AREA)
        flags |= VMA_PROT_READ | VMA_PROT_EXECUTE;
      if (info.protection & B_WRITE_AREA)
        flags |= VMA_PROT_WRITE;

	   bloc.start = (void*)start; 
	   bloc.end = (void*)end;
	   bloc.flags = -1;
	   bloc.reserved = 1;
	   if((start_address==NULL&&end_address==NULL)||((((unsigned long long)bloc.start) >= ((unsigned long long)start_address)) && ((end_address==NULL) || (((unsigned long long)bloc.end) <= ((unsigned long long)end_address))))) {	   
	   {
			VECTOR_ADD_C_STRUCT((*reserved_blocks),bloc,temp);
	   }
    }

#elif HAVE_MQUERY /* OpenBSD */
/*
  uintptr_t pagesize;
  uintptr_t address;
  int /*bool*//* address_known_mapped;

  pagesize = getpagesize ();
  /* Avoid calling mquery with a NULL first argument, because this argument
     value has a specific meaning.  We know the NULL page is unmapped.  */
/*
  address = pagesize;
  address_known_mapped = 0;
  for (;;)
    {
      /* Test whether the page at address is mapped.  */
/* 
     if (address_known_mapped
          || mquery ((void *) address, pagesize, 0, MAP_FIXED, -1, 0)
             == (void *) -1)
        {
          /* The page at address is mapped.
             This is the start of an interval.  */
/*
          uintptr_t start = address;
          uintptr_t end;

          /* Find the end of the interval.  */
/* 
         end = (uintptr_t) mquery ((void *) address, pagesize, 0, 0, -1, 0);
          if (end == (uintptr_t) (void *) -1)
            end = 0; /* wrap around */
/*  
        address = end;

          /* It's too complicated to find out about the flags.  Just pass 0.  */
/* 
           bloc.start = (void*)start; 
			bloc.end = (void*)end;
			bloc.flags = 0;
			bloc.reserved = 1;
			if((start_address==NULL&&end_address==NULL)||((((unsigned long long)bloc.start) >= ((unsigned long long)start_address)) && ((end_address==NULL) || (((unsigned long long)bloc.end) <= ((unsigned long long)end_address)))))
			{	 
				VECTOR_ADD_C_STRUCT((*reserved_blocks),bloc,temp);
			}
          if (address < pagesize) /* wrap around? */
/*
            break;
        }
      /* Here we know that the page at address is unmapped.  */
/*
      {
        uintptr_t query_size = pagesize;

        address += pagesize;

        /* Query larger and larger blocks, to get through the unmapped address
           range with few mquery() calls.  */
/* 
       for (;;)
          {
            if (2 * query_size > query_size)
              query_size = 2 * query_size;
            if (address + query_size - 1 < query_size) /* wrap around? */
/* 
             {
                address_known_mapped = 0;
                break;
              }
            if (mquery ((void *) address, query_size, 0, MAP_FIXED, -1, 0)
                == (void *) -1)
              {
                /* Not all the interval [address .. address + query_size - 1]
                   is unmapped.  */
/*
                address_known_mapped = (query_size == pagesize);
                break;
              }
            /* The interval [address .. address + query_size - 1] is
               unmapped.  */
/* 
           address += query_size;
          }
        /* Reduce the query size again, to determine the precise size of the
           unmapped interval that starts at address.  */
/* 
       while (query_size > pagesize)
          {
            query_size = query_size / 2;
            if (address + query_size - 1 >= query_size)
              {
                if (mquery ((void *) address, query_size, 0, MAP_FIXED, -1, 0)
                    != (void *) -1)
                  {
                    /* The interval [address .. address + query_size - 1] is
                       unmapped.  */
/*
                    address += query_size;
                    address_known_mapped = 0;
                  }
                else
                  address_known_mapped = (query_size == pagesize);
              }
          }
        /* Here again query_size = pagesize, and
           either address + pagesize - 1 < pagesize, or
           mquery ((void *) address, pagesize, 0, MAP_FIXED, -1, 0) fails.
           So, the unmapped area ends at address.  */
/* 
     }
      if (address + pagesize - 1 < pagesize) /* wrap around? *//*
        break;
    }
	return 1;
#endif
}*/
/*

int get_free_blocks(void * start_address,void * end_address, vector_c * free_blocks, VECTOR_C_CAP_TYPE BYTES_RESERVED_SIZE)
{
	int z = VECTOR_IS_GOOD_POINTER_C_EX(free_blocks);
	if(z==0)
		return 0;
	VECTOR_INIT_C_EX(free_blocks);
	VECTOR_RESERVE_CFS_EX(free_blocks,(BYTES_RESERVED_SIZE));
	vector_c new_blocks;
	VECTOR_INIT_C_EX(new_blocks);
	VECTOR_RESERVE_CFS_EX(new_blocks,(BYTES_RESERVED_SIZE));
	unsigned long long t1=(unsigned long long)start_address;
	unsigned long long t2=(unsigned long long)end_address;
	unsigned long long ttt=0;
	if(t1!=t2)
	{
		if(t1>t2)
		{
			ttt=t2;
			t2=t1;
			t1=ttt;
			start_address=(void*)t1;
			end_address=(void*)t2;
		}
	} else if(t1!=0) {
		return 0;
	}
	vector_c reserved_blocks;
	int x = get_reserved_blocks(start_address,end_address,&reserved_blocks,BYTES_RESERVED_SIZE);
	if(x==0)
		return 0;
	VECTOR_C_CAP_TYPE total = VECTOR_TOTAL_C(reserved_blocks);
	VECTOR_C_CAP_TYPE i;
	void * item;
	my_memory_block * my_mem_block;
	struct_saver * ss = NULL;
	void * last_address;
	unsigned long long temper = 0;
	unsigned long long temper2 = 0;
	my_memory_block bloc;
	bloc.reserved=0;
	bloc.flags=0;
	unsigned long long j;
	unsigned long long k;
	unsigned long long isss[total];
	unsigned long long isss_n[total];
	unsigned long long isss_p=0;
	unsigned long long isss_p_n=0;
	unsigned long long isss_x[total];
	unsigned long long isss_y[total];
	for(i = 0; i < total; i++)
	{
		isss[i]=0;
		isss_n[i]=0;
		isss_x[i]=0;
		isss_y[i]=0;
	}
	for(i = 0; i < total; i++)
	{
		ss = VECTOR_GET_C_STRUCT(reserved_blocks,i);
		if(ss==NULL)
		{
			continue;
		}
		my_mem_block=(my_memory_block*)ss->item;
		temper=(((unsigned long long)my_mem_block->start_address));
		temper2=(((unsigned long long)my_mem_block->end_address));
		if(temper>isss[(isss_p-isss_p_n)]&&temper2>isss_n[(isss_p-isss_p_n)])
		{
			isss[(isss_p)]=temper;
			isss_n[(isss_p)]=temper2;
			isss_p++;
			isss_p_n=1;
		} else {
			for(j=0; j < isss_p; j++)
			{
				if(temper>isss[(j)]&&temper2>isss_n[(j)])
				{
					continue;
				} else {
					for(k = 0; k < j; k++)
					{
						isss_x[(k)]=isss[(k)];
						isss_y[(k)]=isss_n[(k)];
					}
					isss_x[(j)]=temper;
					isss_y[(j)]=temper2;
					isss_p++;
					for(k = j + 1; k < isss_p; k++)
					{
						isss_x[(k)]=isss[(k-1)];
						isss_y[(k)]=isss_n[(k-1)];
					}
					for(k=0; k < isss_p; k++)
					{
						isss[(k)]=isss_x[(k)];
						isss_n[(k)]=isss_y[(k)];
					}
					j=isss_p;
				}
			}
		}
	}
	
	return 1;
}

int to_one_block(vector_c * blocks, my_memory_block * blockx, VECTOR_C_CAP_TYPE BYTES_RESERVED_SIZE)
{
	int x = VECTOR_IS_GOOD_POINTER_C_EX(blocks);
	if(x==0)
		return 0;
	int y = VECTOR_IS_GOOD_POINTER_C_EX(blockx);
	if(y==0)
		return 0;
	//VECTOR_INIT_C_EX(blocks);
	//VECTOR_RESERVE_CFS_EX(blocks,(BYTES_RESERVED_SIZE));
	unsigned long long total = VECTOR_TOTAL_C_EX(blocks);
	unsigned long long temper=0;
	unsigned long long temper2=0;
	unsigned long long t1;
	unsigned long long t2;
	my_memory_block * my_mem_block;
	my_memory_block bloc;
	for(i = 0; i < total; i++)
	{
		ss = VECTOR_GET_C_STRUCT((*blocks),i);
		if(ss==NULL)
		{ 
			continue;
		}
		my_mem_block=(my_memory_block*)ss->item;
		temper=((unsigned long long)my_mem_block->start_address);
		temper2=((unsigned long long)my_mem_block->end_address);
		
		if(t1==0&&t2==0)
		{
			blockx->reserved=my_mem_block->reserved;
			blockx->flags=my_mem_block->flags;
			t1=temper;
			t2=temper2;
		} else {
			if(temper>t2)
			{
				t2=temper;
			}
			if(temper<t1)
			{
				t1=temper;
			}
			if(temper2>t2)
			{
				t2=temper2;
			}
			if(temper2<t1)
			{
				t1=temper2;
			}
		}
		blockx->start_address=(void*)t1;
		blockx->end_address=(void*)t2;
		return 1;
}
#endif 
*/
