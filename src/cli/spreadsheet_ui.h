// contains the default constructer and destructor of the struct Cellulose
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <ncurses.h>
#include <stdlib.h>
#include "../client.h"
#ifndef CONFIG_PATH
#define CONFIG_PATH "../config.h"
#endif
#include CONFIG_PATH
// draws the cells that specify the x and y coordinate of the cells
// | ||A||B||C| <
// |1|| || || |
// |2| || || |
// |3|| || || |
//  ^
static void drawColumnHeader(Cellulose *spreadsheet, char* cell);
static void drawRowHeader(Cellulose *spreadsheet, char* cell);

// takes in the client and renders its contents
// the entire client is needed and not just the spreadsheet because there is other info needed
// like the cursor position
static void renderSpreadsheet(Cellulose *client, char* row_cell) {
    attron(COLOR_PAIR(COLUMN_CELL_ID));
    drawColumnHeader(client, row_cell);
    attron(COLOR_PAIR(ROW_CELL_ID));
    drawRowHeader(client, row_cell);
    move(0,0);
    printw("               ");
    // render all of the cells
    for (size_t rows = 0; rows < client->range_y_max - client->range_y_min; ++rows ) {
        // this means the rows doesn't exist and should just be empty cells
        if (rows + client->range_y_min > VEC_LEN(client->spread_sheet)) {
            attron(COLOR_PAIR(STR_CELL_ID));
            for (int empty_row = 0; empty_row < client->range_x_max - client->range_x_min; ++empty_row)
                print_cell((empty_row + 1) * 15, rows + 1, row_cell, "");
            continue;
        }
        // else iterate over the cells in the rows
        for (size_t column = 0; column < client->range_x_max - client->range_x_min; ++column) {
            // then check if the cell is out of bounds in the row
            if (column < ROW_P(rows).length) {
                // this moves where the text should be shown on the TUI
                move(rows + 1,(column + 1) * 15);
                // this checks the cell's value's type which changes how the cell looks depending on the type
                // formatting can be found in config.h
                if (CELL_P(rows, column).cell_type == t_str)
                    attron(COLOR_PAIR(STR_CELL_ID));
                else
                    attron(COLOR_PAIR(INT_CELL_ID));
                // finally print the cell's value
                printw("%s",CELL_P(rows, column).displayed_value);
            }
            // if it is then just print an empty cell
            else
                print_cell((column + 1) * 15, rows + 1, row_cell, "");
        }
    }
}

static void drawColumnHeader(Cellulose *spreadsheet, char* cell) {
    for (int column = 0; column < spreadsheet->range_x_max - spreadsheet->range_x_min; ++column)
        print_cell((column + 1) * 15, 0, cell, "A");

}
static void drawRowHeader(Cellulose *spreadsheet, char* cell) {
    for (int row = 0; row < spreadsheet->range_y_max - spreadsheet->range_y_min; ++row)
        print_cell(0, row + 1, cell, "B" );
}

