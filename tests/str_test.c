#include "tests.h"
#include <string.h>
#include "../src/str.h"
int testNumToStr() {
    long double test_num = 123;
    str string = fromDouble(test_num).string;
    T_ASSERT(string.len == 10);
    T_ASSERT(!strcmp(string.contents, "123.000000" ));
    test_num = 3.14;
    string = fromDouble(test_num).string;
    T_ASSERT(!strcmp(string.contents, "3.140000" ));
    T_ASSERT(string.len == 8);
    return T_SUCCESS;
}
int testStrShrink() {
    str string = strNew().string;
    for (int a = 0; a < 60; ++a)
        pushChar(&string, 'a');
    size_t original_len = string.max_len;
    for (int a = 0; a < 40; ++a)
        popFront(&string);
    size_t new_len = string.max_len;
    printf("%lu\n", string.len);
    printf("%lu %lu\n", original_len, new_len);
    T_ASSERT(original_len > new_len);
    return T_SUCCESS;
}
