//
// Created by Carl on 2018/4/29.
//

#ifndef NEWKERNEL_SYS_TABLE_H
#define NEWKERNEL_SYS_TABLE_H
typedef int (*fn_ptr)();

extern int sys_exit();
//extern int load(char*);

extern int message_sendrecv();

extern fn_ptr sys_call_table[] ;


#endif //NEWKERNEL_SYS_TABLE_H
