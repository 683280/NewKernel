global _set_8259a
global _iret
global _iret_data
global _keyboard_idt
global _system_call
global tick
global s_timer_interrupt
global s_save
global s_open_time

extern k_reenter
extern keyboard_idt
extern StackTop
extern tss
extern time
extern program_dispatch
extern sys_call_table
extern iret_data
extern p_curr_proc
extern do_timer
extern jiffies

s_save:
        pushad
        push ds
        push es
        push fs
        push gs
        mov eax,cr3
        push eax
        ;保存CPU状态结束
        mov esi, esp

        inc dword [k_reenter]
        cmp dword [k_reenter], 0
        jne .1 ;不是内核栈 则继续向下执行，否则到 .1处执行
        mov esp, [StackTop]
        push preturn ;
        jmp [esi + 0x34] ; 等于ret 但是不出栈
    .1:
        push ireturn
        jmp [esi + 0x34]

    preturn:    ;在用户栈进入当前中断，需要返回用户栈
        mov esp,[p_curr_proc]
        lea eax,[esp + 0x4c]    ;将程序表中的栈顶取地址到eax
        mov [tss + 4],eax       ;将eax赋值到tss中的esp0
    ireturn:    ;在内核栈进入当前中断，不用设置内核栈
        dec	dword [k_reenter]
        mov [StackTop],esp
        pop eax
        mov cr3,eax
        pop gs
        pop fs
        pop es
        pop ds
        popad
        add esp,4 ;跳过 retaddr 字段
		iret

		;neat program

s_open_time:;将时钟重新打开
    	push eax
    	mov al,0x20
        out 0xa0,al
        nop
        nop
        out 0x20,al
        nop
        nop
        pop eax
        ret

_keyboard_idt:
        call s_save
        inc byte [0xb8000]
        call keyboard_idt
		ret
_set_8259a:
        mov al, 011h
        out 020h, al ; 主8259, ICW1.
        call io_delay
        out 0A0h, al ; 从8259, ICW1.
        call io_delay
        mov al, 020h ; IRQ0 对应中断向量 0x20
        out 021h, al ; 主8259, ICW2.
        call io_delay
        mov al, 028h ; IRQ8 对应中断向量 0x28
        out 0A1h, al ; 从8259, ICW2.
        call io_delay
        mov al, 004h ; IR2 对应从8259
        out 021h, al ; 主8259, ICW3.
        call io_delay
        mov al, 002h ; 对应主8259的IR2
        out 0A1h, al ; 从8259, ICW3.
        call io_delay
        mov al, 001h
        out 021h, al ; 主8259, ICW4.
        call io_delay
        out 0A1h, al ; 从8259, ICW4.
        call io_delay
        mov al, 11111100b ; 仅仅开启定时器中断
        ;mov al, 11111111b ; 屏蔽主8259所有中断
        out 021h, al ; 主8259, OCW1.
        call io_delay
        mov al, 11111111b ; 屏蔽从8259所有中断
        out 0A1h, al ; 从8259, OCW1.
        call io_delay
        ret

	io_delay:
        nop
        nop
        nop
        nop
        ret

_iret:
		iret
_iret_data:
        call s_save
		pop eax
		call iret_data
		ret
;时钟中断
s_timer_interrupt:
        call s_save
        inc dword [jiffies]
        call do_timer
   end:
        call s_open_time
        ret
