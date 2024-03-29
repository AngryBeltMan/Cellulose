#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <sys/types.h>

#include "cell.h"
#include "client.h"
#include "config.h"
#include "config_include.h"
#include "str.h"
#include "cursor/cursor_include.h"
#include "vec.h"

Cellulose newEmpty(const char* file_name) {
    spreadsheet_t sheet = (spreadsheet_t)VEC_NEW(row_t);
    assert(sheet.elements && "ERROR: failed to allocate memory for cellulose vector");
    return (Cellulose ) {
        .pos_x = 0,
        .pos_y = 0,
        .spread_sheet = sheet,
        .redraw_dividers = true,
        .redraw_spreadsheet = true,
        .redraw_cli = true,
        .redraw_row_header = true,
        .redraw_column_header = true,
        .name = file_name,
    };
}

// checks to see if a certain cell coordinate exists in the spreadsheet
inline bool cellExist(Cellulose *client, size_t x, size_t y) {
    return  rowExist(y) ? (x < ROW_P(y).length): false;
}

// deconstructor for struct Cellulose
void freeSpreadsheet(Cellulose cellulose) {
    // iterates over all of the rows
    VEC_ITER(cellulose.spread_sheet, row_t, sheet_row) {
        // iterates all of the cells in the row
        VEC_ITER(sheet_row, cell_t, row_cell) {
            freeCell(row_cell);
            if (row_cell.displayed_value)
                free(row_cell.displayed_value);
        }
        VEC_FREE(sheet_row); // frees the pointer to the rows
    }
    VEC_FREE(cellulose.spread_sheet);
}

// Iterates over all of the cells that are selected by the cursor and calls the function pointer "fn" with the coordinate of the cell being the argument.
// Function pointer should take two args the first being the x coordinate of the cell, the y coordingate of the cell, and finally a boolean that specifies if that cell exists in the spreadsheet.
// Finally, the function pointer should return an integer with zero indicating a success.
int iterSelectedCells(Cellulose* client, cursor_t* cursor, selected_fn_t fn, void* fn_arg) {
    coord_int_t min_x = (cursor->select_pos_x < cursor->x) ? cursor->select_pos_x : cursor->x;
    coord_int_t max_x = (cursor->select_pos_x > cursor->x) ? cursor->select_pos_x : cursor->x;

    coord_int_t from_y = (cursor->select_pos_y < cursor->y) ? cursor->select_pos_y : cursor->y;
    coord_int_t to_y = (cursor->select_pos_y > cursor->y) ? cursor->select_pos_y : cursor->y;
    if (cursor->visual_state == visual_state_COLUMN_SELECT)
        from_y = 0,
        to_y = SHEET_P.length;

    for (;from_y <= to_y; ++from_y) {
        int from_x = (cursor->visual_state != visual_state_ROW_SELECT) * min_x;
        int to_x = (cursor->visual_state == visual_state_ROW_SELECT) ? (rowExist(from_y) ? (coord_int_t)ROW_P(from_y).length : 0): max_x + 1;
        for (; from_x < to_x; ++from_x) {
            // cast the void pointer into a function and then call it with the necessary arguments
            // fn_arg is an option argument the function can take. NULL can be passed if this argument isn't needed.
            bool exists = cellExist(client, from_x, from_y);
            int res = ((int (*)(Cellulose* ,unsigned short,unsigned short, bool, void* arg))fn)(client,from_x, from_y, exists, fn_arg);
            if (res != 0)
                return -1;
        }
    }
    return 0;
}

// sets the cell.cell_value
// used for the function setCell
inline void setCellValue(Cellulose *client, coord_int_t x, coord_int_t y, str* cell_input) {
    if (isNum(cell_input))
        CELL_P(y, x).cell_type = t_int,
        CELL_P(y, x).cell_value.number = atof(cell_input->contents),
        // free it here because the client will not be able to
        free(cell_input->contents);
    else
        CELL_P(y, x).cell_type = t_str,
        CELL_P(y, x).cell_value.str = cell_input->contents;
}

// Adds rows to the spreadsheet until the number of rows is equal to the argument y. Creates no rows if the number of rows is equal to or greater than y.clsi
inline void createRowsTo(Cellulose *client, size_t y) {
    size_t rs = client->spread_sheet.length;
    for (; rs <= y; ++rs) {
        row_t r = (row_t)VEC_NEW(cell_t);
        VEC_APPEND(SHEET_P, r)
    }
}

// Adds columns to a certain row until the number of columns in that row is equal to the argument x. Creates no columns if the number of columns in the row is equal to or greater than x.
inline int createColumnsTo(Cellulose *client, size_t row_index, size_t x) {
    size_t column = ROW_P(row_index).length;
    for (; column <= x; ++column) {
        cell_t empty_cell = createCellEmpty();
        VEC_APPEND(ROW_P(row_index), empty_cell);
    }
    return 0;
}

int cellCreateEmptyDisVal(Cellulose *client, coord_int_t x, coord_int_t y, bool b_exist) {
    if (!b_exist) {
        createRowsTo(client, y);
        createColumnsTo(client, y, x);
        if (( CELL_P(y, x).displayed_value = malloc(CELL_LEN)) == NULL)
            return -1;
    // cell type empty doesn't have displayed_value allocated which means we will need to allocate a new one
    } else if (CELL_P(y, x).cell_type == t_empty) {
        if (( CELL_P(y, x).displayed_value = malloc(CELL_LEN)) == NULL)
            return -1;
    }
    return 0;
}

// sets the cell at the cursor x and y to the value inside cell_input
inline int setCell(Cellulose *client, coord_int_t x, coord_int_t y, str* cell_input) {
    cellCreateEmptyDisVal(client, x, y, cellExist(client, x, y));
    create_cell(CELL_P(y, x).displayed_value, cell_input->contents, cell_input->len);
    setCellValue(client, x, y, cell_input);
    return 0;
}
