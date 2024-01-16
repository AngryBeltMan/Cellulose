#pragma once
#include <ncurses.h>

#define CMD_MAP_SIZE 15
// text color id for cell type str
#define STR_CELL_ID 1

// text color id for cell type str
#define INT_CELL_ID 6

// text color id for the color of the column cells (X coordinate)
#define COLUMN_CELL_ID 2

// text color id for the color of the row cells (Y coordinate)
#define ROW_CELL_ID 3

// the text color id for the color when the cursor is hovering over that cell
#define CURSOR_CELL_ID 4

// the text color id for the color when the cursor is selecting many cells
#define SELECTED_CELL_ID 5

// the text color id for when the cursor is in normal mode
#define MODE_NORMAL_ID 7

// the text color id for when the cursor is in editing mode
#define MODE_EDIT_ID 8

// the text color id for when the cursor is in select mode
#define MODE_SELECT_ID 9

// the text color id for displaying the bg and fg of the cursor mode
#define CURSOR_MODE_ID 10

// the text color id for when general information is displayed to the user
#define INFO_MSG_ID 11

// the text color id for when an error occured with a command
#define ERROR_MSG_ID 12

#define GREY_ID 100
#define PURPLE_ID 101

#define EMPTY_CELL "              "

#define EMPTY_CELL_FULL "               "
#define CELL_LEN 14

#define COLUMN_HEADER_BG "                                                                                                                                                                                    "

#define CURSOR_COORD_FORMATTING "{%u, %u}"

// the type of integer that should be used to store the cells/cursor/etc coordinates
typedef uint16_t coord_int_t;
// the type of floating point number the cell will hold if it of type num
typedef double cell_num_t;
