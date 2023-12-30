#pragma once
#define COPY_PASTE_CASES() \
        case 'd': { \
            parseKeyD(client, cursor); \
        } break; \
        case 'p': \
                  pasteClipboard(client, cursor); \
                  break;
#include "../cursor/cursor_include.h"
#include "../client.h"
// Copy an area of cells
static int deleteCells(Cellulose *client,unsigned short x, unsigned short y, bool exist, void* arg_cursor) {
    cursor* cur = (cursor*)arg_cursor;
    str contents;
    if (cur->clipboard.length <= y - cur->select_pos_y) {
        str_res empty_str;
        if ((empty_str = strNew()).result == -1)
            return -1;
        VEC_APPEND(cur->clipboard, empty_str.string);
    }
    if (exist && CELL_P(y, x).cell_type != t_empty) {
        free(CELL_P(y, x).displayed_value);
        if (CELL_P(y, x).cell_type == t_str) {
            contents = (str){
                .contents = CELL_P(y, x).cell_value.str,
                .len = strlen(CELL_P(y, x).cell_value.str)
            };
        } else {
            str_res res = fromDouble(CELL_P(y, x).cell_value.number);
            if (res.result == -1)
                return -1;
            contents = res.string;
        }
        // new line is the seperator
        pushChar(&contents, '\n');
        appendStr(&cur->clipboard.elements[y - cur->select_pos_y], &contents);
        free(contents.contents);
        CELL_P(y,x).cell_type = t_empty;
    }
    return 0;
}
static int parseKeyD(Cellulose* client, cursor* cursor) {
    if (cursor->clipboard.length > 0) {
        clearClipboard(cursor);
        cursor->clipboard = (struct clipboardvec)VEC_NEW(str);
    }
    if (cursor->previous_char == 'd' ) {
        cursor->visual_state = visual_state_ROW_SELECT;
        cursor->select_pos_y = cursor->y;
    } else if (cursor->previous_char == 'r') {
        cursor->visual_state = visual_state_COLUMN_SELECT;
        cursor->select_pos_x = cursor->x;
    } else if (cursor->visual_state == visual_state_NONE)
        return 0;
    iterSelectedCells(client, cursor, &deleteCells, cursor);
    client->redraw_spreadsheet = true;
    cursor->visual_state = visual_state_NONE;
    return 0;
}
static int pasteClipboard(Cellulose* client, cursor* cursor) {
    client->redraw_spreadsheet = true;
    VEC_ITER(cursor->clipboard, str, clipboard_value) {
        size_t offset = 0;
        size_t column_index = 0;
        char* current_value = strtok(clipboard_value.contents, "\n");
        while (current_value != NULL) {
            size_t len = strlen(current_value);
            offset += len + 1;
            // creates a copy of the pasted value
            str_res pasted_value;
            if ((pasted_value = fromCharArray(current_value)).result == -1)
                return -1;
            setCell(client, cursor->x + column_index, cursor->y + (clipboard_value_index - 1), &pasted_value.string);
            ++column_index;
            // restore the clipboard back to its normal value
            clipboard_value.contents[offset - 1] = '\n';
            current_value = strtok(NULL, "\n");
        }
    }
    return 0;
}

