#include "tests.h"
#include "../src/client.h"
#include <stdio.h>
#include <stdlib.h>
int testAppendingWholeNum() {
    cell_t cell;
    cell.displayed_value = malloc(CELL_LEN);
    setCellWhole(&cell, 32);
    printf("%s\n", cell.displayed_value);
    T_ASSERT(strcmp(cell.displayed_value, " 32            ") == 0);
    return T_SUCCESS;
}
