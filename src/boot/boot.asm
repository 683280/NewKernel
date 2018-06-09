	

	core_load_address 	equ  0x00001000
	gdt_load_address	equ  0x00007e00
	kernel_size	equ  0x8

	[bits 16]
		jmp _start
	msg_enter_mbr:
		db "enter_mbr"
	msg_enter_protect:
		db "enter_protect"
	_start:
		mov ax,0xb800
		mov ds,ax

		mov ax,0x7c00
		mov sp,ax

		mov si,msg_enter_mbr
		mov cx,msg_enter_protect
		call _print_msg
		jmp _enter_protect
_print_msg:
		push dx
		push ax
		push es
		push si

		mov ax,0xb800
		mov es,ax

		sub cx,si
		mov ax,0x7c0
		mov ds,ax

		xor di,di

	loop_print:
		movsb
		inc di
		loop loop_print

		pop si
		pop es
		pop ax
		pop dx
		ret
		

_enter_protect:
		mov eax,[cs:gdt_desc + 0x7c00 + 0x2]
		xor edx,edx
		mov ebx,16
		div ebx

		mov ds,eax
		mov ebx,edx

		;跳过0#号描述符的槽位 
         ;创建1#描述符，这是一个数据段，对应0~4GB的线性地址空间
         mov dword [ebx+0x08],0x0000ffff    ;基地址为0，段界限为0xFFFFF
         mov dword [ebx+0x0c],0x00cf9300    ;粒度为4KB，存储器段描述符

         ;创建保护模式下初始代码段描述符
         mov dword [ebx+0x10],0x7c000fff    ;基地址为0x00007c00，界限0x1FF
         mov dword [ebx+0x14],0x00409800    ;粒度为1个字节，代码段描述符 

         ;建立保护模式下的堆栈段描述符      ;基地址为0x00007C00，界限0xFFFFE 
         mov dword [ebx+0x18],0x0000ffff    ;粒度为4KB
         mov dword [ebx+0x1c],0x00cf9610
         
         ;建立保护模式下的显示缓冲区描述符   
         mov dword [ebx+0x20],0x80007fff    ;基地址为0x000B8000，界限0x07FFF 
         mov dword [ebx+0x24],0x0040920b    ;粒度为字节

         ;创建保护模式下初始代码段描述符
         mov dword [ebx+0x28],0x0000ffff    ;基地址为0x00007c00，界限0x1FF
         mov dword [ebx+0x2c],0x00cf9b00    ;粒度为1个字节，代码段描述符
         
         ;初始化描述符表寄存器GDTR
         mov word [cs: gdt_desc+0x7c00],47      ;描述符表的界限   
 
         lgdt [cs: gdt_desc+0x7c00]
      
         in al,0x92                         ;南桥芯片内的端口 
         or al,00000010B
         out 0x92,al                        ;打开A20

         cli                                ;中断机制尚未工作

         mov eax,cr0
         or eax,1
         mov cr0,eax                        ;设置PE位
      
         ;以下进入保护模式... ...
         jmp dword 0x0010:flush             ;16位的描述符选择子：32位偏移
[bits 32]

flush:	;ds:ebx
		mov cx,kernel_size
		mov eax,0x0008
		mov ds,eax
		mov eax,1
		mov ebx,core_load_address
		call _load_disk

		mov eax,0x08
		mov ds,eax

        mov eax,0x8
        mov ss,eax
        mov gs,eax
        mov es,eax
        mov fs,eax

		jmp dword 0x28:core_load_address
        ;pushf
        ;push dword 0x28
        ;mov eax,[core_load_address + 0x18]
        ;add eax,0x1000
        ;push dword eax
        ;mov eax,0x00001
        ;iretd
		;jmp dword 0x0028:[0x00100000 + 0x1c]
    show:
        mov eax,0x00000020
        mov ds,ax

        mov byte [0],"D"
        jmp $
_load_disk:
		push eax

		mov dx,0x1f2
		mov al,cl 
		out dx,al
		pop eax

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

    gdt_desc:
    	dw 0
    	dd gdt_load_address
    times 0x1be - ($ - $$) db 0
    	db 0,01,01,00,07,0xFE,0xFF,0xFF
    	dd 70,0x32000
	times 510 - ($ - $$) db 0
		dw 0xAA55
