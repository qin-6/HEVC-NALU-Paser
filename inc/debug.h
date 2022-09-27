#ifndef __DEBUG_H
#define __DEBUG_H 

#if defined(_WIN32) || defined(_WIN16)
#   define NV_WINDOWS
#endif

#if defined(__linux__) && !defined(NV_LINUX) && !defined(NV_VMWARE)
#   define NV_LINUX
#endif  /* defined(__linux__) */

#if defined(NV_LINUX)
#ifdef _DEBUG
#define debug(format, args...) fprintf (stderr, format, args)
#else 
#define debug(format, args...)
#endif
#endif

#if defined(NV_WINDOWS)
#ifdef _DEBUG
#define debug(format, ...) fprintf (stderr, format, __VA_ARGS__)
#else 
#define debug(format, ...)
#endif
#endif

#endif
