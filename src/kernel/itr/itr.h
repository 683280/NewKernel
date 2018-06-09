//
// Created by Carl on 2018/5/29.
//

#ifndef NEWKERNEL_ITR_H
#define NEWKERNEL_ITR_H

void init_itr();

//中断处理函数
void do_div_by_zero();
void do_single_step();
void do_nmi();
void do_breakpoint();
void do_overflow();
void do_bounds();
void do_invalid_opcode();
void do_coprocessor_not_available();
void do_double_fault(int error_code);
void do_coprocessor_segment_overrun();
void do_invalid_task_state_segment(int error_code);
void do_segment_not_present(int error_code);
void do_stack_fault(int error_code);
void do_general_protection_fault(int error_code);
void do_page_fault(int error_code);
void do_reserved();
void do_math_fault();
void do_alignment_check(int error_code);
void do_machine_check();
void do_simd_floating_point_exception();
void do_virtualization_exception();
void do_control_protection_exception();

#endif //NEWKERNEL_ITR_H
