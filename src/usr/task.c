//
// Created by Carl on 2018/4/29.
//
#include <asm.h>
#include "task.h"

extern TASK tasks[2] = {{TestA,0x2000,'testA'},{TestB,0x2000,'testB'}};
void TestA(){
    MESSAGE msg;
    printf("TestA");
    while (1){
    }
}
void TestB(){
    printf("TestB");
    while(1){
        MESSAGE msg;
        memset(&msg,0, sizeof(MESSAGE));
    }
}
int exec_task(TASK* task){
    PROCESS* pro;
    int i;
    i = get_program(&pro);
    if(i < 0)
        return i;
    pro->pid = i;
    int code = add_gdt(getDescriptor(0x0,0xfffff,setDpl(type_code,3)));
    pro->regs.cs = code;

    u32 a = add_gdt(getDescriptor(0x0,0xfffff,setDpl(type_data,3)));
    pro->regs.ss = a;
    pro->regs.es = a;
    pro->regs.fs = a;
    pro->regs.kernel_esp = 0x1000;
    pro->regs.gs = a;
    pro->regs.ds = a;
    pro->regs.eip= (u32)task->initial_eip;
    pro->regs.esp= (u32) 0x45000 + (i * task->stacksize);
    pro->regs.eflags = 0x1202;
    pro->priority = 10;
    pro->time_film = 10;
    memcopy(&pro->p_name,&task->name,16);
    return i;
//    proc_list[i] = pro;
}

void start_program(){
    time = 100;
    int pid;
    for (int i = 0; i < 2; ++i) {
        TASK* task = &tasks[i];
        pid = exec_task(task);
        printf("pid = %d  ",pid);
    }
    printf("\n");
    load_gdt(&lgdt);
    p_curr_proc = &proc_list[0];
    p_curr_proc->time_film = 10;

//    tss.esp0 = &p_curr_proc->ldt_sel;
}
