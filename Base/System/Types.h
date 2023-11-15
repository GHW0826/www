#pragma once
#ifndef __TYPES__
#define __TYPES__

// multi platform type support

#if defined(_MSC_VER)
// Windows/Visual C++
using byte		=    unsigned char;
using int8		=      signed char;
using uint8		=    unsigned char;
using int16		=     signed short;
using uint16	=   unsigned short;
using int32		=	    signed int;
using uint32	=     unsigned int;
using int64		=	signed __int64;
using uint64	= unsigned __int64;
#if defined(_WIN64)
#define WATERFALL_64_BIT 1
using IntPtr	=	signed __int64;
using UintPtr	= unsigned __int64;
#else
typedef signed long        intPtr;
typedef unsigned long      uintPtr;
#endif // defined(_WIN64)

#elif defined(__GNUC__)
// Unix/GCC
typedef unsigned char			byte;
typedef signed char				int8;
typedef unsigned char			uint8;
typedef signed short			int16;
typedef unsigned short			uint16;
typedef signed int				int32;
typedef unsigned int			uint32;
typedef signed long				intPtr;
typedef unsigned long			uintPtr;
#if defined(__LP64__)
#define WATERFALL_64_BIT 1
typedef signed long        Int64;
typedef unsigned long      UInt64;
#else
typedef signed long long   Int64;
typedef unsigned long long UInt64;
#endif
#endif // defined(_MSC_VER)

#endif // #ifndef __TYPES__