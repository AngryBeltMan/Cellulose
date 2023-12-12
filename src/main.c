// displays the spreadsheet using ncurses if defined
// not defined usually for debugging purposes
#define RENDER_TUI

#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cli/tui.h"
#include "cli/spreadsheet_ui.h"
#include "cli/cursor_ui.h"
#include "vim_bindings/vim_bindings.h"

#include "parsing.h"
#include "parsing.c"
#include "vec.h"




int main(void) {
#ifdef RENDER_TUI
    startScreen();
#endif
    cursor cursor = initCursor();
    char* cell = malloc(15);
    // user input
    int in = 0x0;

    Cellulose client = fromCSV("../test.csv");
#ifdef RENDER_TUI
    renderSpreadsheet(&client, cell);
    renderCursor(&cursor, &client);
    do {
        in = getch();
        parseVimMotion(&client, &cursor, in);
        renderSpreadsheet(&client, cell);
        renderCursor(&cursor, &client);
    // exit when the escape key is pressed
    } while (in != 27);
#endif
    free(cell);
    // if true it means an error has occured deallocating
    if (cleanTUI(client)) { exit(1); }
    exit(0);
}
