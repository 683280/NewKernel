//
// Created by 13342 on 2017/8/6.
//

#ifndef NEWKERNEL_IO_H
#define NEWKERNEL_IO_H

#include <type.h>
#include <asm.h>

#define outb(port,value) \
__asm__ ("outb %%al,%%dx"::"a" (value),"d" (port))


#define inb(port) ({ \
unsigned char _v; \
__asm__ volatile ("inb %%dx,%%al":"=a" (_v):"d" (port)); \
_v; \
})

//#define out_byte(port,data) \
//__asm__ ("outb %%al,%%dx\n" \
//		"\tjmp 1f\n" \
//		"1:\tjmp 1f\n" \
//		"1:"::"a" (data),"d" (port));
//#define in_byte(port) ({\
//unsigned char _v; \
//__asm__ volatile ("inb %%dx,%%al\n" \
//	"\tjmp 1f\n" \
//	"1:\tjmp 1f\n" \
//	"1:":"=a" (_v):"d" (port)); \
//_v; \
//})

void io_wait();
#endif //NEWKERNEL_IO_H
