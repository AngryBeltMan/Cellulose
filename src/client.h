#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "cell.h"
#include "vec.h"

#define ELEMENTS client.spread_sheet.elements
#define ELEMENTS_P client->spread_sheet.elements
#define SHEET_P client->spread_sheet
#define CELL_VAL(_y, _x) ELEMENTS[_y].elements[_x].cell_value.str
#define CELL_VAL_P(_y, _x) ELEMENTS_P[_y].elements[_x].cell_value.str
#define CELL_P(_y, _x) ELEMENTS_P[_y].elements[_x]
#define CELL(_y, _x) ELEMENTS[_y].elements[_x]
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
} Cellulose;


// Constructor for struct Cellulose. All of the attributes have their own "default" value.
static Cellulose newEmpty() {
    Cellulose cell;
    cell.pos_x = 0;
    cell.pos_y = 0;
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
static bool cellExist(Cellulose *client, size_t x, size_t y) {
    return (y < SHEET_P.length) ? (x < ROW_P(y).length): false;
}
