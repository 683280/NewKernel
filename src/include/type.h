//
// Created by carljay on 17-7-31.
//

#ifndef NEWKERNEL_TYPE_H
#define NEWKERNEL_TYPE_H

#include <va_list.h>

#define MIN(a, b) ((a<b)?a:b)
#define MAX(a, b) ((a>b)?a:b)

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned long u64;

typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;
typedef unsigned long long 	uint64_t;

#define NULL (void*)0
#define __align(A)       __attribute__((__aligned__(A)))         //!< Aligns the data A bytes

#endif //NEWKERNEL_TYPE_H
