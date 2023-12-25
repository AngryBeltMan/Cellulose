#include "../cursor/cursor_include.h"
#include "../client.h"
#include "../str.h"
#define DELETE_KEY 127
//sets all of the selected cell to a certain value
static int setSelectedCells(Cellulose *client, ushort x, ushort y, bool exists, void* cell_input) {
    str input = strClone((str*)cell_input).string;
    if (exists) {
        /* if (CELL_P(y, x).cell_type == t_str) */
        /*     free(CELL_P(y, x).cell_value.str); */
    }
    setCell(client, x, y, &input );
    return 0;
}
static int inputModeParseKey(Cellulose *client, cursor* cursor, int input, str *cell_input) {
    switch (input) {
        case 10: {
            cursor->mode = NORMAL_MODE;
            // checks to see if there are value in the cell being replaced and frees them if there are
            if (cellExist(client, cursor->x, cursor->y )) {
                free(CUR_CELL_P.displayed_value);
                // free the actual value if the cell type is str because that is stored on the heap
                if (CUR_CELL_P.cell_type == t_str)
                    free(CUR_CELL_P.cell_value.str);
            }
            // sets all of the cells selected to the inputed value
            if (cursor->visual_state != visual_state_NONE)
                iterSelectedCells(client, cursor, &setSelectedCells, cell_input),
                client->redraw_spreadsheet = true,
                cursor->visual_state = visual_state_NONE;
            else
                setCell(client, cursor->x, cursor->y, cell_input);
        } break;
        case DELETE_KEY:
             clear();
             client->redraw_spreadsheet = true;
             client->redraw_dividers = true;
             popFront(cell_input);
             break;
        default: {
            if (pushChar(cell_input, input ) == -1)
                exit(1); // right now just exit with an error
        } break;
    }
    return 0;
}
