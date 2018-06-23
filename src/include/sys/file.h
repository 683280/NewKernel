//
// Created by Carl on 2018/6/14.
//

#ifndef NEWKERNEL_FILE_H
#define NEWKERNEL_FILE_H

#include <sched/sched.h>

int sys_fopen(const char* filename, int mode, PROCESS* proc);
#endif //NEWKERNEL_FILE_H
