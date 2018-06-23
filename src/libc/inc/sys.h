//
// Created by Carl on 2018/6/13.
//

#ifndef LIBC_SYS_H
#define LIBC_SYS_H

#define ALLOC_PAGING 1
#define FREE_ALLOC_PAGING ALLOC_PAGING+1


#define sys_call1(a) \
__asm__("int $0x90;"::"a"(a))
#define sys_call2(a,b) \
__asm__("int $0x90;"::"a"(a), "b"(b))
#define sys_call3(a,b,c) \
__asm__("int $0x90;"::"a"(a), "b"(b), "c"(c))
#define sys_call4(a,b,c,d) \
__asm__("int $0x90;"::"a"(a), "b"(b), "c"(c), "c"(d))
#endif //LIBC_SYS_H
