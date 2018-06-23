//
// Created by Carl on 2018/4/30.
//

#include <x86/idt.h>
#include <video/print.h>
#include <sched/proc.h>
#include <io/cmode.h>
#include "sys/sys.h"

void init_sys(){
    setup_syscall_idt(&s_sys_call);
}

int do_sys_call(int edx,int ecx,int ebx,int eax,PROCESS *pro){

    return sys_call_table[eax](edx,ecx,ebx,pro);
}

int sys_exit(int a, int c, int b, PROCESS *pro){
    proc_list[pro->pid].p_flag = 0;
}
int sys_printf(int edx,char* buf,int size,PROCESS *pro){
    char* _buf = vir_to_physics(pro->regs.cr3,buf);
    dprintf("cr3 = 0x%08x   buf = 0x%08x   _buf 0x%08x\n",pro->regs.cr3,buf,_buf);
    for (int i = 0; i < size; ++i) {
        vga_put(*_buf++);
    }
    repoint();
}