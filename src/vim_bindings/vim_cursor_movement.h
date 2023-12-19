#include "../client.h"
#include "../cursor/cursor.h"
#include <ncurses.h>

static int touchingBottomEdge(Cellulose *client, int y) {
    return y == client->pos_y + CLIENT_SHEET_HEIGHT;
}
static int touchingTopEdge(Cellulose *client, int y) {
    return y == client->pos_y;
}

static int touchingRightEdge(Cellulose *client, int x) {
    return x == client->pos_x + CLIENT_SHEET_WIDTH;
}
static int touchingLeftEdge(Cellulose *client, int x) {
    return x == client->pos_x;
}
// all of the cases for cursor movement
#define CURSOR_MOVEMENT_CASES()  \
        case 'j': { \
            ++cursor->y; \
            client->pos_y += touchingBottomEdge(client, cursor->y); \
        } break; \
        case 'k': { \
            cursor->y -= (cursor->y > 0); \
            /* and sign to make sure we don't underflow */ \
            client->pos_y -= touchingTopEdge(client, cursor->y) && client->pos_y != 0; \
        } break; \
        case 'l': { \
            ++cursor->x; \
            client->pos_x += touchingRightEdge(client ,cursor->x); \
        } break; \
        case 'h': { \
            cursor->x -= (cursor->x > 0); \
            client->pos_x -= touchingLeftEdge(client ,cursor->x) && client->pos_x != 0; \
        } break;

