#include "vim_cursor_movement.h"
int touchingBottomEdge(Cellulose *client, int y) {
    return y >= client->pos_y + CLIENT_SHEET_HEIGHT;
}
int touchingTopEdge(Cellulose *client, int y) {
    return y <= client->pos_y;
}

int touchingRightEdge(Cellulose *client, int x) {
    return x >= client->pos_x + CLIENT_SHEET_WIDTH;
}
int touchingLeftEdge(Cellulose *client, int x) {
    return x < client->pos_x;
}
void gotoTop(Cellulose *client, cursor_t *cursor) {
    if (cursor->previous_char != 'g')
        return;
    client->redraw_spreadsheet = true;
    client->pos_y = 0;
    cursor->y = 0;
}
void gotoBottom(Cellulose *client, cursor_t *cursor) {
    client->redraw_spreadsheet = true;
    client->pos_y = (SHEET_LEN - CLIENT_SHEET_HEIGHT) * (SHEET_LEN > CLIENT_SHEET_HEIGHT);
    cursor->y = SHEET_LEN - (SHEET_LEN != 0);
}
