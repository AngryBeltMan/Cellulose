#include "system_cmd.h"
int commandExit(Cellulose *client, cursor_t *cursor, long double *args) {
    // negative one causes the spreadsheet to exit
    return -1;
}
int commandRefresh(Cellulose *client, cursor_t *cursor, long double *args) {
    client->redraw_spreadsheet = true;
    client->redraw_cli = true;
    client->redraw_dividers = true;
    return 0;
}
