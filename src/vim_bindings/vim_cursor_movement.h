#pragma once
#include "../client.h"
#include "../cursor/cursor.h"
#include <ncurses.h>
// checks if the cursor is touching the bottom edge
static int touchingBottomEdge(Cellulose *client, int y) {
    return y == client->pos_y + CLIENT_SHEET_HEIGHT;
}
// checks if the cursor is touching the top edge
static int touchingTopEdge(Cellulose *client, int y) {
    return y == client->pos_y;
}

// checks if the cursor is touching the bottom edge
static int touchingRightEdge(Cellulose *client, int x) {
    return x == client->pos_x + CLIENT_SHEET_WIDTH;
}
// checks if the cursor is touching the top edge
static int touchingLeftEdge(Cellulose *client, int x) {
    return x == client->pos_x;
}
// parses the key input in normal mode
static int normalModeParseKey(Cellulose *client, cursor* cursor, str* cell_input, int input) {
    switch (input) {
        case 'j': {
            ++cursor->y;
            client->pos_y += touchingBottomEdge(client, cursor->y);
        } break;
        case 'k': {
            cursor->y -= (cursor->y > 0);
            // and sign to make sure we don't underflow
            client->pos_y -= touchingTopEdge(client, cursor->y) && client->pos_y != 0;
        } break;
        case 'l': {
            ++cursor->x;
            client->pos_x += touchingRightEdge(client ,cursor->x);
        } break;
        case 'h': {
            cursor->x -= (cursor->x > 0);
            client->pos_x -= touchingLeftEdge(client ,cursor->x) && client->pos_x != 0;
        } break;
        case 'i': {
            cursor->mode = INSERT_MODE;
            clear();
            str_res res;
            if (( res = strNew()).result == -1)
                return -1;
            *cell_input = res.string;
        } break;
        case 'a': {
            cursor->mode = INSERT_MODE;
            clear();
            str_res res;
            if (CUR_CELL_P.cell_type == t_int) {
                if ((res = fromDouble(CUR_CELL_P.cell_value.number)).result == -1)
                    return -1;
            } else if ((res = fromCharArray(CUR_CELL_P.cell_value.str)).result == -1)
                return -1;
            *cell_input = res.string;
        } break;
    }
    return 0;
}
