//
// Created by Carl on 2018/6/1.
//

#include "mm/paging.h"
#include <video/chr.h>
#include <sched/sched.h>

u32 kernel_cr3;
//4194304   占用4MB内存，高12位
u32 mem_map[4 * 1024];

//u32 mem_map
//mem_map 的指针
u32 mm_map_index = 0;
//当前内存最大能分多少页 每页4kb 最大内存(kb) / 4
u32 mm_max_size = 0;
//
u32 curr_max_size = 0;

//分页目录
PTE_DESC page_dir[1024]     __align(0x1000);
//页表
PTE_DESC page_tables[4][1024]  __align(0x1000);

void paging_init(u32 start,u32 end){
    mm_max_size = end / 4;

//    printf("mm_max_size = %d\n",mm_max_size);
    clear_pagedir(&page_dir);
    clear_pagedir(&page_tables[0]);
    clear_pagedir(&page_tables[1]);
    clear_pagedir(&page_tables[2]);
    clear_pagedir(&page_tables[3]);
//    for (int k = 0; k < 4; ++k) {
//
//    }
    page_dir[0] = ((int)&page_tables[0]) | 0x3;
    page_dir[1] = ((int)&page_tables[1]) | 0x3;
    page_dir[2] = ((int)&page_tables[2]) | 0x3;
    page_dir[3] = ((int)&page_tables[3]) | 0x3;

    for (int j = 0; j < 1000; ++j) {
        page_tables[0][j] = (j << 12) | 0x3;
    }
    //TODO 计算内核占用的内存 ，将内核占用的内存也全部设为 USED
    int kernel_used_paging = 0;

    for (int i = 0; i < 4096; ++i) {
        mem_map[i] = i << 12;
    }

    mm_map_index = 0x200;
    curr_max_size = 1024;
    //将其他的设为 NOT_USED
//    printf("pte_desc = 0x%x  pte_desc[0] = 0x%x\n",&page_dir,page_dir[0]);
//    printf("page_tables = 0x%x  page_tables[0] = 0x%x\n",&page_tables,page_tables[0]);

    kernel_cr3 = &page_dir;
//    printf("kernel_cr3 = %x\n",kernel_cr3);
    set_cr3(kernel_cr3);
    enable_paging();
//    printf("paging end\n");
}

u32 get_free_page(){
    u32 i = mem_map[mm_map_index];
    mm_map_index++;
    return i;
}

void free_page(u32 page){
    mm_map_index--;
    mem_map[mm_map_index] = page;
}
int inline get_use_page(int page){
    return page & 1;
}
u32 inline get_page_address(u32 page){
    return page & 0xFFFFF000;
}
void set_pagedir(void* dir){
    if(!dir) {
        printf("Attempted to set pagedir pointer to NULL!");
        return;
    }
    __asm__ __volatile__("mov %0, %%cr3":: "b"(dir));
}
u32 *get_pagedir() {
    u32 *dir;
    __asm__ __volatile__("mov %%cr3, %0": "=a"(dir));
    return dir;
}
void clear_pagedir(u32 *dir){
    int i = 0;
    for(i = 0; i < 1024; i++)
    {
        //	kerror(ERR_INFO, "      -> PDIRENT %X", i);
        dir[i] = 2; // supervisor, rw, not present.
    }
}