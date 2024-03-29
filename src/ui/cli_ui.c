#include "cli_ui.h"

#define DISPLAY_CURSOR_STATUS(_status)\
    case _status:\
        printw("%s", #_status);\
        break;

static inline void displayRepeatCount(int repeat_count);
static inline void displayCursorCoordinate(cursor_t *cursor);
static inline void displayCursorStatus(int cursor_mode);
static inline void displayCellInput(str *cell_input, int cursor_mode);

void renderCommandLine(cursor_t *cursor, str *cell_input, bool redraw) {
    if (!redraw)
        return;
    displayCursorStatus(cursor->mode);
    displayCursorCoordinate(cursor);
    displayRepeatCount(cursor->repeat_count);
    displayCellInput(cell_input, cursor->mode);
}

static inline void displayCursorStatus(int cursor_mode) {
    // go to the space below all of the cells
    move(CLIENT_SHEET_HEIGHT + 1, (CLIENT_SHEET_WIDTH - 2) * 15);
    attron(COLOR_PAIR(CURSOR_MODE_ID));
    switch (cursor_mode) {
        DISPLAY_CURSOR_STATUS(NORMAL_MODE)
        DISPLAY_CURSOR_STATUS(INSERT_MODE)
        DISPLAY_CURSOR_STATUS(COMMAND_MODE)
    }
}

static inline void displayCellInput(str* cell_input, int cursor_mode) {
    // only display the bottom command line when the user is changing some cells or is entering a command
    switch (cursor_mode) {
        case NORMAL_MODE: return;
        case INSERT_MODE:
            move(CLIENT_SHEET_HEIGHT + 1, 0);
            printw(":%s", cell_input->contents);
            break;
        case COMMAND_MODE:
            move(CLIENT_SHEET_HEIGHT + 1, 0);
            printw("/%s", cell_input->contents);
            break;
    }
}

// shows the amount of times the next command will be repeated
static inline void displayRepeatCount(int repeat_count) {
    move(CLIENT_SHEET_HEIGHT + 1, (CLIENT_SHEET_WIDTH) * 15);
    printw("                    ");
    move(CLIENT_SHEET_HEIGHT + 1, (CLIENT_SHEET_WIDTH) * 15);
    printw("%i", repeat_count);
}
static inline void displayCursorCoordinate(cursor_t* cursor) {
    move(CLIENT_SHEET_HEIGHT + 1, (CLIENT_SHEET_WIDTH - 1) * 15);
    printw("                    ");
    move(CLIENT_SHEET_HEIGHT + 1, (CLIENT_SHEET_WIDTH - 1) * 15);
    printw(CURSOR_COORD_FORMATTING, cursor->x + 1, cursor->y + 1);
}
