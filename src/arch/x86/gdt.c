//
// Created by carljay on 17-7-31.
//

#include <x86/gdt_tool.h>
#include <x86/gdt.h>
#include <sched/sched.h>
#include <asm.h>
#include <video/chr.h>

DESCRIPTOR gdt[GDT_SIZE] = {};

void init_gdt() {
    gdt_size = 0;
    lgdt.p_address = (u32)&gdt;
    //NULL
    code_select = add_gdt(getDescriptor(0,0xfffff,type_code));
    data_select = add_gdt(getDescriptor(0,0xfffff,type_data));
    stack_select = add_gdt(getDescriptor(0,0xfffff,type_data));
    tss_select = add_gdt(getDescriptor(&tss,104,type_tss));

    tss.ss0 = stack_select;
    tss.esp = 0x10000;
    load_gdt((&lgdt));
    __asm__ __volatile__("push %%eax\n"
            "movl %0,%%eax\n"
            "movl %%eax,%%ds\n"
            "movl %%eax,%%es\n"
            "movl %%eax,%%gs\n"
            "movl %%eax,%%fs\n"

            "movl %1,%%eax\n"
            "movl %%eax,%%ss\n"
            "ljmp $8,$(_sta)\n"
            "_sta:\n"
            "pop %%eax\n"
    :
    :"r"(data_select),"r"(stack_select),"r"(code_select)
   );

    printf("gdt inited\n");
}

u32 add_gdt(DESCRIPTOR desc) {
    gdt_size++;
    gdt[gdt_size] = desc;
    lgdt.size = ((gdt_size + 1) * 8) - 1;
    u8 dpl = get_bit(desc.h,13,2);
    return ((gdt_size) << 3) | dpl;
}
DESCRIPTOR getDescriptor(u32 address,u32 lenght,u32 type){
    int desc = address << 16;
    desc = desc | (lenght & 0xffff);
    DESCRIPTOR dt = {desc,0};

    desc = get_bit(address,24,8) << 23;
    desc = desc | (get_bit(type,8,4) << 20);
    desc = desc | (get_bit(lenght,16,4) << 16);
    desc = desc | (get_bit(type,0,8) << 8);
    desc = desc | (get_bit(address,16,8));
    dt.h = desc;
    return dt;
}