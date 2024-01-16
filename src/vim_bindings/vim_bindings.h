#pragma once
#include "../client.h"
#include "../cursor/cursor.h"
#include "vim_normal_mode.h"
#include "vim_insert_mode.h"

// parses the key pressed to run the corresponding vim motion
static int parseVimMotion(Cellulose *client, cursor_t* cursor, str *input_mode_string, int input)  {
    switch (cursor->mode) {
        case NORMAL_MODE:  return normalModeParseKey(client, cursor, input_mode_string, input);
        case COMMAND_MODE:
        case INSERT_MODE:  return parseCliInput(client, cursor, input, input_mode_string);
    }
    return -1;
}
