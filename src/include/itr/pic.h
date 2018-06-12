//
// Created by Carl on 2018/6/12.
//


#ifndef NEWKERNEL_PIC_H
#define NEWKERNEL_PIC_H
#include <type.h>

#define IRQ_TIMER 0
#define IRQ_KEYBOARD 1
#define IRQ_Cascade 2
#define IRQ_COM2 3
#define IRQ_COM1 4
#define IRQ_LPT2 5
#define IRQ_Floppy_Disk 6
#define IRQ_LPT1 7
#define IRQ_CMOS 8
#define IRQ_SCSI 9
#define IRQ_SCSI1 10
#define IRQ_SCSI2 11
#define IRQ_PS2_Mouse 12
#define IRQ_FPU 13
#define IRQ_Primary_ATA 14
#define IRQ_Secondary_ATA 15

//8250 PIC
#define Master_PIC 0x20
#define Slave_PIC 0xa0
#define Master_PIC_Command  Master_PIC
#define Master_PIC_Data     Master_PIC + 1
#define Slave_PIC_Command   Slave_PIC
#define Slave_PIC_Data      Slave_PIC + 1

//PIC Command
#define PIC_INIT 0x11
#define PIC_EOI 0x20 /*中断结束命令代码*/

void setup_8250a();
void send_pic_eoi(u8 irq);


int disable_irq(u8 irq);
int enable_irq(u8 irq);
#endif //NEWKERNEL_PIC_H
