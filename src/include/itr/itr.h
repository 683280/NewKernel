//
// Created by Carl on 2018/5/29.
//

#ifndef NEWKERNEL_ITR_H
#define NEWKERNEL_ITR_H

#include <type.h>
#include <sched/sched.h>

struct pusha_regs {
    uint32_t edi, esi;
    uint32_t ebp, esp;
    uint32_t ebx, edx, ecx, eax;
};

struct iret_regs {
    uint32_t eip, cs;
    uint32_t eflags;
    uint32_t esp, ds;
};

void init_itr();

//中断处理函数
void do_div_by_zero();
void do_single_step();
void do_nmi();
void do_breakpoint();
void do_overflow();
void do_bounds();
void do_invalid_opcode();
void do_coprocessor_not_available(u32 cr2,/* u32 *ebp, */struct pusha_regs regs, struct iret_regs iregs);
void do_double_fault(int error_code);
void do_coprocessor_segment_overrun();
void do_invalid_task_state_segment(int error_code);
void do_segment_not_present(int error_code);
void do_stack_fault(int error_code);
void do_general_protection_fault(int error_code);
void do_page_fault(int error_code,u32 cr2,/* u32 *ebp, */struct pusha_regs regs, struct iret_regs iregs);
void do_reserved(u32 cr2,/* u32 *ebp, */struct pusha_regs regs, struct iret_regs iregs);
void do_math_fault();
void do_alignment_check(int error_code);
void do_machine_check();
void do_simd_floating_point_exception();
void do_virtualization_exception();
void do_control_protection_exception();

void int_test(struct s_stackframe iregs);
#endif //NEWKERNEL_ITR_H
