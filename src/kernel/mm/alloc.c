//
// Created by Carl on 2018/6/5.
//

#include <alloc.h>
#include <type.h>

#define tag_size sizeof(boundary_tag)

u32 page_mem_size = 4096;//每页大小
u32 min_page_size = 16;//每次最小申请数量

struct alloc_tag{
    struct page_tag* page_tag;
    struct page_tag* last_page_tag;
};

struct alloc_tag big_mm = {NULL, NULL};
struct alloc_tag low_mm = {NULL, NULL};


struct page_tag* new_page_tag(struct alloc_tag*tag,u32 size){
    u32 _size;
    u32 page_size;
    struct page_tag* _p_tag;

    if (size)
        page_size = (size / page_mem_size) + (size % page_mem_size ? 1 : 0);
    else
        page_size = 8;

    _size = page_size < min_page_size ? min_page_size : page_size;

    _p_tag = alloc_page(_size);
    if(!_p_tag)
        return NULL;
    _p_tag->size = _size * page_mem_size;
    _p_tag->next = NULL;
    _p_tag->prev = NULL;
    _p_tag->magic = PAGE_TAG_MAGIC;
    _p_tag->max_free_size = _p_tag->size - size - sizeof(struct page_tag);

    if(tag->last_page_tag){
        tag->last_page_tag->next = _p_tag;
    } else{
        tag->page_tag = _p_tag;
    }
    tag->last_page_tag = _p_tag;
    return _p_tag;
}
struct low_tag* new_low_tag(){
    struct low_tag* tag;
    u32 *tem;
    u32 free_size;
    tag = new_page_tag(&low_mm,4096);

    tag->magic = LOW_TAG_MAGIC;
    //共有多少页
    tag->max_size = tag->page_tag.size / 64;
    //还剩多少页没用，总页数减去被占用的页数
    free_size = ((tag->max_size / 64) - 3 ) ;
    free_size = free_size > 0 ? free_size : 0;
    free_size *= 4;
    free_size = free_size / 64 + (free_size % 64 ? 1 : 0);
    tag->free_size = tag->max_size - free_size - 1;

    u32 t = tag->max_size / 64;
    tem = tag->bitmap;
    for (int i = 0; i < t; ++i) {
        (*tem++) = 0;
        (*tem++) = 0;
    }
    tag->bitmap[0] = 3;

    return tag;
}
void* get_low_mem(){
    struct low_tag* tag;
    if(!low_mm.page_tag){
        tag = new_low_tag();
    }

    if(tag->free_size){
    }

    u32 tem = tag->max_size / 64 * 2;
    u32 off;
    int j;
    u32 n;
    int i;
    for (i = 0; i < tem; ++i) {
        off = tag->bitmap[i];
        for (j = 0; j < 32; ++j) {
            n = off & (1 << j);
            if(!n){
                tag->bitmap[i] |= (1<<j);
                goto end;
            }
        }
    }

    end:
    off = ((i * 32) + j) * 64;
    tem = ((void*)tag) + off;
    return tem;
}
void free_low_mem(u32 p){
    struct low_tag* low_tag;
    u32 offset;
    u32 tem = p & 0xffffff00;
    for (int i = 0; i < 16; ++i) {
        low_tag = tem;
        if (low_tag->page_tag.magic == PAGE_TAG_MAGIC)goto free;
        tem -= 0x1000;
    }
    return;
    free:
    if (low_tag->magic != LOW_TAG_MAGIC)return;
    offset = p - ((u32)low_tag);
    //第几块
    offset = offset / 64;
    //在哪个int中
    u32 index = offset / 32;
    offset = offset % 32;
    low_tag->bitmap[index] &= ~(1 << offset);
    return;
}

struct page_tag* new_boundary_tag(u32 size){
    boundary_tag* _tag;
    struct page_tag* page_tag;
    page_tag = new_page_tag(&big_mm,size);
    if (page_tag->magic != PAGE_TAG_MAGIC)return NULL;

    page_tag->max_free_size = page_tag->size - (tag_size + sizeof(struct page_tag));
    _tag = page_tag + 1;

    _tag->magic = BOUNDARY_TAG_MAGIC;
    _tag->next = NULL;
    _tag->prev = NULL;
    _tag->size = tag_size;
    _tag->real_size = page_tag->max_free_size;

    return page_tag;

}

boundary_tag* get_boundary_tag(u32 size){
    struct page_tag* _p_tag;

    _p_tag = big_mm.page_tag;

    while (_p_tag){
        if(size <= _p_tag->max_free_size){
            return _p_tag;
        }
        _p_tag = _p_tag->next;
    }
    return new_boundary_tag(size);
}

void* get_high_mem(u32 size){
    struct page_tag* page_tag;
    boundary_tag* _tag,*tem;
    u32 _size;
    u32 read_size;
    _size = size + tag_size;


    page_tag = get_boundary_tag(_size);
    if (!page_tag) return NULL;
    _tag = page_tag + 1;
    while (_tag){
        read_size = _tag->real_size - _tag->size;
        if (read_size >= _size){
            tem = ((void*)_tag) + _tag->size;
            tem->real_size = read_size;
            tem->size = _size;
            tem->magic = BOUNDARY_TAG_MAGIC;
            tem->prev = _tag;
            tem->next = _tag->next;

            _tag->real_size = _tag->size;
            _tag->next = tem;
            page_tag->max_free_size -= _size;
            return tem + 1;
        }
        _tag = _tag->next;
    }
    return NULL;
}
void free_big_mem(boundary_tag* tag){
    boundary_tag* pre,*next;
    pre = tag->prev;
    next = tag->next;

    pre->next = next;
    pre->real_size += tag->real_size;
    if (next){
        next->prev = pre;
    }

}
void* malloc(int size){
    if (!size)return NULL;
    if(size <= 64){
        return get_low_mem();
    }

    return get_high_mem(size);
}
void free(void* p){
    boundary_tag* _boundary_tag;
    if (!p)return;

    _boundary_tag = p - sizeof(boundary_tag);
    if (_boundary_tag->magic == BOUNDARY_TAG_MAGIC){
        //TODO 释放boundary_tag
        free_big_mem(_boundary_tag);
    } else{
        //TODO 释放low_tag
        free_low_mem(p);
    }
}