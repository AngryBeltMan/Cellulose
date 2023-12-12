#pragma once
#ifndef CONFIG_DIR
#define CONFIG_DIR "../config.h"
#endif
#include CONFIG_DIR

#include <ncurses.h>
#include "../cursor/cursor_include.h"
#include "../client.h"

static void renderCursor(cursor* cursor, Cellulose *client) {
    move((cursor->y + 1) - client->pos_y, ((cursor->x + 1) - client->pos_x ) * 15);
    attron(COLOR_PAIR(MODE_NORMAL_ID));
    if (cellExist(client, cursor->x, cursor->y))
        printw("%s", CELL_P(cursor->y, cursor->x).displayed_value);
    else
        printw("              ");
}
