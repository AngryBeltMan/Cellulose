#include "rand_cmds.h"
#include "../messages.h"
#include "../cell.h"
#include "../vim_bindings/vim_copy_pasting.h"

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
/// enum that specifies what type of random number should be generated. I.E. a whole number, a floating point value.
enum rand_type {
    // chooses a random number between x and y
    RT_range,
    // chooses a random integer
    RT_int,
    // chooses a random whole number between x and y
    RT_range_whole,
};

// information for fillRandom_iter
struct randInfo {
    enum rand_type rand_type;
    double min;
    double max;
};
// Generates a random value given certain argments held inside argument info.
// Arguments:
// p_out - Pointer value where the random value will be stored. Cannot be null.
// info - Contains restrictions on the output of the random value. I.e. the range and if it should be a whole number.
static void genRandNum(cell_num_t *p_out, struct randInfo *p_info) {
    switch (p_info->rand_type) {
        case RT_int: *p_out = rand(); break;
        case RT_range:
            *p_out = (((double)rand()/RAND_MAX) * (p_info->max - p_info->min)) + p_info->min;
            break;
        case RT_range_whole:
            *p_out = (rand() % (int)(p_info->max - p_info->min)) + p_info->min;
            break;
    }
}
static int fillRandom_iter(Cellulose *client, coord_int_t x, coord_int_t y, bool exist, void* arg) {
    if (arg == NULL)
        return -1;
    struct randInfo *p_info = (struct randInfo*)arg;
    cell_num_t num;
    genRandNum(&num, p_info);
    bool b_whole_num = p_info->rand_type != RT_range;
    // allocates and sets the attribute cell_t.displayed_value
    cellCreateEmptyDisVal( client, x, y,  exist);
    // fills the displayed value with a bunch of spaces
    memset(CELL_P(y, x).displayed_value, ' ', CELL_LEN);
    // free the old string value, if it is a string, as it is not longer used
    if (CELL_P(y, x).cell_type == t_str)
        free(CELL_P(y, x).cell_value.str);

    // the difference between setCellWhole and setCellFloat is the formatting of the displayed value. SetCellWhole will not include a decimal point and leading decimal values.
    if (b_whole_num) {
        setCellWhole(&CELL_P(y, x), num);
    } else {
        setCellFloat(&CELL_P(y, x), num);
    }

    CELL_P(y, x).cell_type = t_int;
    CELL_P(y, x).cell_value.number = num;

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
    double max = floor(args[1]) + 1;

    if (min > max)  {
        LOG_ERROR("%s", "MINIMUM NUMBER COMES FIRST")
        return 0;
    }
    struct randInfo info = {
        .min = min,
        .max = max,
        .rand_type = RT_range_whole
    };
    iterSelectedCells(client, cursor, &fillRandom_iter, &info);
    cursor->visual_state = visual_state_NONE;
    // redraw the updated cells
    client->redraw_spreadsheet = 1;
    return 0;
}

// takes no arguments
int shuffleCellsCmd(Cellulose *client, cursor_t *cursor, long double __attribute_maybe_unused__ *args) {
    clearClipboard(cursor);
    srand(time(NULL));
    int selected_length = abs(cursor->select_pos_y - cursor->y) + 1;
    int selected_width = abs(cursor->select_pos_x - cursor->x) + 1;
    // list of booleans to indicate whether a selected cell coordinate has occured or not
    // divide by 8 because we will be using bits to check for random number repetition
    uint8_t* numbers = calloc((selected_length * selected_width) + 1, 1 );
    int res = clipboardAddRowsTo(cursor, selected_length);
    // the x and y offset for the client cell
    coord_int_t min_y = cursor->y > cursor->select_pos_y ? cursor->select_pos_y : cursor->y;
    coord_int_t min_x = cursor->x > cursor->select_pos_x ? cursor->select_pos_x : cursor->x;
    if (numbers == NULL || res == -1)
        return -1;
    for (int i = 0; i < selected_length * selected_width; ++i) {
        int rand_y = rand() % selected_length;
        int rand_x = rand() % selected_width;
        int rcell_index = (rand_y * selected_width) + rand_x;
        // increment rand_x and rand_y until it finds a coordinate that hasn't occurred
        while ((numbers[(int)floor((double)rcell_index/8)] & (1 << (rcell_index % 8))) != 0) {
            // mod opperatator to wrap back around selected width and length
            rand_x = (rand_x + 1) % selected_width;
            rand_y = (rand_y + (rand_x == 0)) % selected_length;
            // update rcell_index so we can reevaluate the index for collisions
            rcell_index = (rand_y * selected_width) + rand_x;
        }
        // set bit on corresponding to the random cell coordinate. This signifies that this coordinate has occured and should appear again
        numbers[(int)floor((double)rcell_index/8)] += 1 << (rcell_index % 8);
        // appends the break character and continues if the coordinate results in a non-existant or empty cell value
        if (!cellExist(client, rand_x + min_x, rand_y + min_y ) || CELL_P(rand_y + min_y, rand_x + min_x).cell_type == t_empty) {
            // add a space to help prevent offset errors
            pushChar(&cursor->clipboard.elements[(size_t)floor((double)i/selected_width)], ' ');
            pushChar(&cursor->clipboard.elements[(size_t)floor((double)i/selected_width)], '\n');
            continue;
        }
        // use random number here
        // the cell value that will be appended
        addCellToClipboard(cursor, CELL_P(rand_y + min_y, rand_x + min_x), floor((double)i/selected_width));
    }
    client->redraw_spreadsheet = true;
    cursor->visual_state = visual_state_NONE;
    cursor->mode = NORMAL_MODE;
    return 0;
}

int swapCellsCmd(Cellulose *client, cursor_t *cursor, long double *args) {
    srand(time(NULL));
    if (args == NULL)
        return -1;
    if (cursor->visual_state != visual_state_FREE_POINT) {
        LOG_INFO("ERROR: incorrect visual state (visual state: %i).", cursor->visual_state);
        return 0;
    }
    // the amount of times the cells should be swapped with one another
    int repeat_amount = floor(args[0]);
    if (repeat_amount <= 0) {
        LOG_ERROR("Repeat amount is less than zero. (Repeat amount %i)", repeat_amount);
        return 0;
    }
    int selected_width = abs(cursor->select_pos_x - cursor->x);
    int selected_length = abs(cursor->select_pos_y - cursor->y);
    // iterate arg1 amount of times swapping two random cells
    for (int repeat_count = 0; repeat_count < repeat_amount; ++repeat_count) {
        size_t first_val_x = selected_width == 0 ? 0  : rand() % selected_width;
        size_t first_val_y = selected_length == 0 ? 0 : rand() % selected_length;

        size_t second_val_x = selected_width == 0 ? 0 : first_val_x + 1 % selected_width;
        size_t second_val_y = selected_length == 0 ? 0: first_val_y + 1 % selected_length;
        if (!cellExist(client, first_val_x + cursor->select_pos_x, first_val_y + cursor->select_pos_y)) {
            createRowsTo(client, first_val_y + cursor->select_pos_y);
            createColumnsTo(client, first_val_y + cursor->select_pos_y, first_val_x + cursor->select_pos_x);
        }
        if (!cellExist(client, second_val_x + cursor->select_pos_x, second_val_y + cursor->select_pos_y)) {
            createRowsTo(client, second_val_y + cursor->select_pos_y);
            createColumnsTo(client, second_val_y + cursor->select_pos_y, second_val_x + cursor->select_pos_x);
        }
        cell_t first_cell = CELL_P(first_val_y + cursor->select_pos_y, first_val_x + cursor->select_pos_x);
        CELL_P(first_val_y + cursor->select_pos_y, first_val_x + cursor->select_pos_x) = CELL_P(second_val_y + cursor->select_pos_y, second_val_x + cursor->select_pos_x) ;
        CELL_P(second_val_y + cursor->select_pos_y, second_val_x + cursor->select_pos_x) = first_cell;
    }
    client->redraw_spreadsheet = 1;
    cursor->visual_state = visual_state_NONE;
    return 0;
}
