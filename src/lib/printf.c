#include "printf.h"
#include "printf_internal.h"
#include <stdarg.h>
#include "string.h"


#define MAX_OUTPUT_LEN 1024

char int_to_char(char i, int base) {
    //  0 <= i <= 255
    if (i >= 10) {
        return 'a' + (i - 10);
    }
    return '0' + i;
}

int unsigned_to_base(char *buf, size_t bufsize, unsigned int val, int base, int min_width)
{
    if(bufsize == 0) return 0;

    int i = 9;
    char tmp[10]; // We use 10 as lg(2^32) < 10. TODO: refactor 10 as a macro.
    memset(tmp, 0, sizeof(tmp));
    while (val> 0) {
        tmp[i] = val % base;
        val /= base;
        i--;
    }

    // TODO we don't need new_str, just reuse tmp.
    char new_str[11];
    memset(new_str, 0, sizeof(new_str));
    for (int i = 0; i < 10; i++) {
        new_str[i] = int_to_char(tmp[i], base);
    }

    // i is the rightmost 0 character
    int val_len_in_base = 9 - i;
    int j = min_width > val_len_in_base ? 10 - min_width : i + 1;
    // We start writing from j
    int k = j;
    while (k <= 10) { // <= 10 to write the null terminator
        if (k-j == bufsize-1) {
            buf[k-j] = '\0';
            break;
        }
        buf[k-j] = new_str[k];
        k++;
    }

    return 10 - j;
}

int signed_to_base(char *buf, size_t bufsize, int val, int base, int min_width)
{
    if (bufsize == 0) {
        return 0;
    }
    // put the negative sign if there is
    if (val < 0) {
        // todo shit
        buf[0] = '-';
        return unsigned_to_base(buf++,bufsize-1, -val, base, min_width-1);
    } else {
        return unsigned_to_base(buf,bufsize, val, base, min_width);
    }
}

int vsnprintf(char *buf, size_t bufsize, const char *format, va_list args)
{
    /* TODO: Your code here */
    return 0;
}

int snprintf(char *buf, size_t bufsize, const char *format, ...)
{
    /* TODO: Your code here */
    return 0;
}

int printf(const char *format, ...)
{
    /* TODO: Your code here */
    return 0;
}
