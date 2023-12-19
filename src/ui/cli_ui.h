#pragma once
#include <ncurses.h>
#include "../cursor/cursor_include.h"
#include "../client.h"
#include "../config_include.h"
#include "../str.h"

static void displayCursorCoordinate(cursor* cursor) {
    move(CLIENT_SHEET_HEIGHT + 1, (CLIENT_SHEET_WIDTH) * 15);
    printw("                    ");
    move(CLIENT_SHEET_HEIGHT + 1, (CLIENT_SHEET_WIDTH) * 15);
    printw(CURSOR_COORD_FORMATTING, cursor->x, cursor->y);
}

static void displayCursorStatus(int cursor_mode) {
    // go to the space below all of the cells
    move(CLIENT_SHEET_HEIGHT + 1, (CLIENT_SHEET_WIDTH - 2) * 15);
    attron(COLOR_PAIR(CURSOR_MODE_ID));
    switch (cursor_mode) {
        case INSERT_MODE:
            printw("EDIT MODE  ");
            break;
        case NORMAL_MODE:
            printw("NORMAL MODE");
            break;
    }
}
static void displayCellInput(str* cell_input, int cursor_mode) {
    // only display the bottom command line when the user is changing some cells or is entering a command
    if (cursor_mode == NORMAL_MODE)
        return;
    move(CLIENT_SHEET_HEIGHT + 1, 0);
    printw(":%s", cell_input->contents);
}
static void renderCommandLine(cursor* cursor, str* cell_input) {
    displayCursorStatus(cursor->mode);
    displayCursorCoordinate(cursor);
    displayCellInput(cell_input, cursor->mode);
}
