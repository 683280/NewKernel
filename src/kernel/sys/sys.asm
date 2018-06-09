global s_sys_call

extern s_save
extern s_open_time
extern sys_call_table
extern p_curr_proc

s_sys_call:
        call s_save
        sli
        push esi
        push dword [p_curr_proc]
        push ebx
        push ecx
        push edx
        call [sys_call_table + eax * 4]
        add esp,4 * 4
        pop esi
        mov [esi + 0x2c],eax
        sti
        call s_open_time
        ret