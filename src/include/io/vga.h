//
// Created by carljay on 17-8-15.
//

#ifndef NEWKERNEL_VGA_H
#define NEWKERNEL_VGA_H

#include "type.h"
typedef struct {
    u8 vc_char:8;
    u8 color:4;
    u8 b_color:4;
}vchar;
extern vchar vchars[25][80];
char x,y;
int _get_point();
void _set_point(int a);
void repoint();
void flip_up();
#endif //NEWKERNEL_VGA_H
