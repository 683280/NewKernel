//
// Created by carljay on 17-8-29.
//

#ifndef NEWKERNEL_USR_H
#define NEWKERNEL_USR_H

/**
 * 调用内核处理函数
 * @param edx 参数1
 * @param ecx 参数2
 * @param ebx 参数3
 * @param n 函数编号
 * @return
 */
int _sys_call(int edx,int ecx,int ebx,int n);
int u_send_message(MESSAGE* message);
#endif //NEWKERNEL_USR_H
