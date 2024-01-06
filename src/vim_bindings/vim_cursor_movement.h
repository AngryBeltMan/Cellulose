#pragma once
#include "../client.h"
#include "../cursor/cursor.h"
#include <ncurses.h>
#define MOVE_AMOUNT (cursor->repeat_count + (cursor->repeat_count == 0))
// calculate how much a cursor can move given an input that garentee that no underflow occurs
#define CALC_MOVEMENT(_amount, _cur_pos) (_amount - ((_amount + _cur_pos) * ((int)(_cur_pos + _amount) < 0)))
#define MOVE_CURSOR(_cursor_amount, _client_x_amount, _client_y_amount, _cond, _cur_attr) \
    /*                                     \/- used to prevent underflows*/\
    client->redraw_spreadsheet = (cursor->visual_state != visual_state_NONE);   \
    cursor->_cur_attr += CALC_MOVEMENT(_cursor_amount, cursor->_cur_attr); \
    if (_cond) { \
        client->redraw_spreadsheet = true; \
        client->pos_x += CALC_MOVEMENT(_client_x_amount, client->pos_x); \
        client->pos_y += CALC_MOVEMENT(_client_y_amount, client->pos_y); \
    } else \
        renderSingleCell(client, cursor, (cursor->x - _client_x_amount) - client->pos_x, (cursor->y - _client_y_amount) -  client->pos_y);\

static int touchingBottomEdge(Cellulose *client, int y) {
    return y >= client->pos_y + CLIENT_SHEET_HEIGHT;
}
static int touchingTopEdge(Cellulose *client, int y) {
    return y <= client->pos_y;
}

static int touchingRightEdge(Cellulose *client, int x) {
    return x >= client->pos_x + CLIENT_SHEET_WIDTH;
}
static int touchingLeftEdge(Cellulose *client, int x) {
    return x < client->pos_x;
}
static void gotoTop(Cellulose *client, cursor_t* cursor) {
    if (cursor->previous_char != 'g')
        return;
    client->redraw_spreadsheet = true;
    client->pos_y = 0;
    cursor->y = 0;
}

static void gotoBottom(Cellulose *client, cursor_t* cursor) {
    client->redraw_spreadsheet = true;
    client->pos_y = (SHEET_LEN - CLIENT_SHEET_HEIGHT) * (SHEET_LEN > CLIENT_SHEET_HEIGHT);
    cursor->y = SHEET_LEN - (SHEET_LEN != 0);
}
// all of the cases for cursor movement
#define CURSOR_MOVEMENT_CASES()  \
        case 'j':  \
            MOVE_CURSOR(MOVE_AMOUNT, 0, MOVE_AMOUNT, touchingBottomEdge(client, cursor->y), y );\
            cursor->repeat_count = 0; break;\
        case 'k':  \
            MOVE_CURSOR(-MOVE_AMOUNT, 0, -MOVE_AMOUNT, touchingTopEdge(client, cursor->y) && client->pos_y > 0, y);\
            cursor->repeat_count = 0; break;\
        case 'l':  \
            MOVE_CURSOR(MOVE_AMOUNT, MOVE_AMOUNT, 0,touchingRightEdge(client, cursor->x), x);\
            cursor->repeat_count = 0; break;\
        case 'h':  \
            MOVE_CURSOR(-MOVE_AMOUNT, -MOVE_AMOUNT, 0,touchingLeftEdge(client, cursor->x) && client->pos_x > 0, x);\
            cursor->repeat_count = 0; break;\
        case 'g': gotoTop(client, cursor); break; \
        case 'G': gotoBottom(client, cursor); break; \
        case '$': { \
            if (cursor->y < SHEET_P.length) \
                client->redraw_spreadsheet = true;\
                cursor->x = ROW_P(cursor->y).length - (ROW_P(cursor->y).length != 0), \
                client->pos_x = (ROW_P(cursor->y).length - CLIENT_SHEET_WIDTH) * (ROW_P(cursor->y).length > CLIENT_SHEET_WIDTH); \
        } break; \
        case '_': \
            client->redraw_spreadsheet = true;\
            cursor->x = 0; \
            client->pos_x = 0; \
            break;
