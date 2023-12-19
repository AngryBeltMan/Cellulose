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
    initCellColors();
}
// deallocates uneeded varibles and exits the TUI if a certain preprossesor is set
static int cleanUp(Cellulose client) {
    freeSpreadsheet(client);
#ifdef RENDER_TUI
    endwin();
#endif
    // zero indicates success
    return 0;
}
