#include "../commands/cmd.h"
#include "../cursor/cursor_include.h"
#include "../client.h"
#include "../str.h"
#include <curses.h>
#define DELETE_KEY 127
//sets all of the selected cell to a certain value
static int setSelectedCells(Cellulose *client, ushort x, ushort y, bool exists, void* cell_input) {
    str input = strClone((str*)cell_input).string;
    if (exists && CELL_P(y, x).cell_type != t_empty) {
        free(CELL_P(y, x).displayed_value);
        if (CELL_P(y, x).cell_type == t_str)
            free(CELL_P(y, x).cell_value.str);
    }
    setCell(client, x, y, &input );
    return 0;
}
// parses the input in cursor mode Command and Insert
static int parseCliInput(Cellulose *client, cursor_t* cursor, int input, str *cell_input) {
    switch (input) {
        case 10: {
            if (cursor->mode == COMMAND_MODE) {
                cursor->mode = NORMAL_MODE;
                return runCommand(client, cursor, cell_input->contents);
            }
            cursor->mode = NORMAL_MODE;
            // checks to see if there are value in the cell being replaced and frees them if there are
            if (cellExist(client, cursor->x, cursor->y ) && CUR_CELL_P.cell_type != t_empty ) {
                free(CUR_CELL_P.displayed_value);
                // free the actual value if the cell type is str because that is stored on the heap
                if (CUR_CELL_P.cell_type == t_str)
                    free(CUR_CELL_P.cell_value.str);
                CUR_CELL_P.cell_type = t_empty;
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
             move(CLIENT_SHEET_HEIGHT + 1, 0);
             printw(COLUMN_HEADER_BG);
             client->redraw_cli = true;
             popFront(cell_input);
             break;
        default: {
            if (pushChar(cell_input, input ) == -1)
                exit(1); // right now just exit with an error
        } break;
    }
    return 0;
}
