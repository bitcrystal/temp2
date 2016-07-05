#ifndef MINIZIP_H
/*
   minizip.c
   Version 1.01e, February 12th, 2005

   Copyright (C) 1998-2005 Gilles Vollant
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

#ifdef unix
# include <unistd.h>
# include <utime.h>
# include <sys/types.h>
# include <sys/stat.h>
#else
# include <direct.h>
# include <io.h>
#endif

#include "zip.h"

#ifdef WIN32
#define USEWIN32IOAPI
#include "iowin32.h"
#endif



#define WRITEBUFFERSIZE (16384)
#define MAXFILENAME (256)
uLong filetime__minizip(char *f,  tm_zip *tmzip, uLong *dt);
int check_exist_file__minizip(const char* filename);
void do_banner__minizip();
void do_help__minizip();
int getFileCrc__minizip(const char* filenameinzip,void*buf,unsigned long size_buf,unsigned long* result_crc);
int main__minizip(int argc,char *argv[]);
#endif