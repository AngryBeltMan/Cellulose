// contains the default constructer and destructor of the struct Cellulose
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <ncurses.h>
#include <stdlib.h>
#include "../client.h"
#include "../config_include.h"
// draws the cells that specify the x and y coordinate of the cells
// | ||A||B||C| <
// |1|| || || |
// |2| || || |
// |3|| || || |
//  ^
static void drawColumnHeader(Cellulose *spreadsheet, char* cell);
static void drawRowHeader(Cellulose *spreadsheet, char* cell);

// render a single spreadsheet's row
static void renderSingleRow(Cellulose *spreadsheet, const size_t row_index);

// checks to see if a cell exists at a given coordinate in the given spreadsheet
static bool cellExist(Cellulose *spreadsheet, size_t x, size_t y);

// takes in the client and renders its contents
// the entire client is needed and not just the spreadsheet because there is other info needed
// like the cursor position
static void renderSpreadsheet(Cellulose *client, char* row_cell) {
    // draws the row and column header
    attron(COLOR_PAIR(COLUMN_CELL_ID));
    drawColumnHeader(client, row_cell);
    attron(COLOR_PAIR(ROW_CELL_ID));
    drawRowHeader(client, row_cell);
    move(0,0);
    // the fill for the block at (0,0)
    attron(COLOR_PAIR(STR_CELL_ID));
    printw("               ");
    // iterates over all of the rows, then renders each of the rows cells
    for (size_t rows = 0; rows < CLIENT_SHEET_HEIGHT; ++rows )
        renderSingleRow(client, rows);

}

static void drawColumnHeader(Cellulose *spreadsheet, char* cell) {
    char* column_number = malloc(7);
    assert(column_number && "ERROR: failed to malloc on function drawColumnHeader");
    for (int column = 0; column < CLIENT_SHEET_WIDTH; ++column) {
        memset(column_number, 0x0, 7);
        snprintf(column_number, 7, "%i", column + spreadsheet->pos_x);
        print_cell((column + 1) * 15, 0, cell, column_number);
    }
    free(column_number);

}
static void drawRowHeader(Cellulose *spreadsheet, char* cell) {
    char* row_number = malloc(7);
    assert(row_number && "ERROR: failed to malloc on function drawColumnHeader");
    for (int row = 0; row < CLIENT_SHEET_HEIGHT; ++row) {
        memset(row_number, 0x0, 7);
        snprintf(row_number, 7, "%i", row + spreadsheet->pos_y);
        print_cell(0, row + 1, cell, row_number );
    }
    free(row_number);
}
static void renderSingleRow(Cellulose *client, const size_t row_index) {
    for (size_t column = 0; column < CLIENT_SHEET_WIDTH; ++column) {
        // this moves where the text should be shown on the TUI
        move(row_index + 1,(column + 1) * 15);
        if (cellExist(client,  column + client->pos_x, row_index + client->pos_y)) {
            // this checks the cell's value's type which changes how the cell looks depending on the type
            // formatting can be found in config.h
            if (CELL_P(row_index + client->pos_y, column + client->pos_x).cell_type == t_str)
                attron(COLOR_PAIR(STR_CELL_ID));
            else
                attron(COLOR_PAIR(INT_CELL_ID));
            printw("%s", CELL_P(row_index + client->pos_y, column + client->pos_x).displayed_value);
        } else
            printw("              |");
    }
}

