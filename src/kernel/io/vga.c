//
// Created by carljay on 17-8-15.
//

#include <mm.h>
#include "vga.h"
int _get_point();
void _set_point(int a);
void repoint() {
    if(x == 80){
        x = 0;
        y++;
    }
    if(y == 25){
        y = 24;
        x = 0;
        flip_up();
    }
    _set_point((y * 80) + x);
}

void flip_up() {
    memcpy(&vchars,&vchars[1][0],80 * 24 * 2);
    memsetw(&vchars[24][0],0,80);
}
