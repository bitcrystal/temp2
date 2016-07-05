#ifndef MHOOK_PREDEF_H
#define MHOOK_PREDEF_H
#include "subhook/my_predef.h"
#if defined(OS_WIN)
#define USE_MHOOK
#elif defined(OS_UNIX_STRUCT)
#define USE_POLYHOOK
#else
#define USE_SUBHOOK
#endif


//MY DEFINES
#define USE__POLYHOOK
//MY DEFINES END


#if defined(USE__MHOOK)&&defined(OS_WIN)
#ifdef USE_POLYHOOK
#undef USE_POLYHOOK
#endif

#ifdef USE_MHOOK
#undef USE_MHOOK
#endif

#ifdef USE_SUBHOOK
#undef USE_SUBHOOK
#endif

#define USE_MHOOK
#elif defined(USE__SUBHOOK)

#ifdef USE_MHOOK
#undef USE_MHOOK
#endif

#ifdef USE_SUBHOOK
#undef USE_SUBHOOK
#endif

#ifdef USE_POLYHOOK
#undef USE_POLYHOOK
#endif
#define USE_SUBHOOK
#elif defined(USE__POLYHOOK)

#ifdef USE_MHOOK
#undef USE_MHOOK
#endif

#ifdef USE_SUBHOOK
#undef USE_SUBHOOK
#endif

#ifdef USE_POLYHOOK
#undef USE_POLYHOOK
#endif
#define USE_POLYHOOK

#endif

#endif
