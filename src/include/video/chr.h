//
// Created by carljay on 17-7-31.
//

#ifndef NEWKERNEL_CHR_H
#define NEWKERNEL_CHR_H
#include <stdarg.h>
// 定义常用符号常数
#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define SMALL	64		/* use 'abcdef' instead of 'ABCDEF' */

#define is_digit(c)	((c) >= '0' && (c) <= '9')

#define FMT_SPEC '%'
#define ZERO_ALL_VID()  \
	do {                \
		is_in_spec = 0; \
		size       = 0; \
		width      = 0; \
		precision  = 0; \
		showsign   = 0; \
		signspace  = 0; \
		leftalign  = 0; \
		padzeros   = 0; \
	} while(0);


// 除法操作，输入：n为被除数，base为除数；结果：n为商，函数返回值为余数。
#define do_div(n,base) ({ \
int __res; \
__asm__("divl %4":"=a" (n),"=d" (__res):"0" (n),"1" (0),"r" (base)); \
__res; })

void dprintf(const char *fmt, ...);
void kprintf(const char *fmt, ...);
void printf(const char* fmt,...);
void printc(char ch);
void printd(double d);
void printh(int d);
void printi(int i);
void itoc(long n);
void cls();
void print_memy_hex(int address,int lenght);

int vsprintf(char *buf, const char *fmt, va_list args);
#endif //NEWKERNEL_CHR_H
