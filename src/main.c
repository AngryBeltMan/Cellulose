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
    // user input
    int in = 0x0;
    Cellulose client = fromCSV("../test.csv");
    str user_input;
#ifdef RENDER_TUI
    do {
        if (parseVimMotion(&client, &cursor, &user_input, in) == -1)
            exit(cleanUp(client));
        drawSpreadsheetDividers(&client.redraw_dividers);
        renderSpreadsheet(&client, &cursor);
        renderCursor(&cursor, &client);
        renderCommandLine(&cursor, &user_input);
        in = getch();
    // exit when the escape key is pressed
    } while (in != 27);
#endif
    free(user_input.contents);
    exit(cleanUp(client));
}
