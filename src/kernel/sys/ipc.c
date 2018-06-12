//
// Created by 13342 on 2018/3/29.
//

#include <video/chr.h>
#include <sys/ipc.h>
#include <sched/sched.h>

int message_sendrecv(int func, int c, int b, PROCESS *pro){
    if(func == SEND){
        return send_message((MESSAGE*)c,pro);
    }else if(func == RECV){
        return recv_message((MESSAGE*)c,pro);
    }else if(func == CONN){
        return open_message_conn(((MESSAGE*)c)->msg);
    }

}
int open_message_conn(int msgid){

}
int send_message(MESSAGE* msg, PROCESS *pro){
//    block(pro);
    printf("send_message msg ");
//    printf(msg->data);
    return 0;
}

int recv_message(MESSAGE* msg, PROCESS *pro){
    printf("recv_message");
//    block(pro);
}
void set(int* p,int i,int a,int b){
    i = i * 2;
    *(p + i) = a;
    *(p + i + 1) = b;
}