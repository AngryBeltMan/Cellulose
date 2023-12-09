// contains the default constructer and destructor of the struct Cellulose
#include "client.h"
#include "config.h"
#include "vec.h"
#include <curses.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
// draws the cells that specify the x and y coordinate of the cells
// | ||A||B||C| <
// |1|| || || |
// |2| || || |
// |3|| || || |
//  ^
void drawColumnHeader(Cellulose *spreadsheet, char* cell);
void drawRowHeader(Cellulose *spreadsheet, char* cell);

void renderSpreadsheet(Cellulose *cellulose, char* row_cell) {
    attron(COLOR_PAIR(COLUMN_CELL_ID));
    drawColumnHeader(cellulose, row_cell);
    attron(COLOR_PAIR(ROW_CELL_ID));
    drawRowHeader(cellulose, row_cell);
    /* attron(COLOR_PAIR(DEFAULT_CELL_ID)); */
    move(0,0);
    printw("               ");
    // render all of the cells
    /* for (int rows = 0; rows < cellulose->range_y_max - cellulose->range_y_min; ++rows ) { */
    /* // this means the rows doesn't exist and should just be empty cells */
    /*     if (rows + cellulose->range_y_min > cellulose->spread_sheet.rows) { */
    /*         for (int empty_row = 0; empty_row < cellulose->range_x_max - cellulose->range_x_min; ++empty_row) { */
    /*             print_cell((empty_row + 1) * 15, rows + 1, row_cell, ""); */
    /*         } */
    /*         continue; */
    /*     } */
    /*     for (int column = 0; column < cellulose->range_x_max - cellulose->range_x_min; ++column) { */
    /*         if (column < cellulose->spread_sheet.sheet[rows].cell_count) */
    /*         move(rows + 1,(column + 1) * 15), */
    /*         printw("%s",cellulose->spread_sheet.sheet[rows].cells[column].displayed_value); */
    /*         else */
    /*         print_cell((column + 1) * 15, rows + 1, row_cell, ""); */
    /*     } */
    /* } */
}

void drawColumnHeader(Cellulose *spreadsheet, char* cell) {
    for (int column = 0; column < spreadsheet->range_x_max - spreadsheet->range_x_min; ++column)
        print_cell((column + 1) * 15, 0, cell, "A");

}
void drawRowHeader(Cellulose *spreadsheet, char* cell) {
    for (int row = 0; row < spreadsheet->range_y_max - spreadsheet->range_y_min; ++row)
        print_cell(0, row + 1, cell, "B" );
}

