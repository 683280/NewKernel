extern s_save

;中断异常处理程序
global s_div_by_zero
global s_single_step
global s_nmi
global s_breakpoint
global s_overflow
global s_bounds
global s_invalid_opcode
global s_coprocessor_not_available
global s_double_fault
global s_coprocessor_segment_overrun
global s_invalid_task_state_segment
global s_segment_not_present
global s_stack_fault
global s_general_protection_fault
global s_page_fault
global s_reserved
global s_math_fault
global s_alignment_check
global s_machine_check
global s_simd_floating_point_exception
global s_virtualization_exception
global s_control_protection_exception
;C中断异常处理程序
extern do_div_by_zero
extern do_single_step
extern do_nmi
extern do_breakpoint
extern do_overflow
extern do_bounds
extern do_invalid_opcode
extern do_coprocessor_not_available
extern do_double_fault
extern do_coprocessor_segment_overrun
extern do_invalid_task_state_segment
extern do_segment_not_present
extern do_stack_fault
extern do_general_protection_fault
extern do_page_fault
extern do_reserved
extern do_math_fault
extern do_alignment_check
extern do_machine_check
extern do_simd_floating_point_exception
extern do_virtualization_exception
extern do_control_protection_exception

errcode:
        dd 0
;被0除
s_div_by_zero:
        call s_save
        call do_div_by_zero
        ret
s_single_step:
        call s_save
        call do_single_step
        ret
s_nmi:
        call s_save
        call do_nmi
        ret
s_breakpoint:
        call s_save
        call do_breakpoint
        ret
s_overflow:
        call s_save
        call do_overflow
        ret
s_bounds:
        call s_save
        call do_bounds
        ret
s_invalid_opcode:
        call s_save
        call do_invalid_opcode
        ret
s_coprocessor_not_available:
        cli
        pop dword [errcode]
        pusha

        mov eax, cr2
        push eax
        call do_coprocessor_not_available
        add esp, 4
        popa
        iret
s_double_fault:
        call s_save
        pop eax
        call do_double_fault
        ret
s_coprocessor_segment_overrun:
        call s_save
        call do_coprocessor_segment_overrun
        ret
s_invalid_task_state_segment:
        call s_save
        pop eax
        call do_invalid_task_state_segment
        ret
s_segment_not_present:
        call s_save
        pop eax
        call do_segment_not_present
        ret
s_stack_fault:
        call s_save
        pop eax
        call do_stack_fault
        ret
s_general_protection_fault:
        call s_save
        pop eax
        call do_general_protection_fault
        ret
s_page_fault:
        cli
        pop dword [errcode]
        pusha

        mov eax, cr2
        push eax
        push dword [errcode]
        call do_page_fault
        add esp, 8
        popa
        iret
s_reserved:
        cli
        pop dword [errcode]
        pusha

        mov eax, cr2
        push eax
        call do_reserved
        add esp, 4
        popa
        iret
s_math_fault:
        call s_save
        call do_math_fault
        ret
s_alignment_check:
        call s_save
        pop eax
        call do_alignment_check
        ret
s_machine_check:
        call s_save
        call do_machine_check
        ret
s_simd_floating_point_exception:
        call s_save
        call do_simd_floating_point_exception
        ret
s_virtualization_exception:
        call s_save
        call do_virtualization_exception
        ret
s_control_protection_exception:
        call s_save
        call do_control_protection_exception
        ret