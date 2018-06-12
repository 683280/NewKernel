//
// Created by carljay on 17-8-28.
//

#include <sys/sys_table.h>
#include <sys/sys_call.h>
#include <video/chr.h>
int print_cr3(){
    int cr3;
    __asm__("movl %%cr3,%%eax":"=a" (cr3));
    printf("cr3 = %x",cr3);
    return 0;
}
fn_ptr sys_call_table[] = {exit,message_sendrecv,print_cr3};


int block(PROCESS* pro) {
    time = 1;
    pro->p_flag = WAITNG;
    return 0;
}

int unblock(PROCESS* pro) {
    pro->p_flag = RUNNING;
    return 0;
}



