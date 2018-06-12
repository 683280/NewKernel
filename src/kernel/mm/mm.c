//
// Created by carljay on 17-7-22.
//
#include <video/chr.h>
#include <sched/sched.h>
#include "mm/mm.h"
#include <mm/paging.h>
#include <asm.h>

void init_mm(u32 start,u32 end_mm){


}
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
    u32* page_dir;
    u32* page_table;
    int tem = 0;
    int page_index;
    int table_index;

    page_dir = get_pagedir();

//    for (int i = 0; i < 1024; ++i) {
//
//    }

    for (page_index = 0; page_index < 4; ++page_index) {

        page_table = *(page_dir + page_index) & 0xfffff000;

        for (table_index = 0; table_index < 1024; ++table_index) {
            if(*page_table & 1){
                tem = 0;
            } else{
//            printf("*page_table = 0x%08x    table_index = %d   tem = %d   size = %d\n",*page_table,table_index,tem,size);
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
//        printf("no memory");
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