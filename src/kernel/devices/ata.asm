
global s_ata_read_sectors
global s_ata_lba_read
;;u32 lba,u32 size,u32 buf
s_ata_read_sectors:
        push eax
        push ebx
        push ecx
        push edx
        push ebp

        mov ebp,esp

		mov ebx,[ebp + 32]  ;;要读取数据的存放位置

		mov eax,[ebp + 28]    ;kernel_size

		mov dx,0x172;0x1f2
		out dx,al           ;将要读取的扇区大小写出  0x1f2
        jmp .1
    .1:
		mov eax,[ebp + 24]   ;kernel_sector 要读取的扇区的开始lba地址

		inc dx              ;0x1f3
		mov cl,8
		out dx,al
        jmp .2
    .2:

		inc dx              ;0x1f4
		shr eax,cl
		out dx,al
        jmp .3
    .3:

		inc dx              ;0x1f5
		shr eax,cl
		out dx,al
        jmp .4
    .4:

		inc dx                          ;0x1f6
        shr eax,cl
        or al,0xe0                      ;第一硬盘  LBA地址27~24
        out dx,al
        jmp .5
    .5:
        inc dx                          ;0x1f7
        mov al,0x20                     ;读命令
        out dx,al
        nop
        nop

	.waits:
	    in al,dx
	    and al,0x88
        cmp al,0x08
        jnz .waits
        ;test al,8
        ;jz .waits                      ;不忙，且硬盘已准备好数据传输

        mov eax,256
        mov ecx,[ebp + 28]
        mul cx
        mov ecx,eax     ;kernel_size                 ;总共要读取的字数
        mov dx,0x170;0x1f0
  	.readw:
        in ax,dx
        mov [ebx],ax
        add ebx,2
        loop .readw

        pop ebp
        pop edx
        pop ecx
        pop ebx
        pop eax

        ret

;=============================================================================
; ATA read sectors (LBA mode)
;
; @param EAX Logical Block Address of sector
; @param CL  Number of sectors to read
; @param edi The address of buffer to put data obtained from disk
;
; @return None
;=============================================================================
s_ata_lba_read:
        pushf
        and eax, 0x0FFFFFFF
        push eax
        push ebx
        push ebx
        push edx
        push edi

        mov ebx, eax         ; Save LBA in ebx

        mov edx, 0x0176      ; Port to send drive and bit 24 - 27 of LBA
        shr eax, 24          ; Get bit 24 - 27 in al
        or al, 11100000b     ; Set bit 6 in al for LBA mode
        out dx, al

        mov edx, 0x0172      ; Port to send number of sectors
        mov al, cl           ; Get number of sectors from CL
        out dx, al

        mov edx, 0x173       ; Port to send bit 0 - 7 of LBA
        mov eax, ebx         ; Get LBA from EBX
        out dx, al

        mov edx, 0x174       ; Port to send bit 8 - 15 of LBA
        mov eax, ebx         ; Get LBA from EBX
        shr eax, 8           ; Get bit 8 - 15 in AL
        out dx, al


        mov edx, 0x175       ; Port to send bit 16 - 23 of LBA
        mov eax, ebx         ; Get LBA from EBX
        shr eax, 16          ; Get bit 16 - 23 in AL
        out dx, al

        mov edx, 0x177       ; Command port
        mov al, 0x20         ; Read with retry.
        out dx, al

.still_going:
        in al, dx
        test al, 8           ; the sector buffer requires servicing.
        jz .still_going      ; until the sector buffer is ready.

        mov eax, 256         ; to read 256 words = 1 sector
        xor bx, bx
        mov bl, cl           ; read CL sectors
        mul bx
        mov ebx, eax         ; ebx is counter for INSW
        mov edx, 0x170       ; Data port, in and out
        rep insw             ; in to [edi]

        pop edi
        pop edx
        pop ebx
        pop ebx
        pop eax
        popf
        ret