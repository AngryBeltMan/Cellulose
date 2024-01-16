#include "cursor.h"

// Cursor contructor function
cursor_t initCursor() {
    return (cursor_t){
        .x = 0,
        .y = 0,
        .mode = NORMAL_MODE,
        .visual_state = visual_state_NONE,
        .previous_char = '\0',
        .clipboard = VEC_NEW(str)
    };
}

int clipboardAddRowsTo(cursor_t* cursor, size_t row_to) {
    while (cursor->clipboard.length <= row_to) {
        str_res empty_str;
        if ((empty_str = strNew()).result != str_no_error)
            return -1;
        VEC_APPEND(cursor->clipboard, empty_str.string);
    }
    return 0;
}
// clears and frees all of the items in the clipboards for the cursor
int clearClipboard(cursor_t* cursor) {
    VEC_ITER(cursor->clipboard, str, row) {
        free(row.contents);
    }
    cursor->clipboard.length = 0;
    return 0;
}
