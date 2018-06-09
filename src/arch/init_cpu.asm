global s_init_cpu

extern init_cpu
extern init_gdt
extern init_itr

s_init_cpu:

        call init_cpu
        ;初始化gdt
        call init_gdt
        ;初始化中断
        call init_itr
        ret