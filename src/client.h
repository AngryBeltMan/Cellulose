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
    bool redraw_dividers, redraw_spreadsheet, redraw_cli;
    // exit the spreadsheet if true
    bool should_exit;
    // check if updates have been made but not saved
    bool has_saved;
    // Name of the file. Used for saving the file.
    const char* name;
} Cellulose;

typedef int (* selected_fn_t)(Cellulose* ,unsigned short,unsigned short, bool, void* arg);

// Constructor for struct Cellulose. All of the attributes have their own "default" value.
Cellulose newEmpty(const char* file_name);

// checks to see if a certain cell coordinate exists in the spreadsheet
bool cellExist(Cellulose *client, size_t x, size_t y);

// writes to a given file with arg seperator seperating each value
int saveSpreadsheet(Cellulose *client);

void freeSpreadsheet(Cellulose cellulose);

int iterSelectedCells(Cellulose *client, cursor_t* cursor, selected_fn_t fn, void* fn_arg);

void setCellValue(Cellulose *client, coord_int_t x, coord_int_t y, str* cell_input);

void createRowsTo(Cellulose *client, size_t y);

int createColumnsTo(Cellulose *client, size_t row_index, size_t x);

int setCell(Cellulose *client, coord_int_t x, coord_int_t y, str* cell_input);
