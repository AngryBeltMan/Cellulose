#include "debug.h"
#include "config.h"
#include "client.h"
#ifdef CELLULOSE_DEBUG_MODE
void logCurrentCellVal(Cellulose* client, coord_int_t x, coord_int_t y) {
    logToFile("DEBUG LOG: Cell at %i %i", x, y);
    if (cellExist(client, x, y)) {
        logToFile("cell has type %i");
    }
}
#endif
