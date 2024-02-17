#include "cell.h"
#include "config.h"
#include "config_include.h"
#include "messages.h"
#include "str.h"

#define VAL_LEN floor(log10(value)) + 1

inline void freeCell(cell_t cell) {
    if (cell.cell_type == t_empty)
        return;
    if (cell.cell_type == t_str)
        free(cell.cell_value.str);
    cell.cell_type = t_empty;
}

inline void print_cell(int x, int y, char *output, char *value) {
    move(y,x);
    memset(output, ' ', 10);
    output[11] = '\0';
    size_t len = strlen(value);
    memmove(output + 1, value, len < 11 ? len : 11);
    printw("%s%s", output,len > 12 ? " .." : "     ");
}

inline void create_cell(char *output, char *value, size_t value_len) {
    memset(output, ' ', 12);
    memcpy(output + 1, value, value_len < 12 ? value_len : 12);
    memcpy(output + 12, value_len > 12 ? ".." : "  ", 3);
}

int setCellFloat(cell_t *p_cell, cell_num_t value) {
    // find the amount of number places value has. Plus 7 because the displayed value will show 7 decimal places worth of values.
    if (VAL_LEN + 6 > CELL_LEN - 2 ) {
        snprintf(p_cell->displayed_value + 1, CELL_LEN - 2, "%f", value );
        // and the .. to end of the displayed value to symbolize there is more to cells value
        p_cell->displayed_value[CELL_LEN - 2] = '.';
        p_cell->displayed_value[CELL_LEN - 1] = '.';
    } else {
        sprintf(p_cell->displayed_value + 1, "%f", value );
        p_cell->displayed_value[strlen(p_cell->displayed_value)] = ' ';
    }
    p_cell->cell_type = t_int;
    return 0;
}
cell_t createCellEmpty() {
    return (cell_t) {
        .displayed_value = NULL,
        .cell_type = t_empty
    };
}
// Wrapper function for macro SET_CELL_NUM. Sets a cells value based on a given coordinate value.
// Arguments:
// p_cell - Pointer to the cell that will be modified to now hold the value of the argument value.
//    P_cell must have the attribute displayed_value allocated.
//    If p_cell was a str the inner value must be freed beforehand as the doesn't deallocate any memory.
// value - The value argument p_cell will hold after the function is called.
int setCellWhole(cell_t *p_cell, int value) {
    if (VAL_LEN > CELL_LEN - 2 ) {
        snprintf(p_cell->displayed_value + 1, CELL_LEN - 2, "%i", value );
        // and the .. to end of the displayed value to symbolize there is more to cells value
        p_cell->displayed_value[CELL_LEN - 2] = '.';
        p_cell->displayed_value[CELL_LEN - 1] = '.';
    } else {
        sprintf(p_cell->displayed_value + 1, "%i", value );
        // get rid of the extra escape char from sprintf
        p_cell->displayed_value[(int)(VAL_LEN) + 1] = ' ';
    }
    p_cell->cell_type = t_int;
    return 0;
}
