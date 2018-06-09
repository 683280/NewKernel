//
// Created by Carl on 2018/5/29.
//

#include "itr.h"
#include <chr.h>

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
void do_coprocessor_not_available(){
    printf("do_coprocessor_not_available\n");
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
void do_page_fault(int error_code){
    printf("do_page_fault error:%d\n",error_code);
}
void do_reserved(){
    printf("do_reserved");
    while (1){};
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