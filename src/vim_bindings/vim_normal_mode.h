#include "../client.h"
#include "../cell.h"
#include "vim_cursor_movement.h"
#include "vim_visual_transitions.h"
#include "command_repeat.h"
#include <string.h>
static int deleteCells(Cellulose *client,unsigned short x, unsigned short y, bool exist, void* arg) {
    if (exist) {
        freeCell(CELL_P(y,x));
        CELL_P(y,x).cell_type = t_empty;
    }
    return 0;
}
static int parseKeyD(Cellulose* client, cursor* cursor) {
    if (cursor->previous_char == 'd' && rowExist(cursor->y + client->pos_y)) {
        deleteRow(client, cursor->y + client->pos_y);
        return 0;
    }
    if (cursor->visual_state == visual_state_NONE)
        return 0;
    iterSelectedCells(client, cursor, &deleteCells, NULL);
    client->redraw_spreadsheet = true;
    cursor->visual_state = visual_state_NONE;
    return 0;

}
static str_res cellValueAsStr(Cellulose* client, cursor* cursor );
static int name(Cellulose *client,unsigned short x, unsigned short y, bool exist, void* arg) {
    if (!exist) {
        createRowsTo(client, y);
        createColumnsTo(client, y, x);
    }
    strcpy(CELL_P(y,x).displayed_value, " hey          ");
    return 0;
}
// parses the key input in normal mode
static int normalModeParseKey(Cellulose* client, cursor* cursor, str* cell_input, int input) {
    switch (input) {
        CURSOR_MOVEMENT_CASES()
        VISUAL_TRANS_CASES()
        REPEAT_CASES()
        case 'i': {
            cursor->mode = INSERT_MODE;
            // we clear the screen so we must redraw everything that was cleared
            client->redraw_dividers = true;
            client->redraw_spreadsheet = true;
            // clear the previous items on the screen
            clear();
            str_res res;
            if (( res = strNew()).result == -1)
                return -1;
            *cell_input = res.string;
        } break;
        case 'a': {
            cursor->mode = INSERT_MODE;
            client->redraw_dividers = true;
            client->redraw_spreadsheet = true;
            clear();
            str_res res;
            if (( res = cellValueAsStr(client, cursor)).result == -1)
                return -1;
            *cell_input = res.string;
        } break;
        case 'b': {
            iterSelectedCells(client, cursor, &name, NULL);
            client->redraw_spreadsheet = true;
            cursor->visual_state = visual_state_NONE;
        } break;
        case 'y': {
        } break;
        case 'd': {
            parseKeyD(client, cursor);
        } break;
    }
    // input will be between 0 to 255 so there should be no undefined behavior
    cursor->previous_char = input;

    return 0;
}
static str_res cellValueAsStr(Cellulose* client, cursor* cursor ) {
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
