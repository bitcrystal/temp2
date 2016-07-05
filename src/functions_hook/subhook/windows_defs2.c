#ifndef WINDOWS_DEFS_2_C
#define WINDOWS_DEFS_2_C
#include "windows_defs2.h"
#include "windows_defs.c"
HANDLE WINAPI CreateFile(LPCTSTR lpFileName,DWORD dwDesiredAccess,DWORD dwShareMode,LPSECURITY_ATTRIBUTES lpSecurityAttributes,DWORD dwCreationDisposition,DWORD dwFlagsAndAttributes,HANDLE hTemplateFile)
{
	/*char pp[3];
	if((dwDesiredAccess&(FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE)))
	{
		pp[0]='w';
		pp[1]='+';
		pp[2]=0;
	} else if((dwDesiredAccess&(FILE_SHARE_READ|FILE_SHARE_DELETE))) {
		pp[0]='w';
		pp[1]='+';
		pp[2]=0;
	} else if((dwDesiredAccess&(FILE_SHARE_WRITE|FILE_SHARE_DELETE))) {
		pp[0]='w';
		pp[1]='+';
		pp[2]=0;
	} else if((dwDesiredAccess&(FILE_SHARE_READ|FILE_SHARE_WRITE)))
	{
		pp[0]='w';
		pp[1]='+';
		pp[2]=0;
	} else if((dwDesiredAccess&(FILE_SHARE_READ)))
		pp[0]='r';
		pp[1]=0;
		pp[2]=0;
	} else if((dwDesiredAccess&(FILE_SHARE_WRITE))) {
		pp[0]='w';
		pp[1]=0;
		pp[2]=0;
	}*/
	
	FILE * file = NULL;
	switch(dwCreationDisposition)
	{
		case CREATE_ALWAYS:
		{
			SetLastError(NO_ERROR);
			file = fopen((const char*)lpFileName, "rb");
			if(file!=NULL)
			{
				fclose(file);
				SetLastError(ERROR_ALREADY_EXISTS);
			}
			file = fopen((const char*)lpFileName, "wb+");
			if(file==NULL)
			{
				return NULL;
			}
			___Win32Helper___FileHandleManage((HANDLE)file,0);
			return (HANDLE)file;
		}
		break;
		
		case CREATE_NEW:
		{
			SetLastError(NO_ERROR);
			file = fopen((const char*)lpFileName, "rb");
			if(file!=NULL)
			{
				fclose(file);
				SetLastError(ERROR_FILE_EXISTS);
				return NULL;
			}
			file = fopen((const char*)lpFileName, "wb+");
			if(file==NULL)
			{
				return NULL;
			}
			___Win32Helper___FileHandleManage((HANDLE)file,0);
			return (HANDLE)file;
		}
		break;
		
		case OPEN_ALWAYS:
		{
			SetLastError(NO_ERROR);
			file = fopen((const char*)lpFileName, "rb");
			if(file==NULL)
			{
				file = fopen((const char*)lpFileName, "wb+");
				if(file==NULL)
					return NULL;
				___Win32Helper___FileHandleManage((HANDLE)file,0);
				return (HANDLE)file;
			}
			fclose(file);
			file = fopen((const char*)lpFileName, "ab+");
			if(file==NULL)
				return NULL;
			SetLastError(ERROR_ALREADY_EXISTS);
			___Win32Helper___FileHandleManage((HANDLE)file,0);
			return (HANDLE)file;
		}
		
		case OPEN_EXISTING:
		{
			SetLastError(NO_ERROR);
			file = fopen((const char*)lpFileName, "rb");
			if(file==NULL)
			{
				SetLastError(ERROR_FILE_NOT_FOUND);
				return NULL;
			}
			fclose(file);
			file = fopen((const char*)lpFileName, "ab+");
			if(file==NULL)
				return NULL;
			___Win32Helper___FileHandleManage((HANDLE)file,0);
			return (HANDLE)file;
		}
		
		case TRUNCATE_EXISTING:
		{
			SetLastError(NO_ERROR);
			file = fopen((const char*)lpFileName, "rb");
			if(file==NULL)
			{
				SetLastError(ERROR_FILE_NOT_FOUND);
				return NULL;
			}
			fclose(file);
			file = fopen((const char*)lpFileName, "wb+");
			if(file==NULL)
				return NULL;
			___Win32Helper___FileHandleManage((HANDLE)file,0);
			return (HANDLE)file;
		}
		break;
	}
	return NULL;
}

BOOL WINAPI ReadFile(HANDLE hFile,LPVOID lpBuffer,DWORD nNumberOfBytesToRead,LPDWORD lpNumberOfBytesRead,LPOVERLAPPED lpOverlapped)
{
	if(___Win32Helper___IsFileHandle(hFile)==FALSE)
	{
		return FALSE;
	}
	if(nNumberOfBytesToRead<=0)
	{
		return FALSE;
	}
	
	if(lpNumberOfBytesRead==NULL)
	{
		return FALSE;
	}
	*lpNumberOfBytesRead = 0;
	char mystring[4096];
	memset((void*)mystring,255,4096);
	FILE* pFile=(FILE*)hFile;
	DWORD filepp=0;
	DWORD filexx=0;
	DWORD llength=0;
	char * buff_pointer=(char*)lpBuffer;
	char * curr_pointer = NULL;
	curr_pointer=(char*)fgets ((char*)mystring , 255 , pFile);
	if(curr_pointer==NULL)
		return FALSE;
	 while ( curr_pointer != NULL )
	 {
	   for(filepp=4095;filepp>=0;filepp--)
	   {
			if(mystring[filepp]==255||mystring[filepp]==-127)
			{
				continue;
			}
			llength=(filepp+1);
			*lpNumberOfBytesRead+=llength;
	   }
       for(filepp=0;filepp<llength&&filexx<nNumberOfBytesToRead;filepp++,filexx++)
	   {
			buff_pointer[filexx]=mystring[filepp];
	   }
	   memset(mystring,255,4096);
	   if(feof(pFile)!=0)
	   {
		return TRUE;
	   }
	   curr_pointer=(char*)fgets ((char*)mystring , 4096 , pFile);
	 }
	 if(*lpNumberOfBytesRead>0)
		return TRUE;
	return FALSE;
}

BOOL WINAPI WriteFile(HANDLE hFile,LPCVOID lpBuffer,DWORD nNumberOfBytesToWrite,LPDWORD lpNumberOfBytesWritten,LPOVERLAPPED lpOverlapped)
{
	if(___Win32Helper___IsFileHandle(hFile)==FALSE)
	{
		return FALSE;
	}
	if(nNumberOfBytesToWrite<=0)
	{
		return FALSE;
	}
	
	if(lpNumberOfBytesWritten==NULL)
	{
		return FALSE;
	}
	*lpNumberOfBytesWritten=0;
	char mystring[4096];
	memset((void*)mystring,0,4096);
	FILE* pFile=(FILE*)hFile;
	DWORD filepp=0;
	DWORD filexx=0;
	DWORD x = 0;
	DWORD y = 0;
	int nwrited=0;
	char * curr_pointer=NULL;
	curr_pointer=(char*)lpBuffer;
	x = (DWORD)(nNumberOfBytesToWrite / (DWORD)4096);
	y = (DWORD)(nNumberOfBytesToWrite % (DWORD)4096);
	DWORD i = 0;
	for(x=x;x>0;x--)
	{
		for(filepp=0;filepp<4096&&filexx<nNumberOfBytesToWrite;filepp++,filexx++)
		{
			mystring[filepp]=curr_pointer[filexx];
		}
		nwrited=fputs(mystring,pFile);
		if(nwrited<0)
		{
			if(*lpNumberOfBytesWritten==0)
			{
				return FALSE;
			}
			return TRUE;
		}
		*lpNumberOfBytesWritten+=nwrited;
		memset((void*)mystring,0,4096);
	}
	if(y>0)
	{
		memset((void*)mystring,0,4096);
		for(filepp=0;filepp<y&&filexx<nNumberOfBytesToWrite;filepp++,filexx++)
		{
			mystring[filepp]=curr_pointer[filexx];
		}
		nwrited=fputs(mystring,pFile);
		if(nwrited<0)
		{
			if(*lpNumberOfBytesWritten==0)
			{
				return FALSE;
			}
			return TRUE;
		}
		*lpNumberOfBytesWritten+=nwrited;
	}
	*lpNumberOfBytesWritten=nNumberOfBytesToWrite;
	return TRUE;
}

DWORD WINAPI SetFilePointer(HANDLE hFile,LONG lDistanceToMove,PLONG lpDistanceToMoveHigh,DWORD dwMoveMethod)
{
	SetLastError(NO_ERROR);
	if(___Win32Helper___IsFileHandle(hFile)==FALSE)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return INVALID_SET_FILE_POINTER;
	}
	FILE* pFile = (FILE*)hFile;
	MY_NUM64_BASE distance;
	MY_NUM_BASE distance_x;
	memset((void*)&distance.value_char[0],0,8);
	memset((void*)&distance_x.value_char[0],0,4);
	distance.u_value=0;
	distance_x.u_value=0;
	if(lpDistanceToMoveHigh!=NULL)
	{
		distance_x.value=*lpDistanceToMoveHigh;
		distance.value_char[4]=distance_x.value_char[0];
		distance.value_char[5]=distance_x.value_char[1];
		distance.value_char[6]=distance_x.value_char[2];
		distance.value_char[7]=distance_x.value_char[3];
		memset((void*)&distance_x.value_char[0],0,4);
		distance_x.u_value=0;
	}
	distance_x.value=lDistanceToMove;
	distance.value_char[0]=distance_x.value_char[0];
	distance.value_char[1]=distance_x.value_char[1];
	distance.value_char[2]=distance_x.value_char[2];
	distance.value_char[3]=distance_x.value_char[3];
	if((((LONG64)distance.value) < ((LONG64)0)))
	{
		SetLastError(ERROR_NEGATIVE_SEEK);
		return INVALID_SET_FILE_POINTER;
	}
	if(dwMoveMethod==FILE_BEGIN)
	{
		MY_NUM_BASE dx;
		MY_NUM64_BASE dy;
		MY_NUM_BASE dz;
		MY_NUM64_BASE dw;
		memset((void*)&dx.value_char[0],0,4);
		memset((void*)&dy.value_char[0],0,8);
		memset((void*)&dz.value_char[0],0,4);
		memset((void*)&dw.value_char[0],0,8);
		dx.u_value=0x7FFFFFFF;
		dy.u_value=(distance.u_value/dx.u_value);
		dz.u_value=(distance.u_value%dx.u_value);
		fseek ( pFile , 0 , SEEK_SET );
		for(dw.u_value=0;dw.u_value<dy.u_value;dw.u_value++)
		{
			fseek( pFile, (long)dx.value, SEEK_CUR); 
		}
		if(dz.value>0)
		{
			fseek( pFile, (long)dz.value, SEEK_CUR); 
		}
	} else if(dwMoveMethod==FILE_END)
	{
		char c;
		MY_NUM_BASE dx;
		MY_NUM64_BASE dy;
		MY_NUM_BASE dz;
		MY_NUM64_BASE dw;
		memset((void*)&dx.value_char[0],0,4);
		memset((void*)&dy.value_char[0],0,8);
		memset((void*)&dz.value_char[0],0,4);
		memset((void*)&dw.value_char[0],0,8);
		MY_NUM64_BASE xp;
		xp.u_value=0;
		fseek ( pFile , 0 , SEEK_SET );
		do
		{
			c=fgetc(pFile);
			xp.u_value++;
		} while(c != EOF);
		fseek ( pFile , 0 , SEEK_SET );
		if(xp.u_value>0)
		{
			xp.u_value-=1;
		} else {
		    SetLastError(ERROR_NEGATIVE_SEEK);
			return INVALID_SET_FILE_POINTER;
		}
		if(xp.u_value<distance.u_value)
		{
		    SetLastError(ERROR_NEGATIVE_SEEK);
			return INVALID_SET_FILE_POINTER;
		}
		xp.u_value-=distance.u_value;
		if(xp.u_value>0)
		{
			dx.u_value=0x7FFFFFFF;
			dy.u_value=(xp.u_value/dx.u_value);
			dz.u_value=(xp.u_value%dx.u_value);
			for(dw.u_value=0;dw.u_value<dy.u_value;dw.u_value++)
			{
				fseek( pFile, (long)dx.value, SEEK_CUR); 
			}
			if(dz.value>0)
			{
				fseek( pFile, (long)dz.value, SEEK_CUR); 
			}
			return *((DWORD*)&distance.value_char[0]);
		} else {
			return *((DWORD*)&distance.value_char[0]);
		}
	} else if(dwMoveMethod==FILE_CURRENT)
	{
		MY_NUM_BASE dx;
		MY_NUM64_BASE dy;
		MY_NUM_BASE dz;
		MY_NUM64_BASE dw;
		memset((void*)&dx.value_char[0],0,4);
		memset((void*)&dy.value_char[0],0,8);
		memset((void*)&dz.value_char[0],0,4);
		memset((void*)&dw.value_char[0],0,8);
		dx.u_value=0x7FFFFFFF;
		dy.u_value=(distance.u_value/dx.u_value);
		dz.u_value=(distance.u_value%dx.u_value);
		for(dw.u_value=0;dw.u_value<dy.u_value;dw.u_value++)
		{
			fseek( pFile, (long)dx.value, SEEK_CUR); 
		}
		if(dz.value>0)
		{
			fseek( pFile, (long)dz.value, SEEK_CUR); 
		}
		return *((DWORD*)&distance.value_char[0]);
	}
	SetLastError(ERROR_INVALID_PARAMETER);
	return INVALID_SET_FILE_POINTER;
}

BOOL ___Win32Helper___FileHandleManage(HANDLE fh,int action)
{
	if(fh==NULL)
		return FALSE;
	static HANDLE fhandles[64];
	static unsigned char init=0;
	if(init!=99)
	{
		int i=0;
		for(i=0; i < 64; i++)
		{
			fhandles[i]=NULL;
		}
		init=99;
	}
	if(action==0)
	{
		int i=0;
		for(i=0; i < 64; i++)
		{
			if(fhandles[i]==NULL)
			{
				fhandles[i]=fh;
				return TRUE;
			}
		}
		return FALSE;
	} else if (action == 1)
	{
		int i = 0;
		for(i=0; i < 64; i++)
		{
			if(fhandles[i]==fh)
			{
				fhandles[i]=NULL;
				return TRUE;
			}
		}
		return FALSE;
	} else if ( action == 2)
	{
		int i = 0;
		for(i=0; i < 64; i++)
		{
			if(fhandles[i]==fh)
			{
				return TRUE;
			}
		}
		return FALSE;
	} else {
		return FALSE;
	}
	return FALSE;
}

BOOL ___Win32Helper___IsFileHandle(HANDLE hObject)
{
	BOOL isFh = ___Win32Helper___FileHandleManage(hObject,2);
	if(isFh==TRUE)
	{
		return TRUE;
	}
	return FALSE;
}
 
BOOL CloseHandle(HANDLE hObject)
{
	if(___Win32Helper___IsFileHandle(hObject)==TRUE)
	{
		fclose((FILE*)hObject);
		___Win32Helper___FileHandleManage(hObject,1);
		return TRUE;
	}
	return FALSE;
}

void WINAPI SetLastError(DWORD set)
{
	______MY_LAST_ERROR______=set;
}

DWORD WINAPI GetLastError()
{
	return (DWORD)______MY_LAST_ERROR______;
}

#endif
