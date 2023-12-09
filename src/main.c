#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifndef CONFIG_PATH
#define CONFIG_PATH "../config.h"
#endif
#include "cli/tui.h"

#include "client.c"
#include "parsing.h"
#include "parsing.c"
#include "vec.h"

// displays the spreadsheet using ncurses if defined
// not defined usually for debugging purposes
/* #define RENDER_TUI */

/* #define GREY 100 */
typedef enum {
    Up,
    Down,
    Left,
    Right,
} movement;


int main(void) {
#ifdef RENDER_TUI
    startScreen();
#endif
    char* cell = malloc(15);
    // user input
    int in = 0x0;

    Cellulose client = fromCSV("../test.csv");
#ifdef RENDER_TUI
    renderSpreadsheet(&client, cell);
    free(cell);
    do {
        in = getch();
        refresh();
    // exit when the escape key is pressed
    } while (in != 27);
#endif

    // if true it means an error has occured deallocating
    if (cleanTUI(client)) { exit(1); }
    exit(0);
}
