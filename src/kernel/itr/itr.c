//
// Created by Carl on 2018/5/29.
//

#include "itr/itr.h"
#include <video/chr.h>
#include <type.h>
#include <mm/symbols.h>
#include <mm/paging.h>

struct interrupt_frame
{
    u32 ip;
    u32 cs;
    u32 flags;
    u32 sp;
    u32 ss;
};
void test111(struct interrupt_frame frame){

}

void die(char* fmt,...){
    va_list args;
    kprintf("\e[31m[\e[32m%08x\e[31m]\e[0m  kernel error \n",1);
    kprintf(fmt,args);
}
void do_div_by_zero(){
    printf("do_div_by_zero\n");
}
void do_single_step(){
    printf("do_single_step\n");
}
void do_nmi(){
    printf("do_nmi\n");
}
void do_breakpoint(){
    printf("do_breakpoint\n");
}
void do_overflow(){
    printf("do_overflow\n");
}
void do_bounds(){
    printf("do_bounds\n");
}
void do_invalid_opcode(){
    printf("do_invalid_opcode\n");
}
void do_coprocessor_not_available(u32 cr2,/* u32 *ebp, */struct pusha_regs regs, struct iret_regs iregs){
    symbol_t * symbol1;
    symbol1 = sym_find_object(iregs.eip,NULL);
    kprintf("<==============[Coprocessor Not Available Exception]====================>\n");
    kprintf("\e[31m[\e[32m%08x\e[31m]\e[0m  kernel error \n",cr2);
    kprintf("cs : %08x  eip : %08x\n",iregs.cs,iregs.eip);

    kprintf("error function = %s\n",symbol1->name);
    while (1){}
}
void do_double_fault(int error_code){
    printf("do_double_fault error:%d\n",error_code);
}
void do_coprocessor_segment_overrun(){
    printf("do_coprocessor_segment_overrun\n");
}
void do_invalid_task_state_segment(int error_code){
    printf("do_coprocessor_segment_overrun error:%d\n",error_code);
}
void do_segment_not_present(int error_code){
    printf("do_segment_not_present error:%d\n",error_code);
}
void do_stack_fault(int error_code){
    printf("do_stack_fault error:%d\n",error_code);
}
void do_general_protection_fault(int error_code){
    printf("do_general_protection_fault error:%d\n",error_code);
}
#define bit_is_set(error_code,position) ((error_code & (1 << position)) >> position)
void do_page_fault(int error_code,u32 cr2,/* u32 *ebp, */struct pusha_regs regs, struct iret_regs iregs){

    dprintf("cr2 = 0x%08x    code = %d    P = %d    W/R = %d    U/S = %d    RSVD = %d    I/D = %d\n",cr2,
            error_code,bit_is_set(error_code,0),bit_is_set(error_code,1),bit_is_set(error_code,2),
            bit_is_set(error_code,3),bit_is_set(error_code,4));
    symbol_t * symbol1;
    symbol1 = sym_find_object(iregs.eip,NULL);
    kprintf("<==============[Page Fault Exception]====================>\n");
    kprintf("\e[31m[\e[32m%08x\e[31m]\e[0m  kernel error \n",cr2);


    kprintf("error code = %d \n",error_code);
    kprintf("cs : %08x  eip : %08x\n",iregs.cs,iregs.eip);
    kprintf("error function = %s\n",symbol1->name);
    while (1){}
}
void do_reserved(u32 cr2,/* u32 *ebp, */struct pusha_regs regs, struct iret_regs iregs){
    symbol_t * symbol1;
    symbol1 = sym_find_object(iregs.eip,NULL);
    kprintf("<==============[Reserved Exception]====================>\n");
    kprintf("\e[31m[\e[32m%08x\e[31m]\e[0m  kernel error \n",cr2);
    kprintf("cs : %08x  eip : %08x\n",iregs.cs,iregs.eip);

    kprintf("error function = %s\n",symbol1->name);
    while (1){}
}
void do_math_fault(){
    printf("do_math_fault \n");
}
void do_alignment_check(int error_code){
    printf("do_alignment_check error:%d\n",error_code);
}
void do_machine_check(){
    printf("do_machine_check\n");
}
void do_simd_floating_point_exception(){
    printf("do_simd_floating_point_exception\n");
}
void do_virtualization_exception(){
    printf("do_virtualization_exception\n");
}
void do_control_protection_exception(){
    printf("do_control_protection_exception\n");
}

void int_test(struct s_stackframe iregs){
    dprintf("<<<<<<<<<<Print stack frame>>>>>>>>>>\n");
    dprintf("cr3        = 0x%08x\n",get_pagedir()    );
    dprintf("cr3        = 0x%08x\n",iregs.cr3       );
    dprintf("gs         = 0x%08x\n",iregs.gs        );
    dprintf("fs         = 0x%08x\n",iregs.fs        );
    dprintf("es         = 0x%08x\n",iregs.es        );
    dprintf("ds         = 0x%08x\n",iregs.ds        );
    dprintf("edi        = 0x%08x\n",iregs.edi       );
    dprintf("esi        = 0x%08x\n",iregs.esi       );
    dprintf("ebp        = 0x%08x\n",iregs.ebp       );
    dprintf("kernel_esp = 0x%08x\n",iregs.kernel_esp);
    dprintf("ebx        = 0x%08x\n",iregs.ebx       );
    dprintf("edx        = 0x%08x\n",iregs.edx       );
    dprintf("ecx        = 0x%08x\n",iregs.ecx       );
    dprintf("eax        = 0x%08x\n",iregs.eax       );
    dprintf("retaddr    = 0x%08x\n",iregs.retaddr   );
    dprintf("eip        = 0x%08x\n",iregs.eip       );
    dprintf("cs         = 0x%08x\n",iregs.cs        );
    dprintf("eflags     = 0x%08x\n",iregs.eflags    );
    dprintf("esp        = 0x%08x\n",iregs.esp       );
    dprintf("ss         = 0x%08x\n",iregs.ss        );
}