//
// Created by carljay on 17-8-1.
//

#include "gdt_tool.h"


int get_one_bit(int a){
    return get_n_bit(a,1);
}
int get_n_bit(int a,int n){
    return get_bit(a,0,n);
}
int get_bit(int i,int s,int n){
    int tmp = 1;
    n--;
    for (int a = 0; a < n; ++a) {
        tmp = tmp << 1;
        tmp = tmp | 1;
    }
    tmp = tmp << s;
    return (i & tmp ) >> s;
}
void wait(){

}
void wait_n(int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < 100; ++j) {
            __asm__ volatile("nop");
        }
    }
}
u32 setDpl(u32 type,u8 dpl){
    return type | (dpl << 5);
}
