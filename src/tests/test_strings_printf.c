#include "assert.h"
#include "printf.h"
#include "printf_internal.h"
#include <stddef.h>
#include "strings.h"
#include "uart.h"

static void test_memset(void)
{
    int numA = 0xefefefef;
    int numB = 2;

    memset(&numB, 0xef, sizeof(int));
    assert(numA == numB);
}

static void test_memcpy(void)
{
    int numA = 0x12345678;
    int numB = 2;

    memcpy(&numB, &numA, sizeof(int));
    assert(numA == numB);
}

static void test_strlen(void)
{
    assert(strlen("green") == 5);
    assert(strlen("\0") == 0);
}

static void test_strcmp(void)
{
    assert(strcmp("apple", "apple") == 0);
    assert(strcmp("apple", "applesauce") < 0);
    assert(strcmp("pears", "apples") > 0);
}

static void test_strlcat(void)
{
    char buf[20];
    memset(buf, 0x77, sizeof(buf)); // init contents with known value

    buf[0] = '\0'; // start with empty string
    assert(strlen(buf) == 0);
    strlcat(buf, "CS", sizeof(buf));
    assert(strlen(buf) == 2);
    assert(strcmp(buf, "CS") == 0);
    strlcat(buf, "107e", sizeof(buf));
    assert(strlen(buf) == 6);
    assert(strcmp(buf, "CS107e") == 0);

    // 1. Test strlen(dst) > maxsize
    memset(buf, 0x77, sizeof(buf)); // init contents with known nonzero value
    assert(strlcat(buf, "CS", sizeof(buf)) == 22);

    // 2. Test src + dst longer than maxsize
    char buf2[5];
    memset(buf2, 0x77, sizeof(buf2)); // init contents with known nonzero value
    buf2[0] = 'c';
    buf2[1] = 's';
    buf2[2] = '\0';
    int r3 = strlcat(buf2, "107e", sizeof(buf2));
    assert(r3 == 6);
    assert(strcmp(buf2, "cs10") == 0);

    // 3. Test when src is NULL
    memset(buf2, 0x77, sizeof(buf2)); // init contents with known nonzero value
    buf2[0] = 'c';
    buf2[1] = 's';
    buf2[2] = '\0';
    int r4 = strlcat(buf2, NULL, sizeof(buf2));
    assert(r4 == 2);
    assert(strcmp(buf2, "cs") == 0);
}

static void test_strtonum(void)
{
    int val = strtonum("013", NULL);
    assert(val == 13);

    const char *input = "107rocks", *rest = NULL;
    val = strtonum(input, &rest);
    assert(val == 107);
    assert(rest == &input[3]);

    // Test base 10
    const char *input2 = "107";
    const char *rest2 = NULL;
    val = strtonum(input2, &rest2);
    assert(val == 107);
    assert(rest2 == &input2[3]);

    const char *input3 = "0";
    const char *rest3 = NULL;
    val = strtonum(input3, &rest3);
    assert(val == 0);
    assert(rest3 == &input3[1]);

    const char *input4 = "a";
    const char *rest4 = NULL;
    val = strtonum(input4, &rest4);
    assert(val == 0);
    assert(rest4 == input4);

    // Test base 16
    const char *input5 = "0x107";
    const char *rest5 = NULL;
    val = strtonum(input5, &rest5);
    assert(val == 263);
    assert(rest5 == &input5[5]);

    const char *input6 = "0x0";
    const char *rest6 = NULL;
    val = strtonum(input6, &rest6);
    assert(val == 0);
    assert(rest6 == &input6[3]);

    const char *input7 = "0xa";
    const char *rest7 = NULL;
    val = strtonum(input7, &rest7);
    assert(val == 10);
    assert(rest7 == &input7[3]);

    const char *input8 = "0xabcdefzz";
    const char *rest8 = NULL;
    val = strtonum(input8, &rest8);
    assert(val == 11259375);
    assert(rest8 == &input8[8]);


    const char *input9 = "0xabcdef";
    val = strtonum(input9, NULL);
    assert(val == 11259375);
}

static void test_to_base(void)
{
    char buf[5];
    size_t bufsize = sizeof(buf);

    memset(buf, 0x77, bufsize); // init contents with known value

    int n = unsigned_to_base(buf, bufsize, 9999, 10, 6);
    assert(strcmp(buf, "0099") == 0)
    assert(n == 6);

    memset(buf, 0x77, bufsize);
    n = signed_to_base(buf, bufsize, -9999, 10, 6);
    assert(strcmp(buf, "-099") == 0)
    assert(n == 6);

    memset(buf, 0x77, bufsize);
    n = unsigned_to_base(buf, bufsize, 1, 10, 3);
    assert(strcmp(buf, "001") == 0)
    assert(n == 3); 

    char buf2[35];
    n = unsigned_to_base(buf2, 20, 35, 16, 4);
    assert(strcmp(buf2, "0023") == 0);
    assert(n==4);

    // test  0
}

// static void test_snprintf(void)
// {
//     char buf[100];
//     size_t bufsize = sizeof(buf);

//     memset(buf, 0x77, sizeof(buf)); // init contents with known value

//     // Start off simple...
//     snprintf(buf, bufsize, "Hello, world!");
//     assert(strcmp(buf, "Hello, world!") == 0);

//     // Decimal
//     snprintf(buf, bufsize, "%d", 45);
//     assert(strcmp(buf, "45") == 0);

//     // Hexadecimal
//     snprintf(buf, bufsize, "%04x", 0xef);
//     assert(strcmp(buf, "00ef") == 0);

//     // Pointer
//     snprintf(buf, bufsize, "%p", (void *) 0x20200004);
//     assert(strcmp(buf, "0x20200004") == 0);

//     // Character
//     snprintf(buf, bufsize, "%c", 'A');
//     assert(strcmp(buf, "A") == 0);

//     // String
//     snprintf(buf, bufsize, "%s", "binky");
//     assert(strcmp(buf, "binky") == 0);

//     // Format string with intermixed codes
//     snprintf(buf, bufsize, "CS%d%c!", 107, 'e');
//     assert(strcmp(buf, "CS107e!") == 0);

//     // Test return value
//     assert(snprintf(buf, bufsize, "Hello") == 5);
//     assert(snprintf(buf, 2, "Hello") == 5);
// }


void main(void)
{
    // TODO: Add more and better tests!

    uart_init();
    uart_putstring("Start execute main() in tests/test_strings_printf.c\n");

    test_memset();
    test_memcpy();
    test_strlen();
    test_strcmp();
    test_strlcat();
    test_strtonum();
    test_to_base();
    // test_snprintf();

    // Our own test


    uart_putstring("Successfully finished executing main() in tests/test_strings_printf.c\n");
    uart_putchar(EOT);
}
