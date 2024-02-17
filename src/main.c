// displays the spreadsheet using ncurses if defined
// not defined usually for debugging purposes
#define RENDER_TUI

#include "config.h"
#include "cursor/cursor.h"
#include "messages.h"

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
#include "vec.h"


int main(int argc, char **argv) {
    initLogFile();
    if (argc == 1)
        printf("Cellulose; a spreadsheet editor with vim-like bindings.\nTo begin provide a path to a file to begin editing the file.\n"),
        exit(0);
    cursor_t cursor = initCursor();
    // user input
    Cellulose client = newEmpty(argv[1]);
    int parsing_res = fromCSV(&client);
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
            goto main_clean_up;
        drawSpreadsheetDividers(&client.redraw_dividers);
        renderSpreadsheet(&client, &cursor);
        drawColumnHeader(&client, &cursor);
        drawRowHeader(&client, &cursor);
        renderCursor(&cursor, &client);
        renderCommandLine(&cursor, &user_input, client.redraw_cli);
        in = getch();
    } while (1);
#else
    const char input[] = "lvjjjif|%{1:y}\n:q\n";
    size_t input_len = strlen(input);
    str user_input;
    for (size_t input_index = 0; input_index < input_len; ++input_index) {
        if (parseVimMotion(&client, &cursor, &user_input, input[input_index]) == -1) {
            goto main_clean_up;
        }
    }
#endif

main_clean_up:
    exit(cleanUp(client, cursor, user_input));
    freeLogFile();
}
