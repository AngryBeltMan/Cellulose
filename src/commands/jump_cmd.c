#include "jump_cmd.h"
#include "../messages.h"
int commandJumpX(Cellulose *client, cursor_t *cursor, long double *args) {
    if (args == NULL)
        return -1;
    if (args[0] <= 0) {
        LOG_ERROR("%s", "X COORD IS LESS THAN 1");
        return 0;
    }
    cursor->x = floor(args[0]) - 1;
    client->pos_x = floor(args[0]) - 1;
    client->redraw_spreadsheet = 1;
    return 0;
}
int commandJumpY(Cellulose *client, cursor_t *cursor, long double *args) {
    if (args == NULL)
        return -1;
    if (args[0] <= 0) {
        LOG_ERROR("%s", "Y COORD IS LESS THAN 1");
        return 0;
    }
    cursor->y = floor(args[0]) - 1;
    client->pos_y = floor(args[0]) - 1;
    client->redraw_spreadsheet = 1;
    return 0;
}
int commandJumpXY(Cellulose *client, cursor_t *cursor, long double *args) {
    if (args == NULL)
        return -1;
    if (commandJumpX(client, cursor, &args[0]) == -1)
        return -1;
    if (commandJumpY(client, cursor, &args[1]) == -1)
        return -1;
    return 0;
}
