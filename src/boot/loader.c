//
// Created by 13342 on 2017/8/5.
//
#include <elf/elf.h>
#include <io/vga.h>
#include "type.h"
#include <multiboot.h>
char* laod_addres = 0x1000;
void _load_disk();
u32 _point;

void itoc(long n) {
    int len=0, p=n;
    char a = 0;
    while(p/=10)
    {
        len++;
    }
    len++;
    for(p=0;p<len;p++)
    {
        int x=1;
        for(int t=p+1;t<len;t++)
        {
            x*=10;
        }
        a = n/x + '0';
        n -=( a - '0' ) * x;
        printc(a);
    }
}
void printc(char ch) {
    unsigned char* c = (char*)0xb8000;
    unsigned int i = _get_point();
    if (ch == '\n') {
        i = (i / 80 + 1) * 80;
        _set_point(i);
        return;
    }
    int offset = i * 2;
    *(c + offset) = ch;
    *(c + offset + 1) = 0x07;
    _set_point(++i);
}
int set(){
    char* p_kernel = 0x3000;
    Elf32_Ehdr* ehdr = p_kernel;

    for (int i = 0; i < ehdr->e_phnum; ++i) {
        Elf32_Phdr* phdr = (Elf32_Phdr*)(p_kernel + ehdr->e_phoff + (i * ehdr->e_phentsize));
        if(phdr->p_type == 1){
            char* p_p = (phdr->p_offset + 0x3000);
            char* p_load = phdr->p_vaddr;
            itoc(phdr->p_offset);
            printc(' ');
            printc(' ');
            printc(' ');
            itoc(p_p);
            printc(' ');
            printc(' ');
            printc(' ');
            itoc(p_load);
            printc('\n');
            for (int j = 0; j < phdr->p_filesz; ++j) {
                *(p_load + j) = *(p_p + j);
            }
        }
    }
    return ehdr->e_entry;
}
multiboot_info_t  mbd;
void lmain(){
    _load_disk();
//    multiboot_info_t * mbd, int magic
    void (*p)();
    int a = set();
    *(int*)&p=(int)(a);

//    __asm__ __volatile__("":"a" (0x1234));
//    __asm__ __volatile__("":"b" (&mbd));
    (*p)();
    while(1){}
}