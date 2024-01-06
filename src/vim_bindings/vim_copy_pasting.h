#pragma once
#include "../cursor/cursor_include.h"
#include "../client.h"
#include "vim_cursor_movement.h"

#define COPY_PASTE_CASES() \
        case 'd':  parseKeyDY(client, cursor, 'd'); break;  \
        case 'y':  parseKeyDY(client, cursor, 'y'); break;  \
        case 'p':  parseKeyP(client, cursor);  break;

static int addCellToClipboard(cursor_t* cursor, cell_t cell, size_t clipboard_row) {
    str contents;
    if (cell.cell_type == t_str) {
        str_res res = fromCharArray(cell.cell_value.str);
        if (res.result == -1)
            return -1;
        contents = res.string;
    } else {
        str_res res = fromDouble(cell.cell_value.number);
        if (res.result == -1)
            return -1;
        contents = res.string;
    }
    // new line is the seperator
    pushChar(&contents, '\n');
    appendStr(&cursor->clipboard.elements[clipboard_row], &contents);
    free(contents.contents);
    return 0;
}

static int deleteCells(Cellulose *client,unsigned short x, unsigned short y, bool exist, void* arg_cursor) {
    cursor_t* cur = (cursor_t*)arg_cursor;
    // calculates the greater and smaller y value between the cursor y position and the cells y position in the spreadsheet
    size_t min_y =  ((cur->select_pos_y >= y) * y) + ((cur->select_pos_y < y) * cur->select_pos_y);
    size_t max_y = (cur->select_pos_y + y) - min_y;

    // calculates where this cell should be in the clipboard
    size_t clipboard_row_index = cur->visual_state == visual_state_COLUMN_SELECT ? y : max_y - min_y;

    // Adds rows to the clipboard vector so that when the cell is appended to the clipboard the row will exist in the vector.
    clipboardAddRowsTo(cur, clipboard_row_index);

    if (exist && CELL_P(y, x).cell_type != t_empty) {
        free(CELL_P(y, x).displayed_value);
        addCellToClipboard(cur, CELL_P(y, x), clipboard_row_index);
        CELL_P(y,x).cell_type = t_empty;
    }
    return 0;
}

static int copyCells(Cellulose *client,unsigned short x, unsigned short y, bool exist, void* arg_cursor) {
    cursor_t* cur = (cursor_t*)arg_cursor;
    size_t min_y =  ((cur->select_pos_y >= y) * y) + ((cur->select_pos_y < y) * cur->select_pos_y);
    size_t max_y = (cur->select_pos_y + y) - min_y;

    // calculates where this cell should be in the clipboard
    size_t clipboard_row_index = cur->visual_state == visual_state_COLUMN_SELECT ? y : max_y - min_y;

    clipboardAddRowsTo(cur, clipboard_row_index);

    if (exist && CELL_P(y, x).cell_type != t_empty)
        addCellToClipboard(cur, CELL_P(y, x), clipboard_row_index);
    return 0;
}
// parses either key d or y cause they are very similar
static int parseKeyDY(Cellulose* client, cursor_t* cursor, char key) {
    if (cursor->clipboard.length > 0)
        clearClipboard(cursor),
        cursor->clipboard = (struct clipboardvec)VEC_NEW(str);
    if (cursor->previous_char == key ) {
        cursor->visual_state = visual_state_ROW_SELECT;
        cursor->select_pos_y = cursor->y;
    } else if (cursor->previous_char == 'r') {
        cursor->visual_state = visual_state_COLUMN_SELECT;
        cursor->select_pos_x = cursor->x;
    } else if (cursor->visual_state == visual_state_NONE)
        return 0;
    if (key == 'd')
        iterSelectedCells(client, cursor, &deleteCells, cursor);
    else
        iterSelectedCells(client, cursor, &copyCells, cursor);
    client->redraw_spreadsheet = true;
    cursor->visual_state = visual_state_NONE;
    return 0;
}

static int pasteClipboard(Cellulose* client, cursor_t* cursor) {
    client->redraw_spreadsheet = true;
    VEC_ITER(cursor->clipboard, str, clipboard_value) {
        size_t offset = 0;
        size_t column_index = 0;
        char* current_value = strtok(clipboard_value.contents, "\n");
        while (current_value != NULL) {
            size_t cell_x = cursor->x + column_index;
            size_t cell_y = cursor->y + (clipboard_value_index - 1);

            size_t len = strlen(current_value);
            offset += len + 1;
            // creates a copy of the pasted value
            str_res pasted_value;
            if ((pasted_value = fromCharArray(current_value)).result == -1)
                return -1;
            if (cellExist(client, cell_x, cell_y))
                freeCell(CELL_P(cell_y, cell_x));

            setCell(client, cell_x, cell_y, &pasted_value.string);
            ++column_index;
            // restore the clipboard back to its normal value
            clipboard_value.contents[offset - 1] = '\n';
            current_value = strtok(NULL, "\n");
        }
    }
    return 0;
}

static void parseKeyP(Cellulose* client, cursor_t* cursor) {
    size_t repeat_amount = (cursor->repeat_count + (cursor->repeat_count == 0));
    for (size_t repeat_count = 0; repeat_count < repeat_amount; ++repeat_count) {
        pasteClipboard(client, cursor);
        // move the cursor down so it is just below the text it pasted
        MOVE_CURSOR(cursor->clipboard.length, 0, cursor->clipboard.length, touchingBottomEdge(client, cursor->y), y);
    }
    cursor->repeat_count = 0;
    client->redraw_spreadsheet = true;
}

