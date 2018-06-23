#include <type.h>
#include <print.h>
#include <stdio.h>
extern char x;
extern char y;

char printbuf[512];

void printf(const char *fmt, ...) {
    va_list args;
    int i;
    va_start(args, fmt);
    i = vsprintf(printbuf, fmt, args);
    char* str = printbuf;
	fwrite(1,str,i,1);
    va_end(args);
}
// 将字符数字转换成整数。输入是数字串指针的指针，返回是结果的数值，另外指针将前移。
static int skip_atoi(const char **s)
{
	int i=0;

	while (is_digit(**s))
		i = i*10 + *((*s)++) - '0';     // 这里导致指针前移
	return i;
}
// 格式化输出
int vsprintf(char *buf, const char *fmt, va_list args) {
	int len;
	int i;
	char * str;     // 用于存放转换过程中的字符串
	char *s;
	int *ip;

	int flags;		/* flags to number() */

	int field_width;	/* width of output field */
	int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
	int qualifier;		/* 'h', 'l', or 'L' for integer fields */

	// 扫描格式字符串，对于不是 % 的就依次存入str
	for (str=buf ; *fmt ; ++fmt) {
		if (*fmt != '%') {
			*str++ = *fmt;
			continue;
		}
		// 取得格式指示字符串中的标志域，并将标志常量放入flags变量中
		/* process flags */
		flags = 0;
		repeat:
		++fmt;		/* this also skips first '%' */
		switch (*fmt) {
			case '-': flags |= LEFT; goto repeat;
			case '+': flags |= PLUS; goto repeat;
			case ' ': flags |= SPACE; goto repeat;
			case '#': flags |= SPECIAL; goto repeat;
			case '0': flags |= ZEROPAD; goto repeat;
		}
		// 取当前参数数字段宽度域值，放入field_width变量中，如果宽度域中是数值则直接取其为宽度值。
		// 如果宽度域中是字符'*'，表示下一个参数指定宽度。因此调用va_arg取宽度值。若此时宽度值
		// 小于0，则该负数表示其带有标志域'-'标志（左靠齐），因此还需要在标志变量中填入该标志，并
		// 将字段宽度值取为其绝对值。
		/* get field width */
		field_width = -1;
		if (is_digit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*') {
			/* it's the next argument */
			field_width = va_arg(args, int);        // 这里有个bug，应插入++fmt。// TODO: 不懂
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		// 取格式转换串的精度域，并放入precision变量中。精度域开始的标志是'.'.
		// 其处理过程与上面宽度域的类似。如果精度域中是数值则直接取其为精度值。如果精度域中是
		// 字符'*',表示下一个参数指定精度。因此调用va_arg取精度值。若此时宽度值小于0，则将
		// 字段精度值取为0.
		/* get the precision */
		precision = -1;
		if (*fmt == '.') {
			++fmt;
			if (is_digit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {
				/* it's the next argument */
				precision = va_arg(args, int);
			}
			if (precision < 0)
				precision = 0;
		}

		/* get the conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
			qualifier = *fmt;
			++fmt;
		}

		// 分析转换指示符
		switch (*fmt) {
			// ‘c’ 表示对应参数应是字符。此时如果标志域表明不是左靠齐，则该字段前面
			// 放入'宽度域值-1'个空格字符，然后再放入参数字符。如果宽度域大于0，
			// 则表示为左靠齐，则在参数字符后面添加'宽度值-1'个空格字符
			case 'c':
				if (!(flags & LEFT))
					while (--field_width > 0)
						*str++ = ' ';
				*str++ = (unsigned char) va_arg(args, int);
				while (--field_width > 0)
					*str++ = ' ';
				break;

				// 's'表示对应参数是字符串。首先取参数字符串的长度，若其超过了精度域值，
				// 则扩展精度域=字符串长度。此时如果标志域表明不是左靠齐，则该字段前放入
				// '宽度值-字符串长度'个空格字符。然后再放入参数字符串。如果宽度域还大于0，
				// 则表示为左靠齐，则在参数字符串后面添加'宽度值-字符串长度'个空格字符。
			case 's':
				s = va_arg(args, char *);
				len = strlen(s);
				if (precision < 0)
					precision = len;
				else if (len > precision)
					len = precision;

				if (!(flags & LEFT))
					while (len < field_width--)
						*str++ = ' ';
				for (i = 0; i < len; ++i)
					*str++ = *s++;
				while (len < field_width--)
					*str++ = ' ';
				break;

				// 'o'表示8进制，通过number函数处理
			case 'o':
				str = number(str, va_arg(args, unsigned long), 8,
							 field_width, precision, flags);
				break;

				// 'p'表示一个指针类型，此时若该参数没有设置宽度域，默认宽度域为8
				// 并且需要添零，然后用number函数处理
			case 'p':
				if (field_width == -1) {
					field_width = 8;
					flags |= ZEROPAD;
				}
				str = number(str,
							 (unsigned long) va_arg(args, void *), 16,
							 field_width, precision, flags);
				break;

				// 'x'-转成16进制
			case 'x':
				flags |= SMALL;
			case 'X':
				str = number(str, va_arg(args, unsigned long), 16,
							 field_width, precision, flags);
				break;

				// 'd' & 'i'  表示带符号整数；'u' 表示无符号整数
			case 'd':
			case 'i':
				flags |= SIGN;
			case 'u':
				str = number(str, va_arg(args, unsigned long), 10,
							 field_width, precision, flags);
				break;

				// 'n'-表示要把到目前为止转换输出字符数保存到对应参数指针指定的位置中。
				// 首先利用va_arg()取得该参数指针，然后将已经转换好的字符数存到指向的位置
			case 'n':
				ip = va_arg(args, int *);
				*ip = (str - buf);
				break;

				// 若格式转换不是 % ，则表示格式字符串有错，直接将一个“%”写入输出串中。
				// 如果格式转换符的位置还有字符，则也直接将该字符写入输入串中，并返回继续处理
				// 格式字符串，否则表示已经处理到格式字符串的结尾处，退出循环。
			default:
				if (*fmt != '%')
					*str++ = '%';
				if (*fmt)
					*str++ = *fmt;
				else
					--fmt;
				break;
		}
	}
	*str = '\0';        // 字符串结尾字符：'\0'
	return str-buf;     // 返回转换好的长度值
}