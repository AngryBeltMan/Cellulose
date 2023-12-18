#include "../cursor/cursor_include.h"
#include "../client.h"
#include "../str.h"
#define DELETE_KEY 127
static int parseInputEditMode(Cellulose *client, cursor* cursor, int input, str *cell_input) {
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
            setCell(client, cursor, cell_input);
        } break;
        case DELETE_KEY:
             clear();
             popFront(cell_input);
             break;
        default: {
            if (pushChar(cell_input, input ) == -1)
                exit(1); // right now just exit with an error
        } break;
    }
    return 0;
}
