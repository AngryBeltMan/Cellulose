#pragma once
#include "config.h"
#include "config_include.h"
#include "vec.h"
#include "cell.h"
#include "str.h"
#include "cursor/cursor_include.h"

#define ELEMENTS client.spread_sheet.elements
#define ELEMENTS_P client->spread_sheet.elements
#define SHEET_P client->spread_sheet
#define SHEET client.spread_sheet
#define SHEET_LEN client->spread_sheet.length

#define rowExist(_y) (_y < SHEET_P.length)

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
    coord_int_t pos_x,pos_y;
    // the spreadsheet
    spreadsheet_t spread_sheet;
    // when true the corresponding elements will be redrawn
    bool redraw_dividers, redraw_spreadsheet, redraw_cli, redraw_row_header, redraw_column_header;
    // exit the spreadsheet if true
    bool should_exit;
    // check if updates have been made but not saved
    bool has_saved;
    // check if updates have been made but not saved
    // Name of the file. Used for saving the file.
    const char* name;
} Cellulose;

typedef int (* selected_fn_t)(Cellulose* ,unsigned short,unsigned short, bool, void* arg);

// Constructor for struct Cellulose. All of the attributes have their own "default" value.
Cellulose newEmpty(const char* file_name);

// Checks to see if a cell in the spreadsheet lies on a given coordinate value.
bool cellExist(Cellulose *client, size_t x, size_t y);

// writes to a given file with arg seperator seperating each value
int saveSpreadsheet(Cellulose *client);

void freeSpreadsheet(Cellulose cellulose);

int iterSelectedCells(Cellulose *client, cursor_t* cursor, selected_fn_t fn, void* fn_arg);

void setCellValue(Cellulose *client, coord_int_t x, coord_int_t y, str* cell_input);

void createRowsTo(Cellulose *client, size_t y);

int createColumnsTo(Cellulose *client, size_t row_index, size_t x);

int setCell(Cellulose *client, coord_int_t x, coord_int_t y, str* cell_input);

// Sets a given cell with an empty display value, if it doesn't exist or is of type empty. If the cell doesn't exist it will add that cell to the spreadsheet.
// The displayed value will be heap allocated but can be freed with freeCell or freeSpreadsheet.
// Arguments:
// client - Used to set the given cell with a new empty display value.
// x, y - Coordinate values on which cell should have an empty display value.
// b_exist - Boolean value that specifies whether or not the cell exist. Nothing will be modified if b_exist is true.
int cellCreateEmptyDisVal(Cellulose *client, coord_int_t x, coord_int_t y, bool b_exist);
