#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "config_include.h"
#include "cell.h"
#include "vec.h"
#include "str.h"
#include "cursor/cursor_include.h"

#define ELEMENTS client.spread_sheet.elements
#define ELEMENTS_P client->spread_sheet.elements
#define SHEET_P client->spread_sheet
#define SHEET_LEN client->spread_sheet.length

#define CELL_VAL(_y, _x) ELEMENTS[_y].elements[_x].cell_value.str
#define CELL_VAL_P(_y, _x) ELEMENTS_P[_y].elements[_x].cell_value.str
#define CELL_P(_y, _x) ELEMENTS_P[_y].elements[_x]
#define CELL(_y, _x) ELEMENTS[_y].elements[_x]
#define CUR_CELL_P CELL_P(cursor->y, cursor->x)
#define ROW(_y) ELEMENTS[_y]
#define ROW_P(_y) ELEMENTS_P[_y]
#define CLIENT_SHEET_WIDTH 11
#define CLIENT_SHEET_HEIGHT 39

typedef  VEC(cell_t) row_t;
typedef  VEC(row_t) spreadsheet_t;

typedef struct {
    // the positon of the spreadsheet. Only cells within an area of (pos_x + screen width, pos_y + screen height) will be rendered
    uint16_t pos_x,pos_y;
    // the spreadsheet
    spreadsheet_t spread_sheet;
    // when true the corresponding elements will be redrawn
    bool redraw_dividers, redraw_spreadsheet;
} Cellulose;


// Constructor for struct Cellulose. All of the attributes have their own "default" value.
static Cellulose newEmpty() {
    Cellulose cell;
    cell.pos_x = 0;
    cell.pos_y = 0;
    cell.redraw_dividers = true;
    cell.redraw_spreadsheet = true;
    cell.spread_sheet = (spreadsheet_t)VEC_NEW(row_t);
    assert(cell.spread_sheet.elements && "ERROR: failed to allocate memory for cellulose vector");
    return cell;
}

// writes to a given file with arg seperator seperating each value
void writeContents(char seperator, row_t* spreadsheet, FILE* output );

// deconstructor for struct Cellulose
void freeSpreadsheet(Cellulose cellulose) {
    // iterates over all of the rows
    VEC_ITER(cellulose.spread_sheet, row_t, sheet_row) {
        // iterates all of the cells in the row
        VEC_ITER(sheet_row, cell_t, row_cell) {
            if (row_cell.cell_type == t_str) free(row_cell.cell_value.str);
            free(row_cell.displayed_value);
        }
        VEC_FREE(sheet_row); // frees the pointer to the rows
    }
    VEC_FREE(cellulose.spread_sheet);
}

// checks to see if a certain cell coordinate exists in the spreadsheet
static inline bool cellExist(Cellulose *client, size_t x, size_t y) {
    return (y < SHEET_P.length) ? (x < ROW_P(y).length): false;
}
// sets the cell.cell_value
// used for the function setCell
static inline void setCellValue(Cellulose *client, cursor* cursor, str* cell_input) {
    if (isNum(cell_input))
        CELL_P(cursor->y, cursor->x).cell_type = t_int,
        CELL_P(cursor->y, cursor->x).cell_value.number = strToNum(cell_input->contents, cell_input->len),
        // free it here because the client will not be able to
        free(cell_input->contents);
    else
        CELL_P(cursor->y, cursor->x).cell_type = t_str,
        CELL_P(cursor->y, cursor->x).cell_value.str = cell_input->contents;
}
// Adds rows to the spreadsheet until the number of rows is equal to the argument y. Creates no rows if the number of rows is equal to or greater than y.
static inline void createRowsTo(Cellulose *client, size_t y) {
    for (size_t rs = client->spread_sheet.length; rs <= y; ++rs) {
        row_t r = (row_t)VEC_NEW(cell_t);
        VEC_APPEND(SHEET_P, r)
    }
}
// Adds columns to a certain row until the number of columns in that row is equal to the argument x. Creates no columns if the number of columns in the row is equal to or greater than x.
static inline int createColumnsTo(Cellulose *client, size_t row_index, size_t x) {
    for (size_t column = ROW_P(row_index).length; column <= x; ++column) {
        char* empty_displayed;
        if ((empty_displayed = malloc(CELL_LEN)) == NULL)
            return -1;
        strcpy(empty_displayed, EMPTY_CELL);
        cell_t empty_cell = (cell_t) {
            .cell_type = t_int,
            .displayed_value = empty_displayed
        };
        VEC_APPEND(ROW_P(row_index), empty_cell);
    }
    return 0;
}
// sets the cell at the cursor x and y to the value inside cell_input
static int setCell(Cellulose *client, cursor* cursor, str* cell_input) {
    if (!cellExist(client, cursor->x, cursor->y))
        createRowsTo(client, cursor->y),
        createColumnsTo(client, cursor->y, cursor->x);
    char* output;
    // update the display value because the value has changed
    if ((output = calloc(15,1)) == NULL) return -1;
    create_cell(output, cell_input->contents, cell_input->len);
    CELL_P(cursor->y, cursor->x).displayed_value = output;
    setCellValue(client, cursor, cell_input);
    // create a new input string
    str_res res = strNew();
    if (res.result == -1) return -1;
    *cell_input = res.string;
    return 0;
}
