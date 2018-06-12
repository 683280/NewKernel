//
// Created by Carl on 2018/4/29.
//

#ifndef NEWKERNEL_TASK_H
#define NEWKERNEL_TASK_H

#include <mm/mm.h>
#include <x86/gdt_tool.h>
#include <video/chr.h>
#include <sys/ipc.h>
typedef	void	(*task_f)	();
typedef struct s_task {
    task_f	initial_eip;//程序入口
    int	stacksize; //栈大小
    char	name[16]; //程序别称
}TASK;
void TestA();
void TestB();
void start_program();
#endif //NEWKERNEL_TASK_H
