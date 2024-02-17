#pragma once
#ifdef CELLULOSE_DEBUG_MODE
#include "config_include.h"
#include "messages.h"
#include "client.h"
void logCurrentCellVal(Cellulose* client,coord_int_t x, coord_int_t y);
#endif
