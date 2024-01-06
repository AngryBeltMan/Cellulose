#pragma once
#include<stdint.h>
#include "../client.h"
#include "../cursor/cursor.h"

typedef struct {
    uint8_t arg_count;
    int (*fn)(Cellulose*, cursor_t*, long double*);
} cmd_t;

int runCommand(Cellulose* client, cursor_t* cursor, char* input);

