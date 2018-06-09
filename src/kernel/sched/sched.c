//
// Created by Carl on 2018/4/29.
// 进程调度
//

#include <chr.h>
#include "sched.h"

extern void s_open_time();
TSS tss;
PROCESS proc_list[4096];
u32 proc_num;
u16 is_end = 0;
PROCESS* p_curr_proc = 0;
long volatile jiffies=0;
int pid = 0;

int get_program(PROCESS**pPROCESS){
    int tmp = pid;
    *pPROCESS = &proc_list[tmp];
    pid++;
    return tmp;
}


void program_dispatch() {

//    printf("curr_proc = %s\n",&p_curr_proc->p_name);

//    printf("p_curr_proc = %x  time_film %x\n",p_curr_proc,p_curr_proc->time_film);

    int i = p_curr_proc->pid;
//    printf("pid = %d",i);
    while (1) {
        if (++i >= pid)
            i = 0;
        p_curr_proc = &proc_list[i];
        p_curr_proc->time_film = p_curr_proc->priority;
//        printf("   pid = %d\n",i);
//        printf("program_dispatch\n");
        break;
    }
}
void do_timer(){
//    printf("do_timer exec jiffies = %d\n",jiffies);
    --p_curr_proc->time_film;
    if(p_curr_proc->time_film == 0)
        program_dispatch();

}