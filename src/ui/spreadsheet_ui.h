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
static void drawColumnHeader(Cellulose *spreadsheet);
static void drawRowHeader(Cellulose *spreadsheet);
static void drawSpreadsheetDividers(bool *redraw);
static inline void renderSingleCell(Cellulose* client, cursor_t* cursor, const size_t x, const size_t y);

// render a single spreadsheet's row
static void renderSingleRow(Cellulose *spreadsheet, cursor_t* cursor, const size_t row_index);

// takes in the client and renders its contents
// the entire client is needed and not just the spreadsheet because there is other info needed
// like the cursor position
static void renderSpreadsheet(Cellulose *client, cursor_t* cursor) {
    if (!client->redraw_spreadsheet)
        return;
    client->redraw_spreadsheet = false;
    // draws the row and column header
    attron(COLOR_PAIR(COLUMN_CELL_ID));
    drawColumnHeader(client);
    attron(COLOR_PAIR(ROW_CELL_ID));
    drawRowHeader(client);
    move(0,0);
    // the fill for the block at (0,0)
    attron(COLOR_PAIR(STR_CELL_ID));
    printw(EMPTY_CELL_FULL);
    // iterates over all of the rows, then renders each of the rows cells
    for (size_t rows = 0; rows < CLIENT_SHEET_HEIGHT; ++rows )
        renderSingleRow(client, cursor, rows);

}

static void drawColumnHeader(Cellulose *spreadsheet) {
    // clear the column of its original values
    move(0,0);
    attron(COLOR_PAIR(COLUMN_CELL_ID));
    printw(COLUMN_HEADER_BG);
    for (int column = 0; column <= CLIENT_SHEET_WIDTH; ++column) {
        move(0, (column * 15) + 6);
        printw("%i", column + spreadsheet->pos_x);
    }

}
static void drawRowHeader(Cellulose *spreadsheet) {
    int row;
    // sets the background for all of the row header cells
    for (row = 0; row <= CLIENT_SHEET_HEIGHT; ++row) {
        move(row, 0);
        printw(EMPTY_CELL_FULL);
    }
    for (row = 0; row <= CLIENT_SHEET_HEIGHT; ++row) {
        // three is the right offset
        move(row, 3);
        printw("%i", row + spreadsheet->pos_y);
    }
}
// see if the cell is being selected by the user in visual mode
static bool isCellSelected(Cellulose *client, cursor_t* cursor, size_t cell_x, size_t cell_y) {
    switch (cursor->visual_state) {
        case visual_state_NONE:
            return false;
        case visual_state_FREE_POINT:
            return (((cell_y < cursor->select_pos_y) + (cell_y > cursor->y) == (cursor->select_pos_y > cursor->y) * 2) &&
                ((cell_x < cursor->select_pos_x) + (cell_x > cursor->x) == (cursor->select_pos_x > cursor->x) * 2));
        case visual_state_ROW_SELECT:
            // This is seperate from the return statement because we needs to make sure a row exists at index cell_y as the rhs of the && statement indexs the spreadsheet for that row.
            // TLDR: this if statement is sperate from the return statement else it would cause a segfault.
            if (!rowExist(cell_y))
                return false;
            return (cell_y < cursor->select_pos_y) + (cell_y > cursor->y) == ((cursor->select_pos_y > cursor->y) * 2) && (ROW_P(cell_y).length > cell_x);
        case visual_state_COLUMN_SELECT:
            return (cell_x < cursor->select_pos_x) + (cell_x > cursor->x) == (cursor->select_pos_x > cursor->x) * 2;
    }
}

static void renderSingleRow(Cellulose *client, cursor_t* cursor, const size_t row_index) {
    for (size_t column = 0; column < CLIENT_SHEET_WIDTH; ++column)
        renderSingleCell(client, cursor, column, row_index);
}

static inline void renderSingleCell(Cellulose* client, cursor_t* cursor, const size_t x, const size_t y) {
    // move where the text should be shown on the TUI
    move(y + 1,(x + 1) * 15);
    attron(COLOR_PAIR(STR_CELL_ID));
    // The second conditional will not segfault because of an index out of bounds because if a cell didn't exist it will just jump to draw_empty_celland not evaluate the other conditional.
    // This may later become an issue when compiler optimizations are enabled.
    if (!cellExist(client,  x + client->pos_x, y + client->pos_y) || CELL_P(client->pos_y + y, client->pos_x + x).cell_type == t_empty)
        goto draw_empty_cell;
    // this checks the cell's value's type which changes how the cell looks depending on the type
    // formatting can be found in config.h
    if (CELL_P(y + client->pos_y, x + client->pos_x).cell_type == t_int)
        attron(COLOR_PAIR(INT_CELL_ID));
    if (isCellSelected(client, cursor, x + client->pos_x, y + client->pos_y))
        attron(COLOR_PAIR(SELECTED_CELL_ID));
    printw("%s", CELL_P(y + client->pos_y, x + client->pos_x).displayed_value);
    return;
draw_empty_cell:
    if (isCellSelected(client, cursor, x + client->pos_x, y + client->pos_y))
        attron(COLOR_PAIR(SELECTED_CELL_ID));
    printw(EMPTY_CELL);
}

static void drawSpreadsheetDividers(bool *redraw) {
    if (!*redraw)
        return;
    *redraw = false;
    attron(COLOR_PAIR(STR_CELL_ID));
    for (int row = 1; row <= CLIENT_SHEET_HEIGHT; ++row)
        for (int column = 2; column <= CLIENT_SHEET_WIDTH + 1; ++column) {
            move(row, (column * 15) - 1);
            printw("|");
        }
}

