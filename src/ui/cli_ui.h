#pragma once
#include <ncurses.h>
#include "../cursor/cursor_include.h"
#include "../client.h"
#include "../config_include.h"
#include "../str.h"

void renderCommandLine(cursor_t* cursor, str* cell_input, bool redraw);
