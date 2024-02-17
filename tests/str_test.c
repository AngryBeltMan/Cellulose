#include "tests.h"
#include <stdio.h>
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
int testWholeNumToStr() {
    int in = 123;
    str_res res = fromNum(in);
    T_ASSERT(res.result == str_no_error);
    T_ASSERT(strcmp(res.string.contents, "123") == 0);
    return T_SUCCESS;
}
int testStrAppendWholeNum() {
    str_res victim = fromCharArray("hello");
    T_ASSERT(victim.result == str_no_error);
    int in = 123;
    appendNum(&victim.string, in);
    T_ASSERT(strcmp(victim.string.contents, "hello123") == 0);
    str a = {
        .contents = "hey",
        .len = 3
    };
    appendStr(&victim.string, &a);
    printf("%s\n", victim.string.contents);
    T_ASSERT(strcmp(victim.string.contents, "hello123hey") == 0);
    return T_SUCCESS;
}
// tests negative inputs for the function fromDouble
int testDoubleToStrNegative() {
    str_res res = fromDouble(-10);
    T_ASSERT(res.result == str_no_error);
    T_ASSERT(strcmp(res.string.contents, "-10.00000"));
    return T_SUCCESS;
}
// tests to see if isNum correctly evaluated negative numbers
int testIsNumNegative() {
    str_res in = fromCharArray("-32");
    T_ASSERT(in.result == str_no_error);
    T_ASSERT(strcmp(in.string.contents, "-32") == 0);
    T_ASSERT(isNum(&in.string))
    return T_SUCCESS;
}
