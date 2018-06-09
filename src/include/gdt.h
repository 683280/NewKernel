//
// Created by carljay on 17-7-31.
//

#ifndef NEWKERNEL_GDT_H
#define NEWKERNEL_GDT_H

#include "type.h"
#define GDT_SIZE 100

//GDT描述符
typedef struct {
    u32 l;
    u32 h;
}DESCRIPTOR;

#pragma pack(1)
//GDTR
typedef struct{
    u16 size;
    u32 p_address;
}DESC_TABLE;
#pragma pack()
DESC_TABLE lgdt;//GDTR指向这里


extern DESCRIPTOR gdt[GDT_SIZE];//描述符存放位置

u32 gdt_size;

const static u16 type_data = 0xC92; //数据段type
const static u16 type_data_1 = 0x4a2;   //数据段
const static u16 type_code = 0xC98; //代码段
const static u16 type_stack = 0xC96;//栈段
const static u16 type_ldt = 0x482;  //陷阱段
const static u16 type_tss = 0x889;  //tss段

int code_select;    //内核代码段选择子
int data_select;    //内核数据段选择子
int stack_select;   //内核栈选择子
int tss_select;     //TSS段选择子

void init_gdt();
u32 add_gdt(DESCRIPTOR desc);
DESCRIPTOR getDescriptor(u32 address,u32 lenght,u32 type);
#endif //NEWKERNEL_GDT_H
