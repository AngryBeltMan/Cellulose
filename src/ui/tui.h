#include <ncurses.h>
#include <stdlib.h>
#include "../client.h"
#include "../config_include.h"


// sets up important features needed for rendering the spreadsheet
static void startScreen() {
    initscr();
    // prevents CTRL-C and other commands from killing the program and instead it is just used as input
    raw();
    // prevents user input from being echoed back
    noecho();

    // disables the cursor
    curs_set(0);

    if (!has_colors()) {
        fprintf(stderr, "Your terminal doesn't support colors\n");
        endwin();
        exit(1);
    }
    start_color();
    /*  create new color grey */
    init_color(GREY_ID, 750, 750, 750);

    /* create new color grey */
    init_color(PURPLE_ID, 550, 50, 450);

    /* arg 1 foreground arg2 background */
    init_pair(STR_CELL_ID, COLOR_BLACK, GREY_ID );

    init_pair(INT_CELL_ID, COLOR_BLUE, GREY_ID );

    init_pair(COLUMN_CELL_ID, COLOR_BLACK, COLOR_GREEN );

    init_pair(ROW_CELL_ID, COLOR_BLACK, COLOR_CYAN );

    init_pair(MODE_NORMAL_ID, COLOR_WHITE, COLOR_BLACK );

    init_pair(MODE_EDIT_ID, COLOR_BLACK, COLOR_CYAN );

    init_pair(SELECTED_CELL_ID, COLOR_WHITE, PURPLE_ID );

    init_pair(CURSOR_MODE_ID, COLOR_WHITE, COLOR_BLACK );
    /* MSG ids */
    init_pair(INFO_MSG_ID, COLOR_BLUE, COLOR_WHITE );
    init_pair(ERROR_MSG_ID, COLOR_RED, GREY_ID );
}
// deallocates uneeded varibles and exits the TUI if a certain preprossesor is set
static int cleanUp(Cellulose client, cursor_t cursor, str user_input) {
    freeSpreadsheet(client);
    if (cursor.clipboard.length > 0)
        clearClipboard(&cursor);
    free(cursor.clipboard.elements);
    if (user_input.contents != NULL)
        free(user_input.contents);
#ifdef RENDER_TUI
    endwin();
#endif
    // zero indicates success
    return 0;
}
