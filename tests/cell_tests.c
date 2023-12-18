#include "../src/parsing.h"
#include "../src/client.h"

#ifndef PARSING_IMPL
#define PARSING_IMPL
#include "../src/parsing.c"
#endif


#include "tests.h"
#include <string.h>
// tests to see if the function strToNum works as itentended
int testStrToNum() {
    char* num1 = "100";
    long double converted_num = strToNum(num1, 3);
    T_ASSERT(converted_num == 100);
    num1 = "163";
    converted_num = strToNum(num1, 3);
    T_ASSERT(converted_num == 163);

    // tests unusual cases where there is a leading zero
    num1 = "0123";
    converted_num = strToNum(num1, 4);
    T_ASSERT(converted_num == 123);

    return T_SUCCESS;
}
int testStrToDecimal() {
    char* num1 = "3.141";
    long double converted_num = strToNum(num1, 5);
    T_ASSERT(converted_num * 1000 == 3141);
    num1 = "12.34";
    converted_num = strToNum(num1, 5);
    T_ASSERT(converted_num * 100 == 1234);
    return T_SUCCESS;
}
int testIntToStr() {
    return T_SUCCESS;
}
