#pragma once
#include "config_include.h"
#include "str.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <math.h>
// a single spreadsheet cell
typedef struct {
    union {
        char* str;
        cell_num_t number;
    } cell_value;
    enum {
        t_str,
        t_int,
        t_empty,
    } cell_type;
    // the value that will be rendered, sometimes different than its actual value especially if the number/string exceeds 12 characters
    char* displayed_value;
} cell_t;

// deallocates all of the allocated parts of the cell
void freeCell(cell_t cell);

// prints a single spread sheet cell
// args:
// x: x position of the cell
// y: y position of the cell
// output: the value where the spreadsheet cell will be written to
// value: the value that should be formatted
void print_cell(int x, int y,char* output, char* value);

// similar to print_cell(), however nothing is printing and the cell is saved to the argument @output
// used to construct a formatted cell
void create_cell(char* output, char* value, size_t value_len);

// converts a string to a floating point value. Used to convert some of the cells into integers while parsing
cell_num_t strToNum(char* value, size_t len);
// get/converts the cell value into a str
/* str_res cellToStr(cell_t cell); */
