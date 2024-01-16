#include "parsing.h"
#include "cell.h"
#include "client.h"
#include "config_include.h"
#include "str.h"
#include "vec.h"
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

#define SHEET client.spread_sheet
// converts the file stream char to a number correctly if the char is '0' ... '9'
#define CH_INT ((int)ch - 48)

static void insertCell(row_t *current_row, str* str, bool is_num) {
    cell_t current_cell;
    char* disp_val = malloc(CELL_LEN + 1);
    create_cell(disp_val, str->contents,  str->len);
    current_cell.displayed_value = disp_val;
    // if there is nothing in the cell then insert cell type empty
    if (str->len == 0) {
        cell_t empty_cell = {
            .cell_type = t_empty
        };
        VEC_APPEND((*current_row), empty_cell );
        return;
    }
    if (is_num)
        // frees value because function freeSpreadsheet won't be able to free it
        current_cell.cell_value.number = strToNum(str->contents, str->len),
        free(str->contents),
        current_cell.cell_type = t_int;
    else
        current_cell.cell_value.str = str->contents,
        current_cell.cell_type = t_str;
    VEC_APPEND((*current_row), current_cell);
}

int fromCSV(Cellulose *client) {
    FILE* fp = fopen(client->name, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: encountered error opening file %s. Error: (%s).\n", client->name, strerror(errno));
        return -1;
    }
    // checks to see if the cell value can be a valid floating point number
    bool is_num = true;

    row_t current_row = (row_t)VEC_NEW(cell_t);
    // one is the size of char
    // used calloc instead of malloc because we don't want the string to be filled with garbage bytes
    str_res res;
    str value;
    if ((res = strNew()).result == -1) {
        printf("ERROR: failed to malloc cell value.");
        return -1;
    }
    value = res.string;
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        switch (ch) {
            case ',': {
                insertCell(&current_row, &value, is_num);
                is_num = true;
                if ((res = strNew()).result == -1) {
                    printf("ERROR: failed to malloc cell value.");
                    return -1;
                }
                value = res.string;
            } break;
            case '\n': {
                insertCell(&current_row, &value, is_num);
                is_num = true;
                if ((res = strNew()).result == -1) {
                    printf("ERROR: failed to malloc cell value.");
                    return -1;
                }
                value = res.string;
                VEC_APPEND(SHEET_P, current_row);
                current_row = (row_t)VEC_NEW(cell_t);
            } break;
            default: {
                // becomes false if the character is not a number or stays false if it vas never a number
                is_num = is_num && (( CH_INT >= 0 && CH_INT <= 9) || ch == '.');
                if (pushChar(&value, ch) == -1) {
                    printf("ERROR: couldn't push char.\n");
                    return -1;
                }
            } break;
        }
    }

    if (value.len == 0)
        free(value.contents);
    // this means the spreadsheet client doesn't hold this row and so will not deallocate it
    if (VEC_LEN(current_row) == 0)
        VEC_FREE(current_row);
    // close the file once we are done with it
    fclose(fp);
    return 0;
}
