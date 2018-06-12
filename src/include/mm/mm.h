//
// Created by carljay on 17-7-31.
//

#ifndef NEWKERNEL_MM_H
#define NEWKERNEL_MM_H

#include "type.h"
#include <sched/sched.h>

void init_mm(u32 start,u32 end);

extern void memcpy(void* p_dst,void* p_src,int size);
void memset(void* p_dst,int src,int size);
void memsetw(void* p_dst,int src,int size);

int liballoc_lock();
int liballoc_unlock();
void* liballoc_alloc(int);

void* alloc_page(int size);
int alloc_free_page(void*page,int size);

void add_pg_desc(int address,short type,int add_address,int n, short is_pte);
void set_pg_type(char avl,char g,char d,char a,char pcd,char pwt,char us,char rw,char p);
void* alloc(int size,PROCESS* pro);

u32 line_to_physics(u32 line_addres);

int mm_check_addr(void *addr);
#endif //NEWKERNEL_MM_H
