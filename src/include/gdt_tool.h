//
// Created by carljay on 17-8-1.
//

#ifndef NEWKERNEL_GDT_TOOL_H
#define NEWKERNEL_GDT_TOOL_H

#include "type.h"

int get_n_bit(int ,int);
int get_one_bit(int a);
int get_bit(int i,int s,int n);
void wait();
void wait_n(int n);
u32 setDpl(u32 type,u8 dpl);
#endif //NEWKERNEL_GDT_TOOL_H
