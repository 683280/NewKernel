//
// Created by Carl on 2018/5/31.
//

#include <alloc.h>
#include <stdlib.h>
#include "string.h"


uint32_t strlen(uint8_t *str)
{
    register int __res ;
    __asm__("cld\n\t"
            "repne\n\t"
            "scasb\n\t"
            "notl %0\n\t"
            "decl %0"
    :"=c" (__res):"D" (str),"a" (0),"0" (0xffffffff));
    return __res;
}

uint8_t *strdup(uint8_t *str)
{
    uint8_t *tmp = malloc(strlen(str) + 1);
    uint8_t *ret = tmp;
    while( *str && (*tmp++ = *str++));
    return *tmp = '\0', ret;
}

uint32_t strcmp(uint8_t *str1, uint8_t *str2)
{
    for(; *str1 && *str2 && *str1 == *str2; ++str1, ++str2);
    return *str1 - *str2;
}

uint8_t *strcat(uint8_t *str1, uint8_t *str2)
{
    uint8_t *ret = malloc(strlen(str1) + strlen(str2) + 1);
    uint8_t *_ret = ret;
    uint32_t i = 0;
    uint32_t str1_len = strlen(str1);
    uint32_t str2_len = strlen(str2);
    while(i++ < str1_len)
        *ret++ = *str1++;
    i = 0;
    while(i++ < str2_len)
        *ret++ = *str2++;
    *ret = '\0';
    return _ret;
}

//void *memset(void *addr, uint8_t val, uint32_t size)
//{
//    uint8_t *_addr = (uint8_t*)addr;
//    while(size--) *_addr++ = val;
//    return (void*)addr;
//}

uint8_t *itoa(uint32_t val)
{
    uint8_t *buf = malloc(12);
    buf[11] = '\0';
    uint32_t i = 11;
    if(!val) return (buf[10] = '0'), &buf[10];
    while(val)
    {
        buf[--i] = '0' + val%10;
        val /= 10;
    }
    return (uint8_t*)&buf[i];
}

uint8_t *strndup(uint8_t *src, uint32_t len)
{
    uint8_t *ret = malloc(len + 1);
    uint32_t i;
    for(i = 0; i < len; ++i)
        *(ret + i) = *(src++);
    ret[i] = '\0';
    return ret;
}

uint32_t isdigit(uint8_t chr)
{
    return chr >= '0' && chr <= '9';
}

uint32_t isalpha(uint8_t chr)
{
    return (chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z');
}

uint32_t within(uint8_t chr, uint8_t *str)
{
    while(*str)
        if(chr == *str++)
            return 1;
    return 0;
}