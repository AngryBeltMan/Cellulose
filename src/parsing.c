#include "parsing.h"
#include "cell.h"
#include "client.h"
#include "vec.h"
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

#define SHEET client.spread_sheet
// converts the file stream char to a number correctly if the char is '0' ... '9'
#define CH_INT ((int)ch - 48)

row_t rowEmpty() {
    row_t new_row = VEC_NEW(cell_t);
    return new_row;
}
void insertCell(char* value, size_t value_len, row_t *current_row, bool is_num) {
    cell_t current_cell;
    char* disp_val = malloc(15);
    create_cell(disp_val, value, value_len);
    current_cell.displayed_value = disp_val;
    if (is_num)
        // frees value because function freeSpreadsheet won't be able to free it
        current_cell.cell_value.number = strToNum(value, value_len),
        free(value),
        current_cell.cell_type = t_int;
    else
        current_cell.cell_value.str = value,
        current_cell.cell_type = t_str;
    current_cell.selected = false;
    VEC_APPEND((*current_row), current_cell);
}

Cellulose fromCSV(const char* file_path) {
    FILE* fp = fopen(file_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: encountered error opening file (%s).\n", strerror(errno));
        fclose(fp);
        exit(1);
    }

    Cellulose client = newEmpty();
    // the max size
    size_t value_max = 6;
    size_t value_len = 0;
    // checks to see if the cell value can be a valid floating point number
    bool is_num = true;

    row_t current_row = rowEmpty();
    // one is the size of char
    // used calloc instead of malloc because we don't want the string to be filled with garbage bytes
    char* value =  calloc(6, 1);
    assert(value && "Failed to malloc while parsing csv file.");

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        switch (ch) {
            case ',': {
                insertCell(value, value_len, &current_row, is_num);
                value_len = 0;
                value_max = 6;
                is_num = true;
                value = calloc(6, 1);
                assert(value && "Failed to malloc while parsing csv file.");
                break;
            }
            case '\n': {
                insertCell(value, value_len, &current_row, is_num);
                value_len = 0;
                value_max = 6;
                value = calloc(6, 1);
                is_num = true;
                assert(value && "Failed to malloc while parsing csv file.");
                VEC_APPEND(SHEET, current_row);
                current_row = rowEmpty();
                break;
            }
            default: {
                // becomes false if the character is not a number or stays false if it vas never a number
                is_num = is_num && (( CH_INT >= 0 && CH_INT <= 9) || ch == '.');
                value[value_len] = ch;
                if (++value_len == value_max) {
                    value_max *= 2;
                    value = realloc(value, value_max);
                    assert(value && "Failed to malloc while parsing csv file.");
                    // store the address of value before memset
                    char* value_original_address = value;
                    // set all of the new malloced bytes to zero because they could be random values which could corrupt the actual value of the cell
                    memset(value + value_len, 0x0, value_max - value_len);
                    // checks to see if memset failed
                    assert(value == value_original_address && "ERROR: MEMSET failed and is now pointing to unknown memory");
                }
                break;
            }
        }
    }

    if (value_len == 0)
        free(value);

    // this means the spreadsheet client doesn't hold this row and so will not deallocate it
    if (VEC_LEN(current_row) == 0)
        VEC_FREE(current_row);
    // close the file once we are done with it
    fclose(fp);
    return client;
}
