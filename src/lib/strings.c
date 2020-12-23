#include "strings.h"

void *memset(void *s, int c, size_t n)
{
    /* TODO: Your code here */
    for (int i = 0; i < n; ++i) {
        ((char*)s)[i] = (char)c;
    }
    return s;
}


/*
 * Copy `n` bytes from the memory area `src` to the memory area `dst`. If `dst`
 * and `src` overlap, the behavior is undefined.
 *
 * Returns its first argument: `dst`.
 */
void *memcpy(void *dst, const void *src, size_t n)
{
    /* TODO: Your code here */
    for (int i = 0; i < n; i++) {
        ((char*)dst)[i] = ((char*)src)[i];
    }
    return dst;
}

/*
 * Report the length of string `s`.
 *
 * Returns the number of characters that precede the null-terminator.
 */
size_t strlen(const char *s)
{
    /* Implementation a gift to you from lab3 */
    int n = 0;
    while (s[n] != '\0') {
        n++;
    }
    return n;
}

/*
 * helper function that returns the minimum of two integers
*/
int min(int a, int b) {
    return a > b ? b : a;
}

/*
 * Lexicographically compare the null-terminated strings `s1` and `s2`.
 *
 * Returns an integer value greater than, equal to, or less than 0, according
 * to whether the string `s1` is greater than, equal to, or less than
 * the string `s2`. Comparison is done as unsigned characters.
 */
int strcmp(const char *s1, const char *s2)
{
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    for (int i = 0; i <= min(len1, len2); i++) {
        if (s1[i] == s2[i]) {
            continue;
        }
        return s1[i] > s2[i] ? 1 : -1;
    }
    return 0;
}

/*
 * Size-bounded string concatenation. Append the null-terminated string `src`
 * to the end of `dst`. `dst` may or may not be null terminated. If `dst` is
 * null terminated, `strlcat` appends at most `maxsize - strlen(dst) - 1` bytes, and
 * null-terminates `dst`. If `dst` is not null terminated, `strlcat` writes
 * nothing to `dst`. If `dst` and `src` overlap, the behavior is undefined.
 *
 * Returns the initial length of `dst` plus the length of `src`. This is
 * equal to the final size of `dst` if there were space to write all of
 * `src`. If `dst` is not null terminated, this is equal to `maxsize
 * + strlen(src)`.
 */
size_t strlcat(char *dst, const char *src, size_t maxsize)
{

    int i = 0;
    while (i < maxsize && dst[i] != '\0') {
        i++;
    }

    int terminated = 0;
    for (int j = i; j < maxsize - 1; j++) {
        dst[j] = src[j-i];
        if (src[j-i] == '\0') { // early exit if src is short
            terminated = 1;
            break;
        }
    }
    if (!terminated) {
        dst[maxsize - 1] = '\0';
    }

    return i + strlen(src);
}

/*
 * Convert the digit characters in `str` to the corresponding unsigned integer
 * value. If `str` begins with the prefix "0x", the characters of `str` will
 * be interpreted as hexadecimal digits (base 16); otherwise the characters
 * are interpreted as decimal digits (base 10). No other bases are supported.
 * The hex letter digits are to be specified in lowercase.
 *
 * `strtonum` processes the characters of `str`, stopping at the first
 * character that is not a valid digit in the base or at the terminating
 * null (whichever comes first).  The function is not required to support
 * leading spaces or a plus/minus sign. Such characters can be
 * treated as invalid and stop the conversion.
 *
 * The argument `endptr` is an output parameter optionally used to
 * communicate back to the caller what characters remain in `str` after having
 * "consumed" the digit characters. A caller can pass NULL for `endptr`
 * if they do not need this information.
 *
 * If `endptr` is not NULL, *endptr is updated to point to the character
 * in `str` where conversion stopped. This is either the address of the
 * first invalid character in `str` or the address of the terminating null
 * if all characters in `str` are valid digits.
 *
 * Returns the result of the conversion as an unsigned integer. Returns 0
 * if the first character of `str` is not a valid digit.
 */

int convert_digit(char c, unsigned int base) {
    return (c - '0' >= base || c < '0') ? -1 : c - '0';
}

int convert_letter(char c, unsigned int base) {
    int s = convert_digit(c, base);
    if (s != -1) {
        return s;
    }
    return (c - 'a' >= base || c < 'a') ? -1 : c - 'a' + 10;
}

int convert_char(char c, unsigned int base) {
    if (c == '\0') return -1;
    if (base > 10) {
        return convert_letter(c, base);
    }
    return convert_digit(c, base);
}


// char* to int
unsigned int convert(const char *str, const char **endptr, unsigned int base) {
    int i = 0;
    unsigned int sum = 0;
    int s = convert_char(str[i], base);
    while (s != -1) {
        sum *= base;
        sum += s;
        i++;
        s = convert_char(str[i], base);
    }
    if (endptr) *endptr = &str[i];
    return sum;
}

unsigned int strtonum(const char *str, const char **endptr)
{
    if (str[0] == '\0' || str[1] == '\0') {
        return convert(str, endptr, 10);
    }
    else if (str[0] == '0' && str[1] == 'x') {
        return convert(&str[2], endptr, 16);
    }
    return convert(str, endptr, 10);
}