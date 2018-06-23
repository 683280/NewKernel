//
// Created by Carl on 2018/6/13.
//

#ifndef NEWKERNEL_PROC_H
#define NEWKERNEL_PROC_H

#include <type.h>

size_t* alloc_cr3(size_t* start,size_t size,u32*vis_start);
size_t* alloc_vir_page(size_t * cr3,u32 size);
size_t vir_to_physics(size_t* cr3,size_t vir);
//size_t* alloc_vir_page
#endif //NEWKERNEL_PROC_H
