#include "rand_cmds.h"
#include "../messages.h"
#include "../cell.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
// information for fillRandom_iter
struct randInfo {
    enum {
        // chooses a random number between x and y
        RT_range,
        // chooses a random integer
        RT_int,
        // chooses a random whole number between x and y
        RT_range_whole,
    } rand_type;
    double min;
    double max;
};

int fillRandom_iter(Cellulose *client, u_short x, u_short y, bool exist, void* arg) {
    if (arg == NULL)
        return -1;
    struct randInfo *info_p = (struct randInfo*)arg;
    double num;
    switch (info_p->rand_type) {
        case RT_int:
            num = rand();
            break;
        case RT_range:
            num = (((double)rand()/RAND_MAX) * (info_p->max - info_p->min)) + info_p->min;
            break;
        case RT_range_whole:
            num = (rand() % (int)(info_p->max - info_p->min)) + info_p->min;
            break;
    }
    if (!exist) {
        createRowsTo(client, y);
        createColumnsTo(client, y, x);
        if (( CELL_P(y, x).displayed_value = calloc(CELL_LEN, 1)) == NULL)
            return -1;
        CELL_P(y, x).displayed_value[0] = ' ';
    }
    // free the old value if it is a string,0 as it is not in use and it is heap allocated
    if (CELL_P(y, x).cell_type == t_str)
        free(CELL_P(y, x).cell_value.str);
    CELL_P(y, x).cell_type = t_int;
    CELL_P(y, x).cell_value.number = num;
    if (floor(log(num) + 7) > CELL_LEN -2 ) {
        snprintf(CELL_P(y, x).displayed_value + 1, CELL_LEN - 2, "%f", num );
        // and the .. to end of the displayed value to symbolize there is more to cells value
        CELL_P(y, x).displayed_value[CELL_LEN - 2] = '.';
        CELL_P(y, x).displayed_value[CELL_LEN - 1] = '.';
    } else {
        sprintf(CELL_P(y, x).displayed_value + 1, "%f", num );
        CELL_P(y, x).displayed_value[strlen(CELL_P(y, x).displayed_value)] = ' ';
    }
    return 0;
}
int fillRandomRange(Cellulose *client, cursor_t *cursor, long double *args) {
    // should not occur
    if (args == NULL)
        return -1;
    // some cells need to be selected as we iterate over the selected cells assigning them a random number
    if (cursor->visual_state == visual_state_NONE) {
        LOG_INFO("%s", "NO CELLS SELECTED!");
        return 0;
    }
    double min = args[0];
    double max = args[1];
    if (min > max)  {
        LOG_ERROR("%s", "MINIMUM NUMBER COMES FIRST")
        return 0;
    }
    struct randInfo info = {
        .min = min,
        .max = max,
        .rand_type = RT_range
    };
    srand(time(NULL));
    iterSelectedCells(client, cursor, &fillRandom_iter, &info);
    cursor->visual_state = visual_state_NONE;
    client->redraw_spreadsheet = 1;

    return 0;
}
int fillRandom(Cellulose *client, cursor_t *cursor,  __attribute_maybe_unused__ long double *args) {

    // some cells need to be selected as we iterate over the selected cells assigning them a random number
    if (cursor->visual_state == visual_state_NONE) {
        LOG_INFO("%s", "NO CELLS SELECTED!");
        return 0;
    }
    struct randInfo info;
    info.rand_type  = RT_int;
    // seed the random number with the unix time in seconds
    srand(time(NULL));
    iterSelectedCells(client, cursor, &fillRandom_iter, &info);

    cursor->visual_state = visual_state_NONE;
    // redraw the updated cells
    client->redraw_spreadsheet = 1;
    return 0;
}
int fillRandomWholeRange(Cellulose *client, cursor_t *cursor, long double *args) {
    // should not occur
    if (args == NULL)
        return -1;
    // some cells need to be selected as we iterate over the selected cells assigning them a random number
    if (cursor->visual_state == visual_state_NONE) {
        LOG_INFO("%s", "NO CELLS SELECTED!");
        return 0;
    }
    // seed the random number with the unix time in seconds
    srand(time(NULL));
    // floor because we can't have a value that is a decimal
    double min = floor(args[0]);
    double max = floor(args[1]);

    if (min > max)  {
        LOG_ERROR("%s", "MINIMUM NUMBER COMES FIRST")
        return 0;
    }
    struct randInfo info = {
        .min = min,
        .max = max,
        .rand_type = RT_range_whole
    };
    iterSelectedCells(client, cursor, fillRandom_iter, &info);
    cursor->visual_state = visual_state_NONE;
    // redraw the updated cells
    client->redraw_spreadsheet = 1;
    return 0;
}
