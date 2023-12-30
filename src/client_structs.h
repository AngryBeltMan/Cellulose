#pragma once
#include "vec.h"
#include "cell.h"

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
    uint16_t pos_x,pos_y;
    // the spreadsheet
    spreadsheet_t spread_sheet;
    // when true the corresponding elements will be redrawn
    bool redraw_dividers, redraw_spreadsheet;
} Cellulose;
