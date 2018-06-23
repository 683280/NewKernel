//
// Created by Carl on 2018/5/24.
//

#include <video/chr.h>
#include "init/init.h"
#define syscall2(call, arg0, arg1) \
	__asm__("int $0x90;":: "a"(call), "b"(arg0), "c"(arg1))
//这是在用户模式下指向的第一个程序

void init(){

    //TODO 加载bash
    char* a = "init task is running\n";
    char* filename = "a.txt";
//    syscall2(3,1,filename);
//    syscall2(2,3,a);
    __asm__("int $0x90;":: "a"(2));
    while (1){}
}
void init2(){
    while (1){
        __asm__("int $0x90;":: "a"(2));
    }
}