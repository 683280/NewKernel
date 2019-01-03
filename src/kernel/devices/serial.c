//
// Created by Carl on 2018/6/12.
//

#include <devices/serial.h>
#include <io/io.h>
#include <itr/pic.h>
#include <video/chr.h>


int init_serial(u16 port){
    outb(port + 1, 0x00);
    outb(port + 3, 0x80);
    outb(port + 0, 0x01);
    outb(port + 1, 0x00);
    outb(port + 3, 0x03);
    outb(port + 2, 0xC7);
    outb(port + 4, 0x0B);
    outb(port + 1, 0x01);

    enable_irq(4);
    enable_irq(3);
}


int is_transmit_empty(u16 port) {
    return inb(port + 5) & 0x20;
}

void write_serial(u16 port,char a) {
    while (is_transmit_empty(port) == 0);
    outb(port,a);
}
