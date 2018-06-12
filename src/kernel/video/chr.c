//
// Created by carljay on 17-7-31.
//

#include <mm/mm.h>
#include <io/vga.h>
#include <video/print.h>
#include <devices/serial.h>
#include "video/chr.h"
#include "mm/string.h"

char hex[] = {'0','1','2','3','4','5','6','7','8','9',
              'A','B','C','D','E','F',};
static char printbuf[1024];
static int print(char *out, const char *format, va_list varg);

void dprintf(const char *fmt, ...){
    va_list args;
    int i;

    va_start(args, fmt);
    i = vsprintf(printbuf, fmt, args);
    char* str = printbuf;
    while(*str) write_serial(SERIAL_COM1,*str++);
    va_end(args);
}
void kprintf(const char *fmt, ...) {

    va_list args;
    int i;

    va_start(args, fmt);
    i = vsprintf(printbuf, fmt, args);
    char* str = printbuf;
    while(*str) vga_put(*str++);
    repoint();
    va_end(args);
}
void printf(const char *fmt, ...) {

    va_list args;
    int i;

    va_start(args, fmt);
    i = vsprintf(printbuf, fmt, args);
    vchar* v;
    for (int j = 0; j < i; ++j) {
        if (printbuf[j] == '\n'){
            y++;
            x = 0;
            repoint();
            continue;
        }
        v = &vchars[y][x];
        v->vc_char = printbuf[j];
        v->color = 0x7;
        x++;

    }
    repoint();
    va_end(args);
}

void printc(char ch) {
    if (ch == '\n') {
        y++;
        x = 0;
        repoint();
        return;
    }
    vchar* v = &vchars[y][x];
    v->vc_char = ch;
    v->color = 0x7;
    x++;
    repoint();
}

void printd(double d) {
    itoc(d);
}

void printi(int i) {
    itoc(i);
}

void itoc(long n) {
    int len=0, p=n;
    char a = 0;
    while(p/=10)
    {
        len++;
    }
    len++;
    for(p=0;p<len;p++)
    {
        int x=1;
        for(int t=p+1;t<len;t++)
        {
            x*=10;
        }
        a = n/x + '0';
        n -=( a - '0' ) * x;
        printc(a);
    }
}

void printh(int i) {
    char* cc = "11111111";
    int tmp;
    int x = 15;

    for(int a = 0;a < 8;a++){
        tmp = i & x;
        *(cc + 7 - a) = hex[tmp];
        i = i >> 4;
    }
    printf(cc);
}
void print_memy_hex(int address,int lenght){
    int* ptr = (int*)address;
    for (int i = 0; i < lenght; ++i) {
        int a = *(ptr + (i));
        printh(a);
        printf("\n");
    }
}
void cls(){
    memsetw((short*)vchars,0x7<<8,25*80);
    _set_point(0);
}

// 将字符数字转换成整数。输入是数字串指针的指针，返回是结果的数值，另外指针将前移。
static int skip_atoi(const char **s)
{
    int i=0;

    while (is_digit(**s))
        i = i*10 + *((*s)++) - '0';     // 这里导致指针前移
    return i;
}
// 将整数转换为指定进制的字符串。
// 输入：num-整数；base-进制；size-字符串长度；precision-数字长度(精度)；type-类型选项。
// 输出：str字符串指针
static char * number(char * str, int num, int base, int size, int precision, int type)
{
    char c,sign,tmp[36];
    const char *digits="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i;

    // 根据类型定义字母集，默认是大写字母
    if (type&SMALL) digits="0123456789abcdefghijklmnopqrstuvwxyz";
    // 如果类型指出要左调整(靠左边界)，则屏蔽类型中的填零标志。
    // TODO: 这句没看懂？？ 为什么要把最低位置为0
    if (type&LEFT) type &= ~ZEROPAD;
    // 本程序只能处理的进制范围：2-36
    if (base<2 || base>36)
        return 0;
    // TODO: 这句跟LEFT相关？？
    c = (type & ZEROPAD) ? '0' : ' ' ;
    if (type&SIGN && num<0) {
        sign='-';
        num = -num;
    } else
        sign=(type&PLUS) ? '+' : ((type&SPACE) ? ' ' : 0);
    // 如果是带符号的，字符串宽度就减一
    if (sign) size--;
    if (type&SPECIAL) {
        if (base==16) size -= 2;    // 16进制需要减2，用于前面的0x
        else if (base==8) size--;   // 8进制的减1，因为前面的0
    }
    i=0;
    if (num==0)
        tmp[i++]='0';
    else while (num!=0)
            tmp[i++]=digits[do_div(num,base)];
    if (i>precision) precision=i;       // 若字符个数大于精度值，精度值扩展为字符个数
    size -= precision;                  // 宽度再减去用于存放数值字符的个数
    // 将转换结果放在str中，如果类型中没有填零(ZEROPAD)和左靠齐标志，
    // 则在str中首先填放剩余宽度值指出的空格数。若需要带符号位，则存入符号
    if (!(type&(ZEROPAD+LEFT)))
        while(size-->0)
            *str++ = ' ';
    if (sign)
        *str++ = sign;
    // 如果是特殊转换的处理，8进制和16进制分别填入0/0x/0X
    if (type&SPECIAL) {
        if (base==8)
            *str++ = '0';
        else if (base==16) {
            *str++ = '0';
            *str++ = digits[33];    // 'x' or 'X'
        }
    }
    // 如果类型么有左靠齐标志，则在剩余的宽度中存放c字符（‘0’或者空格）
    if (!(type&LEFT))
        while(size-->0)
            *str++ = c;
    // 若i存有数值num的数字个数，若数字个数小于精度值，则str中放入 精度值-i 个'0'
    while(i<precision--)
        *str++ = '0';
    // 将数值转换好的数字字符填入str中，共i个
    while(i-->0)
        *str++ = tmp[i];
    // 若宽度值仍大于零，则表达类型标志中有左靠齐标志，则在剩余宽度中放入空格
    while(size-->0)
        *str++ = ' ';
    return str;
}
static int get_dec(const char *str, u32 *out)
{
    int n = 0;
    while(*str >= '0' && *str <= '9')
    {
        n *= 10;
        n += (int)(*str - '0');
        str++;
    }
    *out = (u32)str;

    return n;
}
static int print_int(u32 num, u8 base, u8 u, u8 pad, u8 padzero, u8 possign, u8 posspace, u8 _case, char *out)
{
    int onum = (int)num;
    if(onum < 0) if(!u) num = (~num) + 1;
    char *nums;
    if(_case) nums = "0123456789ABCDEF";
    else      nums = "0123456789abcdef";

    char ans[16] = { '0', };
    int i = 0;
    while(num)
    {
        ans[i++] = nums[num % base];
        num /= base;
    }
    if(i == 0) i++;

    if(!u)
    {
        if(onum >= 0)
        {
            if(possign)
            {
                *out++ = '+';
            }
            else if(posspace)
                *out++ = ' ';
        }
        else if(onum < 0)
            *out++ = '-';
    }

    int p = pad - i;
    if(p > 0)
    {
        while(p--) *out++ = (padzero ? '0' : ' ');
        while(--i >= 0) *out++ = ans[i];
        return (int)((pad > strlen(ans)) ? pad : strlen(ans)) + ((((possign || posspace) && !u) && onum >= 0) || ((onum < 0) && !u)) + 1;
    }
    else
    {
        while(--i >= 0) *out++ = ans[i];
        return (int)(strlen(ans) + ((((possign || posspace) && !u) && onum >= 0) || ((onum < 0) && !u)));
    }
}

static int print(char *out, const char *format, va_list varg)
{
    u8 is_in_spec = 0;
    s8 size = 0;      // Size of the integer
    u8 width = 0;     // Width of the number at minimum
    u8 precision = 0; // Precision
    u8 showsign = 0;  // Show the sign on positive numbers
    u8 signspace = 0; // Place a space before positive numbers
    u8 leftalign = 0; // Align to the left
    u8 padzeros = 0;  // Use zeros instead of spaces for padding

    int nchars = 0;    // Number of chars printed so far

    u32 temp;

    for(; *format != 0; format++)
    {
        if(!is_in_spec)
        {
            if(*format == FMT_SPEC)
            {
                is_in_spec = 1;
                continue;
            }
            *out++ = *format;
            nchars++;
            continue;
        }

        switch(*format)
        {
            case FMT_SPEC: is_in_spec = 0;
                *out++ = FMT_SPEC;
                nchars++;
                break;

            case 'l': if(size < 2) size++;
                break;

            case 'L': size = 1;
                break;

            case 'h': if(size > -2) size--;
                break;

            case 'z':
            case 'j':
            case 't': size = 0;
                break;

            case '+': showsign = 1;
                break;

            case ' ': signspace = 1;
                break;

            case '-': leftalign = 1;
                break;

            case '0': padzeros = 1;
                break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': width = (u8)get_dec(format, &temp);
                format = (char *)(temp - 1);
                break;

            case '.': format++;
                precision = (u8)get_dec(format, &temp);
                format = (char *)(temp - 1);
                break;


                // Numbers, strings, etc...

            case 'd':
            case 'i': temp = (u32)va_arg(varg, int);
                if(size == -1) temp &= 0xFFFF;
                if(size == -2) temp &= 0xFF;
                if(size == 2){} // TODO: Handle this!
                temp = (u32)print_int(temp, 10, 0, width, padzeros, showsign, signspace, 0, out);
                nchars += temp;
                out += temp;
                ZERO_ALL_VID();
                break;

            case 'u': temp = (u32)va_arg(varg, u32);
                if(size == -1) temp &= 0xFFFF;
                if(size == -2) temp &= 0xFF;
                if(size == 2){} // TODO: Handle this!
                temp = (u32)print_int(temp, 10, 1, width, padzeros, showsign, signspace, 0, out);
                nchars += temp;
                out += temp;
                ZERO_ALL_VID();
                break;

            case 'f':
            case 'F':
            case 'e':
            case 'E':
            case 'g':
            case 'G': (void)va_arg(varg, double);
                ZERO_ALL_VID();
                break;

            case 'x':
            case 'X': temp = (u32)va_arg(varg, u32);
                if(size == -1) temp &= 0xFFFF;
                if(size == -2) temp &= 0xFF;
                if(size == 2){} // TODO: Handle this!
                temp = (u32)print_int(temp, 16, 1, width, padzeros, showsign, signspace, (*format == 'X'), out);
                nchars += temp;
                out += temp;
                ZERO_ALL_VID();
                break;

            case 'o': temp = (u32)va_arg(varg, u32);
                if(size == -1) temp &= 0xFFFF;
                if(size == -2) temp &= 0xFF;
                if(size == 2){} // TODO: Handle this!
                temp = (u32)print_int(temp, 8, 1, width, padzeros, showsign, signspace, 0, out);
                nchars += temp;
                out += temp;
                ZERO_ALL_VID();
                break;

            case 's': if(size > 0)
                {
                    temp = (u32)va_arg(varg, s16 *);
                    if(temp == 0) { temp = (u32)L"(null)"; }
                    else if(!mm_check_addr((void *)temp)) { temp = (u32)L"(badaddr)"; }
                    nchars += strlen((s16 *)temp);
                    while(*(s16 *)temp) *out++ = (char)*(s16 *)temp++;
                }
                else
                {
                    temp = (u32)va_arg(varg, char *);
                    if(temp == 0) { temp = (u32)"(null)"; }
                    else if(!mm_check_addr((void *)temp)) { temp = (u32)"(badaddr)"; }
                    nchars += strlen((char *)temp);
                    while(*(char *)temp) *out++ = *(char *)temp++;
                }
                ZERO_ALL_VID();
                break;

            case 'c': temp = (u32)va_arg(varg, int);
                if(size > 0) *out++ = (char)temp;
                else         *out++ = (char)temp;
                nchars++;
                ZERO_ALL_VID();
                break;

            case 'p': temp = (u32)va_arg(varg, u32);
                temp = (u32)print_int(temp, 16, 1, width, padzeros, showsign, signspace, 1 /* Should this be upper or lower case? */, out);
                nchars += temp;
                out += temp;
                break;

            case 'a':
            case 'A': (void)va_arg(varg, double);
                ZERO_ALL_VID();
                break;

            case 'n': (void)va_arg(varg, int);
                ZERO_ALL_VID();
                break;
        }

    }

    *(out) = 0;

    // These aren't used yet:
    (void)precision;
    (void)leftalign;

    return nchars;
}
// 格式化输出
int vsprintf(char *buf, const char *fmt, va_list args)
{
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