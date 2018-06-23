//
// Created by Carl on 2018/4/30.
//

#ifndef NEWKERNEL_SYS_H
#define NEWKERNEL_SYS_H

#include <sys/sys_table.h>
#include <sched/sched.h>
extern void s_sys_call();

void init_sys();
//系统调用
int sys_exit(int i, int c, int b, PROCESS *pro);
int sys_printf(int edx,char* buf,int size,PROCESS *pro);
int do_sys_call(int edx,int ecx,int ebx,int eax,PROCESS *pro);

#endif //NEWKERNEL_SYS_H
