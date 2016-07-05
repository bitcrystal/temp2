/* iowin32.h -- IO base function header for compress/uncompress .zip
   files using zlib + zip or unzip API
   This IO API version uses the Win32 API (for Microsoft Windows)

   Version 1.01e, February 12th, 2005

   Copyright (C) 1998-2005 Gilles Vollant
*/
#include "../functions_hook/subhook/my_predef.h"
#ifdef OS_WIN
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(OS_WIN) && defined(OS_UNIX_STRUCT)
#include "../functions_hook/subhook/windows_platform_defs.h"
#endif

#ifndef OS_UNIX_STRUCT
	void fill_win32_filefunc OF((zlib_filefunc_def* pzlib_filefunc_def));
#endif

#ifdef __cplusplus
}
#endif
