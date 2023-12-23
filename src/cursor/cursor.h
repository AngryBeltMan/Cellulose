#pragma once
#include <stdbool.h>

typedef struct {
    // the coordinates of the cursor
    unsigned short x,y;

    // the coordinates of the cursor
    unsigned short select_pos_x,select_pos_y;
    // This mode is used to select multiple cells

    // The different states visual mode may have.
    enum {
        visual_state_NONE = 0,
        visual_state_FREE_POINT = 1,
        visual_state_ROW_SELECT = 2,
        visual_state_COLUMN_SELECT = 3,
    } visual_state;

    // holds the previous char pressed
    char previous_char;

    // the amount of times a command should be repeated
    unsigned int repeat_count;

    // The mode the cursor The modes are similar to vim
    enum {
        // This mode is used to edit text inside a cell
        INSERT_MODE,
        // normal mode is the mode where you can use vim bindings to move and edit
        NORMAL_MODE,
        // This mode is used to type out commands
        COMMAND_MODE,
    } mode;
} cursor;

// Cursor contructor function
static cursor initCursor() {
    return (cursor){
        .x = 0,
        .y = 0,
        .mode = NORMAL_MODE,
        .visual_state = visual_state_NONE,
        .previous_char = '\0'
    };
}
