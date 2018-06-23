//
// Created by Carl on 2018/6/12.
//

#include <itr/pic.h>
#include <io/io.h>
#include <video/chr.h>

void setup_8250a(){

    outb(Master_PIC_Command,PIC_INIT);
    io_wait();
    outb(Slave_PIC_Command,PIC_INIT);
    io_wait();
    outb(Master_PIC_Data,0x20);
    io_wait();
    outb(Slave_PIC_Data,0x28);
    io_wait();
    outb(Master_PIC_Data,0x4);
    io_wait();
    outb(Slave_PIC_Data,0x2);
    io_wait();
    outb(Master_PIC_Data,0x1);
    io_wait();
    outb(Slave_PIC_Data,0x1);
    io_wait();
    outb(Master_PIC_Data,0xFFFF);
    io_wait();
    outb(Slave_PIC_Data,0xFFFF);
    io_wait();
    enable_irq(0);
    io_wait();
    enable_irq(1);
}

void send_pic_eoi(u8 irq){
    if(irq >= 8)
        outb(Slave_PIC_Command,PIC_EOI);
    outb(Master_PIC_Command,PIC_EOI);
}

/**
 * Disable an IRQ line
 *
 * @param irq the IRQ to be disabled
 * @return returns 0 if success
 */
int disable_irq(u8 irq) {
    if(irq > 16) return 1;
    if(irq < 8)  outb(Master_PIC_Data, inb(Master_PIC_Data) | (1 << irq));
    else         outb(Slave_PIC_Data, inb(Slave_PIC_Data) | (u8)(0x100 << irq));
    return 0;
}

/**
 * Enable an IRQ line
 *
 * @param irq the IRQ to be enabled
 * @return returns 0 if success
 */
int enable_irq(u8 irq) {
    u32 f;
    if(irq > 16) return 1;
    if(irq < 8) {
        f = inb(Master_PIC_Data) & ~(1 << irq);
        outb(Master_PIC_Data,f);
    }
    else         outb(Slave_PIC_Data, inb(Slave_PIC_Data) & ~(0x100 << irq));
    return 0;
}
