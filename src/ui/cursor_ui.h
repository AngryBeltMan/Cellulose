#pragma once

#include <ncurses.h>
#include "../cursor/cursor_include.h"
#include "../client.h"
#include "../config_include.h"

static void renderCursor(cursor* cursor, Cellulose *client) {
    move((cursor->y + 1) - client->pos_y, ((cursor->x + 1) - client->pos_x ) * 15);
    attron(COLOR_PAIR(MODE_NORMAL_ID));
    if (cellExist(client, cursor->x, cursor->y) && CUR_CELL_P.cell_type != t_empty)
        printw("%s", CELL_P(cursor->y, cursor->x).displayed_value);
    else
        printw("              ");
}
