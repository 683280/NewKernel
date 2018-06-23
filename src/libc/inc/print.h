/** \file print.h
 *  \brief Contains functions dealing with printing to the VGA screen.
 *
 *  Contains functions that help print to and manage the VGA screen.
 */

#ifndef VGA_PRINT_H
#define VGA_PRINT_H

#include <type.h>

// 定义常用符号常数
#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define SMALL	64		/* use 'abcdef' instead of 'ABCDEF' */

#define is_digit(c)	((c) >= '0' && (c) <= '9')
void printf(const char *fmt, ...);
int vsprintf(char *buf, const char *fmt, va_list args);

#define is_ansi(x) ((x == 'A') || (x == 'B') || (x == 'C') || (x == 'D') || (x == 's') || (x == 'u') || (x == 'H') || (x == 'J') || (x == 'K') || (x == 'm'))

#endif