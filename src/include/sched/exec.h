//
// Created by Carl on 2018/5/24.
//

#ifndef NEWKERNEL_EXEC_H
#define NEWKERNEL_EXEC_H

#include "sched/sched.h"
//系统调用，执行程序
int sys_exec(int edx,char* filename, int flag, PROCESS* curr);
#endif //NEWKERNEL_EXEC_H
