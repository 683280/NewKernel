//
// Created by carljay on 17-7-22.
//
#include <video/chr.h>
#include <sched/sched.h>
#include "mm/mm.h"
#include <mm/paging.h>
#include <asm.h>

int liballoc_lock(){
//    cli
}
int liballoc_unlock(){
//    sti
}


int alloc_free_page(void*page,int size){
    u32 *p_page = page;
    u32 tem;
    for (int i = 0; i < size; ++i) {
        *p_page = *p_page | ~1;
        tem = *p_page & 0xFFFFF000;
        free_page(tem);
    }
    return 0;
}
u32 line_to_physics(u32 line_addres){
    u32*pagedir;

    u32 dir_index;
    u32 table_index;

    u32 table_addr;
    u32 physics_address;
    pagedir = get_pagedir();

    dir_index = line_addres >> 22;
    table_index = line_addres >> 12;

    table_addr = (u32*)(*pagedir) + dir_index;
    table_addr &= 0xfffff000;

    physics_address = *((u32*)table_addr + table_index);
    physics_address &= 0xfffff000;
    return physics_address;
}
void* alloc_page(int size){
    return alloc_dir_page(get_pagedir(),size);
}
void* alloc_dir_page(u32* page_dir,int size){
    u32* tem_dir;
    u32* page_table;
    int tem = 0;
    int page_index;
    int table_index;

    for (page_index = 0; page_index < 1024; ++page_index) {
        //获取页目录
        tem_dir = *(page_dir + page_index);
        //页目录是否存在
        if(((u32)tem_dir) & 1 == 0){
            //不存在，分配一页
            *tem_dir = get_free_page() | 0x3;
        }
        tem_dir = (u32)tem_dir & 0xfffff000;
        page_table = *tem_dir;
        for (table_index = 0; table_index < 1024; ++table_index) {
            if(*page_table & 1){
                tem = 0;
            } else{
                tem++;
                if (tem == size){
                    goto end;
                }
            }
            page_table += 1;
        }
        table_index = 0;
    }
end:
    if(tem != size){
        return 0;
    }
    tem--;

    page_table -= tem;
    for (int j = 0; j < size; ++j) {
        *(page_table + j)= get_free_page() | 3;
    }
    table_index = (page_index * 1024) + table_index;
    table_index -= tem;
//    printf("table_index = %d   *page_table = %x\n",table_index,*page_table);
    return table_index << 12;
}

void add_pg_desc(int address,short type,int add_address,int n, short is_pte){
    int desc = address & 0xfffff000;
    int* ptr = (int*)add_address;
    desc = desc | type;
    *(ptr + n) = desc;
}

void set_pg_type(char avl,char g,char d,char a,char pcd,char pwt,char us,char rw,char p){

}

void* alloc(int size,PROCESS* pro){
//    pro->p_flag
    return 0x1;
}

int mm_check_addr(void *addr) {
#if defined(ARCH_X86)
    u32 page = get_page_entry(addr);

	if(!(page & 0x01)) return 0; // Page not present

	return (page & 0x02) ? (2) : (1); // Check R/W bit
#else
    // Unimplemented for this architecture
    return 0;
#endif
}