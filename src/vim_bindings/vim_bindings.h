#pragma once
#include "../client.h"
#include "../cursor/cursor.h"
#include "vim_cursor_movement.h"
static void normalModeParseKey(Cellulose *client, cursor* cursor, int input);
// parses the key pressed to run the corresponding vim motion
static void parseVimMotion(Cellulose *client, cursor* cursor, int input) {
    switch (cursor->mode) {
        case NORMAL_MODE:  normalModeParseKey(client, cursor, input); break;
        case INSERT_MODE:  break;
        case COMMAND_MODE:  break;
    }
}
