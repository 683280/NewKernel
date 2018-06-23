global s_sys_call

extern s_save
extern s_open_time
extern sys_call_table
extern do_sys_call
extern p_curr_proc

s_sys_call:
        call s_save
        cli
        push esi
        push dword [p_curr_proc]
        push eax
        push ebx
        push ecx
        push edx
        call do_sys_call
        add esp,5 * 4
        pop esi
        mov [esi + 0x2c],eax
        sti
        ret