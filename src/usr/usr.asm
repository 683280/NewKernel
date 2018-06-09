global _sys_call

_sys_call:
        mov eax, [esp + 16] ;º¯Êý±àºÅ
        mov	ebx, [esp + 12]	;²ÎÊý1
        mov	ecx, [esp + 8]	;
        mov	edx, [esp + 4]	;
        int 0x90
        ret