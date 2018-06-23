//
// Created by carljay on 17-7-31.
//

#ifndef NEWKERNEL_TYPE_H
#define NEWKERNEL_TYPE_H

#include <va_list.h>

#define MIN(a, b) ((a<b)?a:b)
#define MAX(a, b) ((a>b)?a:b)

typedef char      s8;  //!< Signed 8 bit value
typedef short     s16; //!< Signed 16 bit value
typedef int       s32; //!< Signed 32 bit value
typedef long long s64; //!< Signed 64 bit value

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned long u64;

typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;
typedef uint32_t	uint64_t;

typedef u32 size_t;

#define NULL (void*)0
#define __align(A)       __attribute__((__aligned__(A)))         //!< Aligns the data A bytes
#define __interrupt     __attribute__((interrupt))
#endif //NEWKERNEL_TYPE_H
