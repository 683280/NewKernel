//
// Created by Carl on 2018/6/13.
//

#include <sched/proc.h>
#include <mm/paging.h>
#include <video/chr.h>


size_t* alloc_cr3(size_t* start,size_t size,u32*vis_start){
    u32* cr3;
    u32* tem_table;
    u32  tem_start;
    u32 page_size;
    u32 dir_size;
    cr3 = get_user_page_dir();
    //多个页
    page_size = (size / 1024) + ((size % 1024) ? 1 : 0);
    //多少个目录
    dir_size = (page_size / 1024) + ((page_size % 1024) ? 1 : 0);
//    dprintf("alloc_cr3 = 0x%08x    page_size = %d   dir_size = %d\n",cr3,page_size,dir_size);

    u32 tem_page_size = 1024;

    //去掉0-12位
    tem_start = ((u32)start) & 0xfffff000;
    for (int i = 0; i < dir_size; ++i) {
        tem_table = get_free_page();
        clear_pagedir(tem_table);
        *(cr3 + 4 + i) = (u32)tem_table | 0x7;
//        dprintf("tem_table = 0x%08x\n",tem_table);
        if (i + 1 == dir_size)tem_page_size = page_size % 1024;
//        dprintf("page_size = %d\n",tem_page_size);
        for (int j = 0; j < tem_page_size; ++j) {
            *(tem_table + j) = tem_start | 0x7;
//            dprintf(" *(tem_table + j)  = 0x%08x\n", *(tem_table + j) );
            tem_start += 0x1000;
        }
    }
    *vis_start = ((u32)start & 0xfff) | (4 << 22);
    return cr3;
}

size_t* alloc_vir_page(size_t * cr3,u32 size){
    u32* tem_dir;
    u32* tem_page;
    u32* page_table;
    u32 p_i,t_i;
    int tem = 0;
    int page_index;
    int table_index;

    for (page_index = 4; page_index < 1024; ++page_index) {
        //获取页目录
        tem_dir = cr3 + page_index;
        //页目录是否存在
        if((*tem_dir) & 1 == 0){
            //不存在，分配一页
//            dprintf("get_free_page   ----\n");
            u32 tem = get_free_page();
            clear_pagedir(tem);
            *tem_dir = tem | 0x7;
        }
        tem_page = *tem_dir & 0xfffff000;
//        dprintf("page_index = %d   tem_page = 0x%08x   *tem_page = 0x%08x \n",page_index,tem_page,*tem_page);
        for (table_index = 0; table_index < 1024; ++table_index) {
            if(*tem_page & 1){
                tem = 0;
            } else{
                if(!tem){
                    p_i = page_index;
                    t_i = table_index;
                }
                tem++;
                if (tem == size){
                    goto end;
                }
            }
            tem_page += 1;
        }
    }
    end:
    if(tem != size){
        return 0;
    }
//    dprintf("p_i = %d   t_i = %d\n",p_i,t_i);
    //开始页目录
    tem_dir = cr3 + p_i;
    //开始页表项
    tem_page = *tem_dir;
    tem_page = (u32)tem_page & 0xfffff000;

//    tem_page += t_i;
    u32 address =  p_i << 22 | t_i << 12;
    u32* offset;
//    dprintf("tem_page 0x%08x \n",tem_page);
    for (int k = 0; k < size; ++k) {
        offset = tem_page + t_i;
//        dprintf("offset = 0x%08x   *offset = 0x%08x\n",offset,*offset);
        *offset = get_free_page() | 0x7;
//        dprintf("offset = 0x%08x   *offset = 0x%08x\n",offset,*offset);
        t_i++;
        if(t_i == 1024){
            tem_page = *(tem_dir++);
            tem_page = (u32)tem_page & 0xfffff000;
            t_i = 0;
        }
    }
    return address;
}

size_t vir_to_physics(size_t* cr3,size_t vir){
    u32 dir_index = vir >> 22;
    u32 page_index = (vir >> 12) & 0x3ff;
    u32* page_tables = *(cr3 + dir_index);
//    dprintf("dir_index = %d   page_index = %d  page_tables = 0x%08x   \n",dir_index,page_index,page_tables);
    page_tables = (u32)page_tables & 0xfffff000;
    u32 physics = *(page_tables + page_index);
    physics &=  0xfffff000;
    physics |= vir & 0xfff;
    return physics;
}