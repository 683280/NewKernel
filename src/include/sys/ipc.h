//
// Created by carljay on 17-8-28.
//

#ifndef NEWKERNEL_IPC_H
#define NEWKERNEL_IPC_H


#include "sys_call.h"

/**
 * пе╨е
 */
typedef struct{
    int msg;
    char len;
    void* data;
}MESSAGE;
#define SEND 1
#define RECV 2
#define CONN 3
int message_sendrecv(SYS_CALL_PARA);
int open_message_conn(int msgid);
int send_message(MESSAGE* msg, PROCESS *pro);
int recv_message(MESSAGE* msg, PROCESS *pro);
#endif //NEWKERNEL_IPC_H
