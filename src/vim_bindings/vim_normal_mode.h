#include "../client.h"
#include "../cell.h"
#include "vim_copy_pasting.h"
#include "vim_cursor_movement.h"
#include "vim_visual_transitions.h"
#include "command_repeat.h"
#include "../config_include.h"
#include <stdio.h>
#include <string.h>
static str_res cellValueAsStr(Cellulose* client, cursor_t* cursor );
// parses the key input in normal mode
static int normalModeParseKey(Cellulose* client, cursor_t* cursor, str* cell_input, int input) {
    switch (input) {
        CURSOR_MOVEMENT_CASES()
        COPY_PASTE_CASES()
        VISUAL_TRANS_CASES()
        REPEAT_CASES()
        case 'i': {
            cursor->mode = INSERT_MODE;
            move(CLIENT_SHEET_HEIGHT + 1, 0);
            printw(COLUMN_HEADER_BG);
            client->redraw_cli = true;
            str_res res;
            if (( res = strNew()).result == -1)
                return -1;
            *cell_input = res.string;
        } break;
        case 'a': {
            cursor->mode = INSERT_MODE;
            move(CLIENT_SHEET_HEIGHT + 1, 0);
            printw(COLUMN_HEADER_BG);
            client->redraw_cli = true;
            str_res res;
            if (( res = cellValueAsStr(client, cursor)).result == -1)
                return -1;
            *cell_input = res.string;
        } break;
        case 27: {
            cursor->visual_state = visual_state_NONE;
            cursor->mode = NORMAL_MODE;
            // get rid of all of the purple selected cells
            client->redraw_spreadsheet = true;
        } break;
        case '/':
        case ':':
            cursor->mode = COMMAND_MODE;
            move(CLIENT_SHEET_HEIGHT + 1, 0);
            printw(COLUMN_HEADER_BG);
            client->redraw_cli = true;
            str_res res;
            if (( res = strNew()).result == -1)
                return -1;
            *cell_input = res.string;
            break;
    }
    // input will be between 0 to 255 so there should be no undefined behavior
    cursor->previous_char = input;
    return 0;
}
static str_res cellValueAsStr(Cellulose* client, cursor_t* cursor ) {
    str_res result;
    // if the cell doesn't exist return a new empty string
    if (!cellExist(client, cursor->x, cursor->y) || CUR_CELL_P.cell_type == t_empty)
        return strNew();
    // parses the cell value different;y according to the cell's type
    if (CUR_CELL_P.cell_type == t_int)
        result = fromDouble(CUR_CELL_P.cell_value.number);
    else
        result = fromCharArray(CUR_CELL_P.cell_value.str);
    return result;
}
