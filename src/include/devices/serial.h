//
// Created by Carl on 2018/6/12.
//


#ifndef NEWKERNEL_SERIAL_H
#define NEWKERNEL_SERIAL_H
#include <type.h>

#define SERIAL_COM1 0x3F8
#define SERIAL_COM2 0x2F8
#define SERIAL_COM3 0x3E8
#define SERIAL_COM4 0x2E8
int init_serial(u16 port);

void write_serial(u16 port,char a);
#endif //NEWKERNEL_SERIAL_H
