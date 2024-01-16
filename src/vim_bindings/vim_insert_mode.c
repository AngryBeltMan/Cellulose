#include "vim_insert_mode.h"
#define DELETE_KEY 127
int setSelectedCells(Cellulose *client, coord_int_t x, coord_int_t y, bool exists, void *cell_input) {
    str input = strClone((str*)cell_input).string;
    if (exists && CELL_P(y, x).cell_type != t_empty) {
        free(CELL_P(y, x).displayed_value);
        if (CELL_P(y, x).cell_type == t_str)
            free(CELL_P(y, x).cell_value.str);
    }
    setCell(client, x, y, &input );
    return 0;
}
int parseCliInput(Cellulose *client, cursor_t *cursor, int input, str *cell_input) {
    switch (input) {
        case 10: {
            if (cursor->mode == COMMAND_MODE) {
                cursor->mode = NORMAL_MODE;
                int res = runCommand(client, cursor, cell_input->contents);
                free(cell_input->contents);
                cell_input->contents = NULL;
                return res;
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
        case 27:
            cursor->mode = NORMAL_MODE;
            free(cell_input->contents);
            cell_input->contents = NULL;
            break;
        default: {
            if (pushChar(cell_input, input ) == -1)
                exit(1); // right now just exit with an error
        } break;
    }
    return 0;
}
