global _sys_call

_sys_call:
        mov eax, [esp + 16] ;�������
        mov	ebx, [esp + 12]	;����1
        mov	ecx, [esp + 8]	;
        mov	edx, [esp + 4]	;
        int 0x90
        ret