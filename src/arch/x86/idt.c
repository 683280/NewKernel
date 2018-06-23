#include <x86/idt.h>
#include <sys/sys.h>
#include <sched/sched.h>
#include <devices/keymap.h>
#include <itr/pic.h>

IDT_TABLE idt[IDT_SIZE];

extern void s_div_by_zero();
extern void s_single_step();
extern void s_nmi();
extern void s_breakpoint();
extern void s_overflow();
extern void s_bounds();
extern void s_invalid_opcode();
extern void s_coprocessor_not_available();
extern void s_double_fault();
extern void s_coprocessor_segment_overrun();
extern void s_invalid_task_state_segment();
extern void s_segment_not_present();
extern void s_stack_fault();
extern void s_general_protection_fault();
extern void s_page_fault();
extern void s_reserved();
extern void s_math_fault();
extern void s_alignment_check();
extern void s_machine_check();
extern void s_simd_floating_point_exception();
extern void s_virtualization_exception();
extern void s_control_protection_exception();

typedef void (*irq_handler)();
irq_handler irq_handlers[] = {s_div_by_zero,                           // 0x0
                              s_single_step,                           // 0x1
                              s_nmi ,                                  // 0x2
                              s_breakpoint,                            // 0x3
                              s_overflow,                              // 0x4
                              s_bounds,                                // 0x5
                              s_invalid_opcode,                        // 0x6
                              s_coprocessor_not_available,             // 0x7
                              s_double_fault,                          // 0x8
                              s_coprocessor_segment_overrun,           // 0x9
                              s_invalid_task_state_segment,            // 0xa
                              s_segment_not_present,                   // 0xb
                              s_stack_fault,                           // 0xc
                              s_general_protection_fault,              // 0xd
                              s_page_fault,                            // 0xe
                              s_reserved,                              // 0xf
                              s_math_fault,                            // 0x10
                              s_alignment_check,                       // 0x11
                              s_machine_check,                         // 0x12
                              s_simd_floating_point_exception,         // 0x13
                              s_virtualization_exception,              // 0x14
                              s_control_protection_exception};         // 0x15

void init_itr(){
    setup_irq(&irq_handlers, sizeof(irq_handlers) / 4);
    add_idt(code_select,((int)&s_timer_interrupt) ,0x8e00,0x20);
    add_idt(code_select,((int)&s_page_fault) ,0x8e00,0x2f);
    add_idt_dpl(code_select,((int)&s_sys_call) ,0x8e00,0x90,3);
    k_reenter = -1;
    printf("idt inited\n");
//    setup_syscall_idt();
}

void add_idt_dpl(int select,int address,int type,int n,u8 dpl){
    IDT_TABLE* idt1 = &idt[n];

    unsigned int desc;
    desc = select << 16;
    int tmp = address & 0xffff;
    desc = desc | tmp;
    idt1->l = desc;

    type = type | (dpl << 13);
    desc = address & 0xffff0000;
    desc = desc | type;
    idt1->h = desc;
}


void add_idt(int select,int address,int type,int n){
    add_idt_dpl(select,address,type,n,0);
}

void keyboard_idt(){
    char a = inb(0x60);
    char aa = keymap[(a&0x7F)*3];
    char make = (a & FLAG_BREAK ? 0 : 1);
    if(make){
        if(aa == 3){
            printc('\n');
        } else{
            printc(aa);
        }
    }
//    out_byte(0x20,0xa0);
    __asm__ __volatile__("movb $0x20,%al\n"
            "out %al,$0xa0\n"
            "out %al,$0x20\n"
            "nop\n"
            "nop\n"
            "nop\n"
            "nop\n");
}
void iret_data(int state){
    printf("iret_data %d",state);

}
void setup_irq(int*irqs,int size){
    lidt.p_address = (int)&idt;
    lidt.size = (256 * 8) - 1;

    for (int i = 0; i < size; ++i) {
        add_idt(code_select,*irqs,InterruptGate,i);
        irqs++;
    }
    _load_igdt(&lidt);
    setup_8250a();
    StackTop = 0x10000;


}
void setup_syscall_idt(int syscall){
    add_idt(code_select,syscall,set_dpl(TrapGate,3),0x90);
    _load_igdt(&lidt);
}

//void init_idt(){
//    add_idt(code_select,((int)&s_div_by_zero) ,0x8e00,0x0);
//    add_idt(code_select,((int)&s_single_step) ,0x8e00,0x1);
//    add_idt(code_select,((int)&s_nmi) ,0x8e00,0x2);
//    add_idt(code_select,((int)&s_breakpoint) ,0x8e00,0x3);
//    add_idt(code_select,((int)&s_overflow) ,0x8e00,0x4);
//    add_idt(code_select,((int)&s_bounds) ,0x8e00,0x5);
//    add_idt(code_select,((int)&s_invalid_opcode) ,0x8e00,0x6);
//    add_idt(code_select,((int)&s_coprocessor_not_available) ,0x8e00,0x7);
//    add_idt(code_select,((int)&s_double_fault) ,0x8e00,0x8);
//    add_idt(code_select,((int)&s_coprocessor_segment_overrun) ,0x8e00,0x9);
//    add_idt(code_select,((int)&s_invalid_task_state_segment) ,0x8e00,0xa);
//    add_idt(code_select,((int)&s_segment_not_present) ,0x8e00,0xb);
//    add_idt(code_select,((int)&s_stack_fault) ,0x8e00,0xc);
//    add_idt(code_select,((int)&s_general_protection_fault) ,0x8e00,0xd);
//    add_idt(code_select,((int)&s_page_fault) ,0x8e00,0xe);
//    add_idt(code_select,((int)&s_reserved) ,0x8e00,0xf);
//    add_idt(code_select,((int)&s_math_fault) ,0x8e00,0x10);
//    add_idt(code_select,((int)&s_alignment_check) ,0x8e00,0x11);
//    add_idt(code_select,((int)&s_machine_check) ,0x8e00,0x12);
//    add_idt(code_select,((int)&s_simd_floating_point_exception) ,0x8e00,0x13);
//    add_idt(code_select,((int)&s_virtualization_exception) ,0x8e00,0x14);
//    add_idt(code_select,((int)&s_control_protection_exception) ,0x8e00,0x15);
//
//    add_idt(code_select,((int)&_keyboard_idt) ,0x8e00,0x21);
//
//    add_idt(code_select,((int)&s_timer_interrupt) ,0x8e00,0x20);
//    add_idt(code_select,((int)&s_timer_interrupt) ,0x8e00,0x40);
////    printf("code_select = %x",code_select);
//    add_idt_dpl(code_select,((int)&s_sys_call) ,0x8e00,0x90,3);
//    k_reenter = -1;
//    printf("idt inited\n");
//}

