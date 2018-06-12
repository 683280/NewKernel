//
// Created by Carl on 2018/5/29.
//

#include "itr/itr.h"
#include <video/chr.h>
#include <type.h>
#include <mm/symbols.h>

struct interrupt_frame
{
    u32 ip;
    u32 cs;
    u32 flags;
    u32 sp;
    u32 ss;
};
__interrupt void test111(struct interrupt_frame* frame){

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
void do_page_fault(int error_code,u32 cr2,/* u32 *ebp, */struct pusha_regs regs, struct iret_regs iregs){
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