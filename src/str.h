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
typedef struct {
    str string;
    int result;
} str_res;

static str_res strNew() {
    str str;
    if ((str.contents = calloc(STR_DEFAULT_SIZE, 1)) == NULL) {
        return (str_res) {.result = -1};
    }
    str.len = 0;
    str.max_len = STR_DEFAULT_SIZE;
    return (str_res) {
        .string = str,
        .result = 0,
    };
}
// constructs a struct str with the contents containing the given char array
static str_res fromCharArray(char* value) {
    str str;
    // malloc the amount of bytes value has
    str.len = strlen(value);
    str.max_len = strlen(value) + 5;
    if ((str.contents = calloc(str.max_len, 1)) == NULL)
        return (str_res) {.result = -1};
    // no need for strncpy cause str.contents will always have enough memory to hold value
    strcpy(str.contents, value);
    return (str_res) {.string = str, .result = 0};
}

static str_res strClone(str* string) {
    str new_string = { .len = string->len, .max_len = string->max_len};
    if ((new_string.contents = calloc(string->max_len, 1)) == NULL)
        return (str_res) {.result = -1};
    strcpy(new_string.contents, string->contents);
    return (str_res) {.string = new_string, .result = 0};
}

// return a str with its contents holding value as a char array
// doesn't currently work with floating point numbers
static str_res fromDouble(long double value) {
    str str;
    // gets the number places of value
    str.len = floor(log10(value)) + 8;
    str.max_len = str.len + 12;
    if ((str.contents = calloc(str.max_len, 1)) == NULL)
        return (str_res) {.result = -1};
    snprintf(str.contents, str.len + 8, "%Lf", value);
    return (str_res) {.string = str, .result = 0};
}

static int pushChar(str* string, char character) {
    string->contents[string->len] = character;
    if (++string->len == string->max_len) {
        string->max_len *= 2;
        if ((string->contents = realloc(string->contents, string->max_len) ) == NULL)
            return -1;
#ifdef CHECK_MEMSET
        char* original_address = string->contents;
#endif
        // sets all of the garbage bytes from realloc to zero
        memset(string->contents + string->len, 0x0,string->max_len - string->len );
#ifdef CHECK_MEMSET
        // checks to see if the memory address changed after memset
        // if it did that means an error had occured
        if (original_address != string->contents) return -1;
#endif
    }
    return 0;
}
static int appendStr(str* string, str* value) {
    int res = 0;
    for (size_t index = 0; index < value->len; ++index) {
        res += pushChar(string, value->contents[index]);
    }
    if (res != 0)
        return -1;
    return 0;
}
// pops the front char in the string "foo" -> "fo"
static int popFront(str *string) {
    // if the length is zero that means we can't pop anything
    if (string->len == 0)
        return -2;
    // reallocate to a smaller size when the string length is much smaller than the string max
    if (string->len * SHRINK_REQ < string->max_len ) {
        string->max_len /= 1.25;
        if ((string->contents = realloc(string->contents, string->max_len) ) == NULL)
            return -1;
    }
    string->contents[--string->len] = 0x0;
    return 0;
}
// checks to see if the given str is a valid number
static inline bool isNum(str* string) {
    bool decimal_appear = false;
    for (size_t char_index = 0; char_index < string->len; ++char_index) {
        if ((string->contents[char_index] >= 48) && (string->contents[char_index] < 58)) {
            continue;
        }
        // this means we encountered another decimal in the number making it invalid
        if (string->contents[char_index] == '.' && !decimal_appear)
            decimal_appear = true;
        else
            return false;
    }
    return true;
}
