#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <math.h>
// a single spreadsheet cell
typedef struct {
    union {
        char* str;
        long double number;
    } cell_value;
    enum {
        t_str,
        t_int,
        t_empty,
    } cell_type;
    // the value that will be rendered, sometimes different than its actual value especially if the number/string exceeds 12 characters
    char* displayed_value;
} cell_t;

static inline void freeCell(cell_t cell) {
    if (cell.cell_type == t_empty)
        return;
    free(cell.displayed_value);
    if (cell.cell_type == t_str)
        free(cell.cell_value.str);
}
// prints a single spread sheet cell
// args:
// x: x position of the cell
// y: y position of the cell
// output: the value where the spreadsheet cell will be written to
// value: the value that should be formatted
static inline void print_cell(int x, int y,char* output, char* value) {
    move(y,x);
    memset(output, ' ', 10);
    output[11] = '\0';
    size_t len = strlen(value);
    memmove(output + 1, value, len < 11 ? len : 11);
    printw("%s%s", output,len > 12 ? " .." : "     ");
}
// similar to print_cell(), however nothing is printing and the cell is saved to the argument @output
// used to construct a formatted cell
static inline void create_cell(char* output, char* value, size_t value_len) {
    memset(output, ' ', 12);
    memcpy(output + 1, value, value_len < 12 ? value_len : 12);
    memcpy(output + 12, value_len > 12 ? ".." : "  ", 3);
}
#define CURRENT_CHAR value[num_index - 1]
// converts a string to a floating point value. Used to convert some of the cells into integers while parsing
static long double strToNum(char* value, size_t len) {
    long double converted_value = 0;
    float number_place = 1;
    for (size_t num_index = len; num_index > 0; --num_index, number_place *= 10) {
        if (CURRENT_CHAR == '.')
            number_place -= powf(10, len - num_index) - 0.1,
            converted_value /= powf(10, len - num_index);
        else
            converted_value += (value[num_index - 1] - 48) * number_place;
    }
    return converted_value;
}
