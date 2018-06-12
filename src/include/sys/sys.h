//
// Created by Carl on 2018/4/30.
//

#ifndef NEWKERNEL_SYS_H
#define NEWKERNEL_SYS_H

#include <sys/sys_table.h>
#include <sched/sched.h>
extern void s_sys_call();

//系统调用
int exit(int i, int c, int b, PROCESS *pro);

#endif //NEWKERNEL_SYS_H
