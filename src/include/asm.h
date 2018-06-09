//
// Created by Carl on 2018/5/23.
//

#ifndef NEWKERNEL_ASM_H
#define NEWKERNEL_ASM_H

#define _asm(arg) __asm__(arg);
#define _asm_volatile(arg) __asm__ __volatile__(arg);

#define cli _asm_volatile("cli")
#define sti _asm_volatile("sti")

#define load_gdt(address)\
_asm_volatile(\
"lgdt (%0)"\
:\
:"a"(address)\
)


#endif //NEWKERNEL_ASM_H
