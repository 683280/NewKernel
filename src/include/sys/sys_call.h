//
// Created by carljay on 17-8-28.
//

#ifndef NEWKERNEL_SYS_CALL_H
#define NEWKERNEL_SYS_CALL_H

#include <sched/sched.h>
#define SYS_CALL_PARA int func, int c, int b, PROCESS *pro
#define SYS_CALL_MSG 0


int block(PROCESS* pro);
int unblock(PROCESS* pro);


#endif //NEWKERNEL_SYS_CALL_H
