//
// Created by Carl on 2018/6/5.
//

#ifndef MALLOC_ALLOC_H
#define MALLOC_ALLOC_H

#include <type.h>

#define PAGE_TAG_MAGIC 0xa68e3280

#define BOUNDARY_TAG_MAGIC 0xc68d3280

#define LOW_TAG_MAGIC 0x683280ca


typedef struct {
    unsigned int magic;			//< It's a kind of ...
    unsigned int size; 			//< Requested size.
    unsigned int real_size;		//< Actual size.
    int index;					//< Location in the page table.

//    struct boundary_tag *split_left;	//< Linked-list info for broken pages.
//    struct boundary_tag *split_right;	//< The same.

    struct boundary_tag *next;	//< Linked list info.
    struct boundary_tag *prev;	//< Linked list info.
}boundary_tag __align(1);

struct page_tag{
    u32 magic;
    u32 size;
    u32 max_free_size;
    struct page_tag* next;
    struct page_tag* prev;
};

struct low_tag{
    struct page_tag page_tag;
    u32 magic;
    u32 free_size;  //还剩余多少块
    u32 max_size;   //共有多少块 191
    u32 node_size;  //每块大小
    u32 bitmap[6];

} __align(64);




void* malloc(int size);
void free(void* p);
#endif //MALLOC_ALLOC_H
