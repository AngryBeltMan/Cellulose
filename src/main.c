// displays the spreadsheet using ncurses if defined
// not defined usually for debugging purposes
#include "ui/cli_ui.h"
#define RENDER_TUI

#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ui/tui.h"
#include "ui/spreadsheet_ui.h"
#include "ui/cursor_ui.h"
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
    Cellulose client = fromCSV("../asl_test.csv");
    str user_input;
#ifdef RENDER_TUI
    do {
        if (parseVimMotion(&client, &cursor, &user_input, in) == -1)
            if (cleanUp(client))
                exit(1);
        renderSpreadsheet(&client, cell);
        renderCursor(&cursor, &client);
        renderCommandLine(&cursor, &user_input);
        in = getch();
    // exit when the escape key is pressed
    } while (in != 27);
#endif
    free(cell);
    free(user_input.contents);
    // if true it means an error has occured deallocating
    if (cleanUp(client)) { exit(1); }
    exit(0);
}
