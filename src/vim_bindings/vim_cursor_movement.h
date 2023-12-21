#include "../client.h"
#include "../cursor/cursor.h"
#include <ncurses.h>
#define MOVE_AMOUNT (cursor->repeat_count + (cursor->repeat_count == 0))
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
static void gotoTop(Cellulose *client, cursor* cursor) {
    if (cursor->previous_char != 'g')
        return;
    client->pos_y = 0;
    cursor->y = 0;
}

static void gotoBottom(Cellulose *client, cursor* cursor) {
    client->pos_y = (SHEET_LEN - CLIENT_SHEET_HEIGHT) * (SHEET_LEN > CLIENT_SHEET_HEIGHT);
    cursor->y = SHEET_LEN - (SHEET_LEN != 0);
}

// all of the cases for cursor movement
#define CURSOR_MOVEMENT_CASES()  \
        case 'j': { \
            cursor->y += MOVE_AMOUNT; \
            client->pos_y += touchingBottomEdge(client, cursor->y) * MOVE_AMOUNT; \
            cursor->repeat_count = 0;\
        } break; \
        case 'k': { \
            cursor->y -=  MOVE_AMOUNT - ((MOVE_AMOUNT - cursor->y) * ((int)(cursor->y - MOVE_AMOUNT) < 0)); \
            client->pos_y = touchingTopEdge(client ,cursor->y) ? (cursor->y) : client->pos_y; \
            cursor->repeat_count = 0;\
        } break; \
        case 'l': { \
            cursor->x += MOVE_AMOUNT; \
            client->pos_x += touchingRightEdge(client ,cursor->x) * (MOVE_AMOUNT); \
            cursor->repeat_count = 0;\
        } break; \
        case 'h': { \
            cursor->x -=  MOVE_AMOUNT - ((MOVE_AMOUNT - cursor->x) * ((int)(cursor->x - MOVE_AMOUNT) < 0)); \
            client->pos_x = touchingLeftEdge(client ,cursor->x) ? (cursor->x) : client->pos_x; \
            cursor->repeat_count = 0;\
        } break; \
        case 'g': {\
            gotoTop(client, cursor);\
        } break;\
        case 'G': {\
            gotoBottom(client, cursor);\
        } break;\

