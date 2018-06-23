//
// Created by Carl on 2018/6/16.
//

#include <devices/graphics/vga.h>
#include <video/chr.h>
#include "graphics.h"

static void calc_rect(struct _window *window,int x,int y,int w,int h,Rect* rect){
    int _x,_y,_w,_h,_r,_b;
    Rect *rect1 = &window->rect;


    if(x > rect1->w)x = rect1->w;
    if(y > rect1->h)y = rect1->h;

    _x = rect1->x + x;
    _y = rect1->y + y;


    _w = x + w;
    _h = y + h;
    _r = rect1->x + rect1->w;
    _b = rect1->y + rect1->h;
    if(_w > _r)w = _r - rect1->x;
    if(_h > _b)h = _b - rect1->y;

    rect->x = _x;
    rect->y = _y;
    rect->w = w;
    rect->h = h;
}

int alloc_window(int x,int y,int w,int h,struct _window* window){
    if(!window)return -1;
    if(x < 0)x = 0;
    if(y < 0)y = 0;
    int _w,_h;
    get_screen_size(&_w,&_h);

    if(w > _w)w = _w;
    if(h > _h)h = _h;

    window->rect.x = x;
    window->rect.y = y;
    window->rect.w = w;
    window->rect.h = h;

}

int window_draw_rect(struct _window *window,int color,int x,int y,int w,int h){
    Rect rect;
    calc_rect(window,x,y,w,h,&rect);
    dprintf("window_draw_rect x = %d   y = %d   w = %d   h = %d",rect.x,rect.y,rect.w,rect.h);
    vga_draw_rect(color,rect.x,rect.y,rect.w,rect.h);
    return 0;
}