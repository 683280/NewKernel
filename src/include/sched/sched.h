//
// Created by Carl on 2018/4/29.
//

#ifndef NEWKERNEL_SCHED_H
#define NEWKERNEL_SCHED_H

#include <type.h>
#include <x86/gdt.h>

#define RUNNING 0
#define WAITNG 1

typedef struct s_stackframe {
    u32 cr3;
    u32 gs; /* 0 */
    u32 fs; /* 4 */
    u32 es; /* 8 */
    u32 ds; /* c */
    //popad
    u32 edi; /* 10 */
    u32 esi; /* 14 pushed by save( ) */
    u32 ebp; /* 18 */
    u32 kernel_esp; /* 1c <- 'popad' will ignore it */
    u32 ebx; /* 20 */
    u32 edx; /* 24 */
    u32 ecx; /* 28 */
    u32 eax; /* 2c */

    u32 retaddr; /* 30 return addr for kernel.asm::save( ) */
    //iretd
    u32 eip; /* 34 */
    u32 cs; /* 38 */
    u32 eflags; /* 3c | pushed by CPU during interrupt */

    u32 esp; /* 40 */
    u32 ss; /* 44 */
}STACK_FRAME;

//进程体
typedef struct s_proc {
    STACK_FRAME regs; /* 保存cpu当前状态 */
    int p_flag;
    u8 priority;    //优先级
    u8 time_film;   //时间片
    u32 pid; // 进程ID 对应在proc_list中的位置
    char p_name[16]; /* 进程名称 */
}PROCESS;

typedef struct s_tss {
    u32	backlink;
    u32	esp0;		/* stack pointer to use during interrupt */
    u32	ss0;		/*   "   segment  "  "    "        "     */
    u32	esp1;
    u32	ss1;
    u32	esp2;
    u32	ss2;
    u32	cr3;
    u32	eip;
    u32	flags;
    u32	eax;
    u32	ecx;
    u32	edx;
    u32	ebx;
    u32	esp;
    u32	ebp;
    u32	esi;
    u32	edi;
    u32	es;
    u32	cs;
    u32	ss;
    u32	ds;
    u32	fs;
    u32	gs;
    u32	ldt;
    u16	trap;
    u16	iobase;	/* I/O位图基址大于或等于TSS段界限，就表示没有I/O许可位图 */
    /*u8	iomap[2];*/
}TSS;

extern TSS tss;

u32 time;

//进程列表
extern PROCESS proc_list[4096];
//进程数量
extern u32 proc_num;

extern u16 is_end;
//当前进程指针
extern PROCESS* p_curr_proc;
extern void _process_dispatch();

int get_program(PROCESS**pPROCESS);

//时钟中断处理程序
void do_timer();
void program_dispatch();

#endif //NEWKERNEL_SCHED_H
