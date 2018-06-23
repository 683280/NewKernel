global s_kernel_entry
global entry_init
extern kmain
extern tss_select

extern cls
;Bootloader Header
MEMINFO     equ  1<<0                 ; 获取内存信息
MODULEALIGN equ  1<<1                 ; align loaded modules on page boundaries
FLAGS       equ  MODULEALIGN | MEMINFO  ; this is the Multiboot 'flag' field
MAGIC       equ   0x1BADB002           ; 'magic number' lets bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)        ; checksum required

section .multiboot
align 4
    MultiBootHeader:
        dd MAGIC
        dd FLAGS
        dd CHECKSUM
;内核入口函数
s_kernel_entry:
        cli
	    mov byte [0xb8000],'C'

        mov esp,0x10000
        push eax

        call cls
        ;初始化cpu
        extern s_init_cpu
        call s_init_cpu

        ;设置tss
        xor	eax, eax
        mov	eax, [tss_select]
        ltr	ax  ;加载tss

        pop eax
        ;跳转到kmain中
        push eax
        push ebx
        call kmain
[global _set_point]
_set_point:
		push ebp
		push eax
		push ebx
		push edx

		mov ebp,esp

		;sub ebp,0x18

		mov ebx,[ss:ebp + 20]

		mov dx,0x3d4
		mov al,0x0e
		out dx,al

		inc dx
		mov al,bh
		out dx,al

		mov dx,0x3d4
		mov al,0x0f
		out dx,al

		inc dx
		mov al,bl
		out dx,al

		pop edx
		pop ebx
		pop eax
		pop ebp
		ret
[global _get_point]
_get_point:
		push dx

		xor eax,eax

		mov dx,0x3d4
		mov al,0x0e
		out dx,al

		inc dx
		in al,dx

		mov ah,al

		mov dx,0x3d4
		mov al,0x0f
		out dx,al

		inc dx
		in al,dx

		pop dx

		ret
	io_delay:
    	nop
    	nop
    	nop
    	nop
    	ret
extern p_curr_proc
extern tss
entry_init:
        mov esp,[p_curr_proc]
        lea eax,[esp + 0x4c]    ;将程序表中的栈顶取地址到eax
        mov [tss + 4],eax       ;将eax赋值到tss中的esp0
    ireturn:    ;在内核栈进入当前中断，不用设置内核栈
        pop eax
        mov cr3,eax

        pop gs
        pop fs
        pop es
        pop ds
        popad
        add esp,4 ;跳过 retaddr 字段
		iret