#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#define STR_DEFAULT_SIZE 8
// how much the size of the string should increase when it needs to realloc for more size
#define STR_GROW_MULT 2
// checks to see if memset was successful
#define CHECK_MEMSET

// the amount times greater a string max length must be compared to the normal length in order to reallocate to a smaller size
// This number should be greater than SHRINK_DIV and greater than one.
#define SHRINK_REQ 1.5
// the amount (division) max size should shrink when reallocating to a smaller string
// this number should be greater than SHRINK_REQ and greater than one
#define SHRINK_DIV 1.5

typedef struct {
    char* contents;
    size_t len;
    size_t max_len;
} str;
typedef enum {
    str_no_error = 0,
    str_mem_error = -1,
    str_index_out_of_bounds = -2,
    str_null_input = -2,
    str_corrupted_pointer = -2,
} str_res_id;
typedef struct {
    str string;
    str_res_id result;
} str_res;

// constructs an empty str with STR_DEFAULT_SIZE
str_res strNew();

// constructs a struct str with the contents containing the given char array
str_res fromCharArray(char* value);

// creates a new str from a reference of another str
str_res strClone(str* string);

// return a str with its contents holding value as a char array
// doesn't currently work with floating point numbers
str_res fromDouble(long double value);
str_res fromNum(unsigned int value);

int pushChar(str* string, char character);

int appendDouble(str* string, long double value);
int appendNum(str* string, unsigned int value);
int appendStr(str* string, str* value);
// Copys the argument value into str. Value is const an reatains its original value.
// Value must not be null or the code will abort.
// Arguments:
// @str - The string that will have argument value appended to.
// @value - The value that will be appended to arg str.
// @v_len - The amount of bytes of arg value that will be written to str.
int appendCharArray(str* str, const char* value, size_t v_len);

// Similar to function AppendCharArray, however the entire argument value is append.
int appendCAFull(str *str, const char* value);

// pops the front char in the string "foo" -> "fo"
int popFront(str *string);

// checks to see if the given str is a valid number
bool isNum(str* string);

#define insertAt(str, index, input) _Generic((input), \
        char: insertAt_char, \
        char*: insertAt_char_array \
        )(str, index, input)
// puts the char at the specified index shifting all the other bytes right
// If there is a value at index "index" then in will take it place and it will be left one place.
int insertAt_char(str* str, size_t index, char in);

// copys the chars in the char array "in" into the str at the specified index
int insertAt_char_array(str* str, size_t index, char* in);
/* int strReplace(str *str, char* find, char* replace); */
