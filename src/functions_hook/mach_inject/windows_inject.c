#include "windows_inject.h"
#include "whereami.h"
#include "whereami.c"
char * get_current_directory_os()
{
        static char dir[1024];
        memset((void*)&dir[0],0,1024);
	int length = wai_getExecutablePath(0, 0, 0);
	if(length>=1024)
		return (char*)0;
        int dirname_length;
        wai_getExecutablePath((char*)&dir[0], length, &dirname_length);
        return (char*)&dir[0];
}
char get_current_os_seperator()
{
	static char seperator=0;
	if(seperator!=0)
		return seperator;
	#ifdef OS_WIN
		seperator='\\';
	#else
		seperator='/';
	#endif
	return seperator;
}

char * _string_to_c_string(const char * sstring,unsigned int * sstring_size)
{
	static char string[1024];
	static unsigned int string_size_preserv;
	memset(string,0,1024);
	static unsigned int string_size;
	if(sstring==0)
		return (char*)0;
	if(sstring_size==0)
		sstring_size=(unsigned int *)&string_size_preserv;
	string_size=0;
	while(sstring[string_size]!=0&&string_size<1023)
	{
		string[string_size]=sstring[string_size];
		string_size++;
	}
	string[string_size]=0;
	*sstring_size=string_size;
	return (char*)&string[0];
}

char * _uint_to_c_string(unsigned long long x,unsigned int * string_size)
{
        static char string[1024];
	static unsigned int string_size_preserv;
	memset(string,0,1024);
	if(string_size==0)
		string_size=(unsigned int*)&string_size_preserv;
        if(x==0)
	{
		string[0]=48;
		return (char*)&string[0];
	}
	unsigned long long y=x;
        unsigned long long z=0;
        string[1023]=0;
        int i = 1022;
        while(i>=0&&y>0)
        {
                z=y%(unsigned long long)10;
                z+=48;
                y/=(unsigned long long)10;
                string[i]=(char)z;
                i--;
        }
	i++;
	*string_size=1023-i;
        return (char*)&string[i];
}
#include <stdio.h>

int char_buffer_to_file(char * buffer, unsigned long long buffer_length, unsigned long long copy_size, char * copyto)
{
	if(buffer==0||copyto==0||buffer_length==0)
		return 0;
	FILE * xfile = fopen(copyto,"wb");
	if(xfile==0)
		return 0;
	unsigned long long already_copyed=0;
	unsigned long long temp;
	if(copy_size==0||copy_size>buffer_length)
		copy_size=buffer_length;
	while(already_copyed<buffer_length)
	{
		temp=buffer_length-already_copyed;
		if(temp<copy_size)
			copy_size=temp;
		fwrite((char*)&buffer[already_copyed],sizeof(char),copy_size,xfile);
		already_copyed+=copy_size;
	}
	fclose(xfile);
	return 1;
}

int file_to_char_pointer(const char * file, const char * copyto )
{
        if(file==0||copyto==0)
                return 0;
        char buffer[4096];
        char xxbuffer[4096];
        FILE * xfile = fopen(file,"rb");
        if(xfile==0)
                return 0;
        unsigned int i=0;
        unsigned int j=0;
        unsigned int string_size=0;
        unsigned int temp=0;
        char * string;
        string=_string_to_c_string("static char * lib_memory()\n{\n unsigned long long x = 0;\n char * b = (char*)malloc( ", &temp); 
        memcpy((void*)&xxbuffer[string_size],(void*)&string[0],temp);
        string_size+=temp;
        xxbuffer[string_size]=0;
        unsigned long long fsize = 0;
        int r=0;
       while((r = fread(buffer, sizeof(char), 4096, xfile))>0)
        {
                fsize+=r;
        }
        fclose(xfile);
        string=_uint_to_c_string(fsize,&temp);
        memcpy((void*)&xxbuffer[string_size],(void*)&string[0],temp);
        string_size+=temp;
        xxbuffer[string_size]=0;
        string=_string_to_c_string(" );\n",&temp);
        memcpy((void*)&xxbuffer[string_size],(void*)&string[0],temp);
        string_size+=temp;
        FILE * yfile = fopen(copyto,"wb");
        if(yfile==0)
        {
                fclose(xfile);
                return 0;
        }
        fwrite(xxbuffer, sizeof(char), string_size, yfile);
        xfile=fopen(file,"rb");
        if(xfile==0)
        {
                fclose(yfile);
                return 0;
        }
        string=_string_to_c_string(" b[x++] = (char)                                                ;\n",&temp);
        memcpy((void*)&xxbuffer[0],(void*)&string[0],temp);
        string_size=temp;
        xxbuffer[string_size]=0;
        int ttemp=0;
       char * yy=0;
	char ttest[4];
	ttest[0]=255;
	ttest[1]=255;
	ttest[2]=255;
	ttest[3]=0;
	ttemp=3;
        for(temp=0;temp<string_size;temp++)
        {
                if(xxbuffer[temp]==')')
                {
                        temp++;
                        break;
                }
        }
	int temp_y=0;
        while((r = fread(buffer, sizeof(char), 4096, xfile))>0)
       {
                for(i=0;i<r;i++)
                {
                        yy=(char*)_uint_to_c_string((unsigned long long)((int)((unsigned char)buffer[i])),&ttemp);
			//yy=(char*)&ttest[0];
			//printf("%s\n",yy);
			//printf("%d\n",ttemp);
                        for(j=0;j<ttemp&&xxbuffer[temp+j]!=';';j++)
                        {
                                if( ((int)yy[j]) < 48 || ((int)yy[j]) > 57)
                                        break;
                                xxbuffer[temp+j]=yy[j];
                        }
			//printf("1X: %s\n",xxbuffer);
			temp_y=j;
                        fwrite(xxbuffer, sizeof(char), string_size, yfile);
			//fputs(xxbuffer,yfile);
			fflush(yfile);
			for(j=0;xxbuffer[temp+j]!=';'&&j<temp_y;j++)
				xxbuffer[temp+j]=' ';
                }
        }
        fclose(xfile);
        yy=(char*)_string_to_c_string(" return b;\n}\n ",&ttemp);
        fwrite(yy, sizeof(char), ttemp, yfile);
        fclose(yfile);
        return 1;
}

#if defined(OS_WIN)
#include "windows_regs.h"
#include "windows_regs.c"
int inject_libs(const char*libraryPath,int is_x64_libs)
{
	static BOOL is_x64=FALSE;
	#ifdef IS_X64
		is_x64=TRUE;
	#endif
	BOOL x64=FALSE;
	if(is_x64==TRUE&&is_x64_libs==TRUE)
	{
		x64=TRUE;
	} else {
		x64=(BOOL)2;
	}
	static const char winpath[]="SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows";
	static const char signkey[]="RequireSignedAppInit_DLLs";
	static const char init_dlls[]="AppInit_DLLs";
	HKEY hKey;
	LONG nError = OpenRegKeyA((HKEY)HKEY_LOCAL_MACHINE, (LPCSTR)&winpath[0],(PHKEY)&hKey,x64);
	if(REG_KEY_OK(nError)!=TRUE)
	{
		return 0;
	}
        DWORD nValue=0;
        SetRegKeyA(hKey, REG_DWORD, signkey,(const char*)&nValue[0]);
        nError=SetRegKeyA(hKey,REG_SZ,init_dlls,libraryPath);
        CloseRegKeyA(hKey);
        if(REG_KEY_OK(nError)==TRUE)
                return 1;
        else
                return 0;
}

#elif defined(OS_MAC)
#include <stdlib.h>
int inject_libs(const char * libraryPath,int is_x64_libs)
{
	int e = setenv("DYLD_INSERT_LIBRARIES", libraryPath, 1);
	e++;
	return e;
}
#elif defined(OS_UNIX_STRUCT)
int inject_libs(const char * libraryPath,int is_x64_libs)
{
        int e = setenv("LD_PRELOAD", libraryPath, 1);
	e++;
	return e;
}
#endif
int inject_libs_portable(const char * libraryPath)
{
	#ifndef PORTABLE_X64
		#ifdef IS_X64
			#define PORTABLE_X64 1
		#else
			#define PORTABLE_X64 0
		#endif
	#endif
	return inject_libs(libraryPath,PORTABLE_X64);
}
