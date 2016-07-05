#ifndef MINIUNZ_H
#define MINIUNZ_H
/*
   miniunz.c
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
#else
# include <direct.h>
# include <io.h>
#endif

#include "unzip.h"

#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (8192)
#define MAXFILENAME (256)

#ifdef WIN32
#define USEWIN32IOAPI
#include "iowin32.h"
#endif

/*
  mini unzip, demo of unzip package

  usage :
  Usage : miniunz [-exvlo] file.zip [file_to_extract] [-d extractdir]

  list the file in the zipfile, and print the content of FILE_ID.ZIP or README.TXT
    if it exists
*/


/* change_file_date : change the date/time of a file
    filename : the filename of the file where date/time must be modified
    dosdate : the new date at the MSDos format (4 bytes)
    tmu_date : the SAME new date at the tm_unz format */
#ifdef __cplusplus
extern "C" {
#endif
void do_banner__miniunz();
void do_help__miniunz();
void change_file_date__miniunz(const char *filename,uLong dosdate,tm_unz tmu_date);
int mymkdir__miniunz(const char * dirname);
int makedir__miniunz(const char *newdir);
int do_list__miniunz(unzFile uf);
int do_extract_currentfile__miniunz(unzFile uf,const int* popt_extract_without_path,int* popt_overwrite,const char* password);
int do_extract__miniunz(unzFile uf,int opt_extract_without_path,int opt_overwrite,const char* password);
int do_extract_onefile__miniunz(unzFile uf,const char* filename,int opt_extract_without_path,int opt_overwrite,const char* password);
int main__miniunz(int argc,char *argv[]);
#ifdef __cplusplus
}
#endif
#endif