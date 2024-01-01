// displays the spreadsheet using ncurses if defined
// not defined usually for debugging purposes
#include "cursor/cursor.h"
#define RENDER_TUI

#include "ui/cli_ui.h"

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


int main(int argc, char **argv) {
    if (argc == 1)
        printf("Cellulose; a spreadsheet editor with vim-like bindings.\nTo begin provide a path to a file to begin editing the file.\n"),
        exit(0);
    cursor cursor = initCursor();
    // user input
    Cellulose client = newEmpty();
    int parsing_res = fromCSV(&client, argv[1]);
    if (parsing_res) {
        printf("ERROR: failed to parse/open input file.\n");
        exit(1);
    }
#ifdef RENDER_TUI
    str user_input;
    int in = 0x0;
    startScreen();
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
    if (cursor.clipboard.length > 0)
        clearClipboard(&cursor);
    exit(cleanUp(client));
}
