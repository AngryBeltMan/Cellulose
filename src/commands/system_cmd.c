#include "system_cmd.h"
#include "../messages.h"
#include <sys/cdefs.h>
#define ARG_UNUSED __attribute_maybe_unused__
int commandExit(Cellulose ARG_UNUSED *client, cursor_t ARG_UNUSED *cursor, long double ARG_UNUSED *args) {
    // negative one causes the spreadsheet to exit
    return -1;
}
int commandSave(Cellulose *client, cursor_t ARG_UNUSED *cursor, long double ARG_UNUSED *args) {
    saveSpreadsheet(client);
    LOG_INFO("Modifications have been saved to (%s)", client->name);
    client->has_saved = true;
    return 0;
}
int commandRefresh(Cellulose *client, cursor_t ARG_UNUSED *cursor, long double ARG_UNUSED *args) {
    clear();
    client->redraw_spreadsheet = true;
    client->redraw_cli = true;
    client->redraw_dividers = true;
    return 0;
}
