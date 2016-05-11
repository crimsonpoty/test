#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <byteswap.h>
#include <list>

// #define bit_swap_16(p)	bswap_16(*(const ushort * const)p)
// #define bit_swap_32(p)	bswap_32(*(const ushort * const)p)
// #define bit_swap_64(p)	bswap_64(*(const ushort * const)p)
// #define DVB_PID(p)	((bit_swap_16(p)) & 0x1FFF)
#define FLAG0(x) (x & 0x80) >> 7	// '10000000' �ڸ��� Flag ���� ��ũ��
#define FLAG1(x) (x & 0x40) >> 6	// '01000000' �ڸ��� Flag ���� ��ũ��
#define FLAG2(x) (x & 0x20) >> 5	// '00100000' �ڸ��� Flag ���� ��ũ��
#define FLAG3(x) (x & 0x10) >> 4	// '00010000' �ڸ��� Flag ���� ��ũ��
#define FLAG4(x) (x & 0x08) >> 3	// '00001000' �ڸ��� Flag ���� ��ũ��
#define FLAG5(x) (x & 0x04) >> 2	// '00000100' �ڸ��� Flag ���� ��ũ��
#define FLAG6(x) (x & 0x02) >> 1	// '00000010' �ڸ��� Flag ���� ��ũ��
#define FLAG7(x) x & 1				// '00000001' �ڸ��� Flag ���� ��ũ��
#define PARSER_PRINT(x) printf x;

typedef unsigned long   ulong;
typedef long long int64 ;
typedef int int32 ;
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

void char_buffer_dump(const char *buffer, const int size, const int mode);

#endif