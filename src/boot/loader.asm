global _entry_loader
global _load_disk
global _point
extern lmain

kernel_size	    equ  120
kernel_sector	equ  20
entry_loader:
	mov esp,0x1000
	mov byte [0xb8000],'D'
    call lmain
_point:
    dd 0
_load_disk:
		mov ebx,0x3000
		mov dx,0x1f2
		mov al,kernel_size
		out dx,al
		mov eax,kernel_sector

		inc dx
		mov cl,8
		out dx,al

		inc dx
		shr eax,cl
		out dx,al

		inc dx
		shr eax,cl
		out dx,al

		inc dx                          ;0x1f6
        shr eax,cl
        or al,0xe0                      ;第一硬盘  LBA地址27~24
        out dx,al

        inc dx                          ;0x1f7
        mov al,0x20                     ;读命令
        out dx,al
	.waits:
        in al,dx
        and al,0x88
        cmp al,0x08
        jnz .waits                      ;不忙，且硬盘已准备好数据传输

        mov ecx,256 * kernel_size                 ;总共要读取的字数
        mov dx,0x1f0
  	.readw:
        in ax,dx
        mov [ebx],ax
        add ebx,2
        loop .readw

        ret
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