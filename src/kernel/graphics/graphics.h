//
// Created by Carl on 2018/6/16.
//

#ifndef NEWKERNEL_GRAPHICS_H
#define NEWKERNEL_GRAPHICS_H
typedef struct _rect{
    int x,y,w,h;
}Rect;
typedef struct _window{
    struct _rect rect;
    int index;
}Window;

int alloc_window(int x,int y,int w,int h,struct _window*);
int window_draw_rect(struct _window *window,int color,int x,int y,int w,int h);
#endif //NEWKERNEL_GRAPHICS_H
