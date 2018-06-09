//
// Created by Carl on 2018/5/24.
//

#include <chr.h>
#include "init.h"
#define syscall2(call, arg0, arg1) \
	asm("int $0x80;":: "a"(call), "b"(arg0), "c"(arg1));
//这是在用户模式下指向的第一个程序
void init(){

    printf("init started\n");
    //TODO 加载bash
//    syscall2(2,0,0);
    __asm__("int $0x90;"::"a"(2), "b"(0), "c"(0));
    while (1){}
}