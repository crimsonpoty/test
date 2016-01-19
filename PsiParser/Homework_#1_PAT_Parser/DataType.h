/**
 @file  	DataType.h
 @date  	2014. 01. 07
 @author	Native1 Team
 @brief 	일반적으로 쓰이는 쓰이는 데이터 타입에 대한 typedef
 @remarks
*/
#ifndef _UTILS_DATATYPES_H_
#define _UTILS_DATATYPES_H_

#include <stdint.h> 
#include <ctime>

#define KSTD_UNUSED(x) ((void)x)

typedef unsigned long   ulong;
typedef long long int64;
typedef int int32;
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned int uint;
typedef short int16;
typedef unsigned short uint16;
typedef char int8;
typedef unsigned char uint8;
typedef unsigned short ushort;
typedef unsigned char uchar;

#if (defined(__CPLUSPLUS__) || defined (__cplusplus))
#else 
/* [2006. 12. 14] saramis C99 support _Bool type as bool */
	typedef _Bool			 		bool;
	#define true				1
	#define false				0
#endif

#endif // _UTILS_DATATYPES_H_
