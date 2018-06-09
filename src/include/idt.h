//
// Created by 13342 on 2017/8/6.
//

#ifndef NEWKERNEL_IDT_H
#define NEWKERNEL_IDT_H

#include <gdt.h>
#include <chr.h>
#include <io.h>

#define IDT_SIZE 255

#define TaskGate 0x8D00   //任务门
#define InterruptGate 0x8e00  //中断门
#define TrapGate 0x8F00   //陷阱门

#define set_dpl(idt,dpl) idt & (dpl << 13)
#define _load_igdt(address)__asm__ volatile("lidt (%0)"::"r"(address));

extern void _test();
void _set_8259a();
short idt_lenght;

typedef struct{
    int address;//偏移地址
    short select;//选择器
    int type;//类型
}IDT_DESC;

#pragma pack(1)
typedef struct{
    u16 size;       //IDT_TABLE size
    u32 p_address;  //IDT_TABLE address
}I_DESC_TABLE;
typedef struct {
    u32 l;//低32位   0 - 15 偏移量(0-15) 16 - 32 段选择子
    u32 h;//高32位   0 - 4 保留, 5-7 000,8 - 10 门类型(101 任务门,110 中断门,111 陷阱门,调用门 陷阱门改权限),
          //        11 标志位 0=16位 1=32位,12 0,13 - 14 权限级,15 段是否存在,16 - 32 偏移量(16-32)
}IDT_TABLE;
I_DESC_TABLE lidt;

extern IDT_TABLE idt[];

#pragma pack()
int k_reenter;
int StackTop;//内核栈

void setup_irq(int irqs[],int size);
void setup_syscall_idt(int syscall);
void _iret();
void _iret_data();
void iret_data(int state);
void _keyboard_idt();
void keyboard_idt();


//asm
void s_timer_interrupt();

#endif //NEWKERNEL_IDT_H
