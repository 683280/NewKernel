//
// Created by 13342 on 2017/8/6.
//

#include "io.h"

u16 in_word(u16 port){
    unsigned short data;
    __asm__ __volatile__(//"push %%eax\n"
    "push %%dx\n"
            "movw %1,%%dx\n"
            "in %%dx,%%ax\n"
            "movw %%ax,%0\n"
            "pop %%dx\n"
//            "pop %%eax\n"
    :"=r"(data)
    :"d"(port)
    :

    );
    return data;
}