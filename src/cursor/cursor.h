#pragma once
#include "../client_structs.h"
#include <stdbool.h>
#include "../str.h"
typedef struct {
    // the coordinates of the cursor
    unsigned short x,y;

    // the coordinates of the cursor
    unsigned short select_pos_x,select_pos_y;

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
    // This holds the cells that can be copy and pasted
    VEC_ATTR(str ,clipboard);
} cursor;

// Cursor contructor function
static cursor initCursor() {
    return (cursor){
        .x = 0,
        .y = 0,
        .mode = NORMAL_MODE,
        .visual_state = visual_state_NONE,
        .previous_char = '\0',
        .clipboard = VEC_NEW(str)
    };
}

static int clipboardAddRowsTo(cursor* cursor, size_t row_to) {
    while (cursor->clipboard.length <= row_to) {
        str_res empty_str;
        if ((empty_str = strNew()).result == -1)
            return -1;
        VEC_APPEND(cursor->clipboard, empty_str.string);
    }
    return 0;
}
// clears and frees all of the items in the clipboards for the cursor
static int clearClipboard(cursor* cursor) {
    VEC_ITER(cursor->clipboard, str, row) {
        free(row.contents);
    }
    free(cursor->clipboard.elements);
    cursor->clipboard.length = 0;
    return 0;
}
