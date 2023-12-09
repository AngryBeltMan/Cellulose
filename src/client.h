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
#define CELL_VAL(_y, _x) ELEMENTS[_y].elements[_x].cell_value.str
#define CELL_VAL_P(_y, _x) ELEMENTS_P[_y].elements[_x].cell_value.str
#define CELL_P(_y, _x) ELEMENTS_P[_y].elements[_x]
#define CELL(_y, _x) ELEMENTS[_y].elements[_x]
#define ROW(_y) ELEMENTS[_y]
#define ROW_P(_y) ELEMENTS_P[_y]

typedef  VEC(cell_t) row_t;
typedef  VEC(row_t) spreadsheet_t;

typedef struct {
    // the cursor positon
    uint16_t pos_x,pos_y;
    // the spreadsheet
    spreadsheet_t spread_sheet;
    // the viewing range of the spread sheet
    // What this means is only the cells in the spreadsheet between range_x_min, range_x_max, range_y_min, and range_y_max are rendered on the tui.
    uint16_t range_x_min, range_x_max, range_y_min, range_y_max;
} Cellulose;


// Constructor for struct Cellulose. All of the attributes have their own "default" value.
static Cellulose newEmpty() {
    Cellulose cell;
    cell.range_x_max = 16;
    cell.range_x_min = 5;
    cell.range_y_min = 0;
    cell.range_y_max = 39;
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


