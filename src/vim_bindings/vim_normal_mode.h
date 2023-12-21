#include "../client.h"
#include "../cell.h"
#include "vim_cursor_movement.h"
#include "command_repeat.h"
static str_res cellValueAsStr(Cellulose* client, cursor* cursor );
// parses the key input in normal mode
static int normalModeParseKey(Cellulose* client, cursor* cursor, str* cell_input, int input) {
    switch (input) {
        CURSOR_MOVEMENT_CASES()
        REPEAT_CASES()
        case 'i': {
            cursor->mode = INSERT_MODE;
            clear();
            str_res res;
            if (( res = strNew()).result == -1)
                return -1;
            *cell_input = res.string;
        } break;
        case 'a': {
            cursor->mode = INSERT_MODE;
            clear();
            str_res res;
            if (( res = cellValueAsStr(client, cursor)).result == -1)
                return -1;
            *cell_input = res.string;
        } break;
        case 'v': {
            cursor->select_pos_x = cursor->x;
            cursor->select_pos_y = cursor->y;
        } break;
    }
    // input will be between 0 to 255 so there should be no undefined behavior
    cursor->previous_char = input;

    return 0;
}
static str_res cellValueAsStr(Cellulose* client, cursor* cursor ) {
    str_res result;
    // if the cell doesn't exist return a new empty string
    if (!cellExist(client, cursor->x, cursor->y))
        return strNew();
    // parses the cell value different;y according to the cell's type
    if (CUR_CELL_P.cell_type == t_int)
        result = fromDouble(CUR_CELL_P.cell_value.number);
    else
        result = fromCharArray(CUR_CELL_P.cell_value.str);
    return result;
}
