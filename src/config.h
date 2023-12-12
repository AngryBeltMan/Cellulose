#pragma once
#include <ncurses.h>
// id for cell type str
#define STR_CELL_ID 1

// id for cell type str
#define INT_CELL_ID 6

// id for the color of the column cells (X coordinate)
#define COLUMN_CELL_ID 2

// id for the color of the row cells (Y coordinate)
#define ROW_CELL_ID 3

// the id for the color when the cursor is hovering over that cell
#define CURSOR_CELL_ID 4

// the id for the color when the cursor is selecting many cells
#define SELECTED_CELL_ID 5

// the id for when the cursor is in normal mode
#define MODE_NORMAL_ID 7

// the id for when the cursor is in editing mode
#define MODE_EDIT_ID 8

// the id for when the cursor is in select mode
#define MODE_SELECT_ID 9

#define GREY_ID 100
static void initCellColors() {
    // create new color grey
    init_color(GREY_ID, 750, 750, 750);

    // arg 1 foreground arg2 background
    init_pair(STR_CELL_ID, COLOR_BLACK, GREY_ID );

    init_pair(INT_CELL_ID, COLOR_BLUE, GREY_ID );

    init_pair(COLUMN_CELL_ID, COLOR_BLACK, COLOR_GREEN );

    init_pair(ROW_CELL_ID, COLOR_BLACK, COLOR_CYAN );

    init_pair(MODE_NORMAL_ID, COLOR_WHITE, COLOR_BLACK );

    init_pair(MODE_EDIT_ID, COLOR_BLACK, COLOR_CYAN );

    init_pair(MODE_SELECT_ID, COLOR_BLACK, COLOR_CYAN );

}
