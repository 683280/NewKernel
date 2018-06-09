//
// Created by Carl on 2018/6/1.
//

#ifndef NEWKERNEL_PAGING_H
#define NEWKERNEL_PAGING_H

#include <type.h>
#define KB 1024
#define MB 1024 * KB
#define GB 1024 * MB
#define MAX_MEM_SIZE 4 * GB
//1048576
#define PAGING_PAGES (MAX_MEM_SIZE >> 12)

#define USED 100

#define enable_paging() \
 __asm__ volatile( \
"movl %cr0,%eax\n" \
"orl $0x80000000,%eax\n" \
"movl %eax,%cr0\n" \
);
#define disable_paging() \
 __asm__ volatile( \
"movl %cr0,%eax\n" \
"xorl $0x80000000,%eax\n" \
"movl %eax,%cr0\n" \
);

#define set_cr3(address){ \
int _offset = address; \
_offset &= 0xffffffff; \
__asm__ __volatile__("movl %%eax,%%cr3\n" \
::"a"(_offset)    \
:);}


struct {
    u32 cr3;

};

typedef u32 PTE_DESC;

//extern u32 mem_map[4][1024];
u32 kernel_cr3;

void paging_init(u32 start,u32 end);
u32 get_free_page();
void free_page(u32 page);

void set_pagedir(void* dir);
u32 *get_pagedir();
u32 get_page_address(u32 page);
int get_use_page(int page);

void clear_pagedir(u32 *dir);
#endif //NEWKERNEL_PAGING_H
