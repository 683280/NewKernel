//
// Created by Carl on 2018/4/29.
//

#ifndef NEWKERNEL_TASK_H
#define NEWKERNEL_TASK_H

#include <mm.h>
#include <gdt_tool.h>
#include <chr.h>
#include <ipc.h>
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
