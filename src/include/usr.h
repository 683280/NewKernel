//
// Created by carljay on 17-8-29.
//

#ifndef NEWKERNEL_USR_H
#define NEWKERNEL_USR_H

/**
 * �����ں˴�����
 * @param edx ����1
 * @param ecx ����2
 * @param ebx ����3
 * @param n �������
 * @return
 */
int _sys_call(int edx,int ecx,int ebx,int n);
int u_send_message(MESSAGE* message);
#endif //NEWKERNEL_USR_H
