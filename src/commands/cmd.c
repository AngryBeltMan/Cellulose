#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "cmd.h"
#include "../config_include.h"
#include "../messages.h"
#include "../cell.h"
#include "rand_cmds.h"
#include "system_cmd.h"
#include "jump_cmd.h"

#define CMD_CASE(_id, _func, _argc) \
    case _id: \
        return (cmd_t) {.fn = &_func, .arg_count = _argc};

// Hashes the command name into a unique integer. Order of the chars changes the output.
static uint32_t cmd_hash(const char* val) {
    uint32_t sum = 0;
    char ch;
    int index = 0;
    do {
        ch = val[index];
        sum += ch * powf(10, index);
        ++index;
    } while (ch != 0 && index < 14);
    return sum;
}

cmd_t getCommand(const char* name, int *result) {
    *result = 0;
    switch (cmd_hash(name)) {
        // Command name/identifier "rand". Generates a random integer number.
        CMD_CASE(112084, fillRandom, 0)
        // Command name "randr". Generates a random floating point value number between a min (arg1) and a max arg(2).
        CMD_CASE(1252084, fillRandomRange, 2)
        // Command name "randw". Generates a random whole integer number between a min (arg1) and a max arg(2).
        CMD_CASE(1302084, fillRandomWholeRange, 2)
        // Command name "exit" or "q". Exits the spreadsheet.
        CMD_CASE(127801, commandExit , 0)
        CMD_CASE(113, commandExit , 0)
        // Command name "jmp". Jumps to the specified coordinate.
        CMD_CASE(12396, commandJumpXY, 2)
        // Command name "jmpx". Jumps to the specified column while preserving the cursor's row position.
        CMD_CASE(132396, commandJumpX, 1)
        // Command name "jmpy". Alternate way to exit the spreadsheet.
        CMD_CASE(133396, commandJumpY, 1)
        // Command name "ref". Redraws the spreadsheet and command line.
        CMD_CASE(11324, commandRefresh, 0)
        // Command name "save" or "s". Writes the edited contents into a file.
        CMD_CASE(113885, commandSave, 0)
        CMD_CASE(115, commandSave, 0)
        // Command name "swap". Swaps between two randomly selected cells arg1 amount of times.
        CMD_CASE(123005, swapCellsCmd, 1)
        // Command name "shuffle". Randomly reorders the selected cells saving it to the cursor's clipboard. This doesn't affect the selected cells.
        CMD_CASE(112934856, shuffleCellsCmd, 0)
        default: {
            LOG_ERROR("%s", "COMMAND DOESN'T EXIST!");
            *result = -1;
            return (cmd_t){ .fn = NULL, .arg_count = 0 };
        } break;
    }

}

int runCommand(Cellulose* client, cursor_t* cursor, char *input) {
    int i = 0;
    char ch;
    while (1) {
        ch = input[i];
        if (ch == '\0')
            break;
        if (ch == '(') {
            input[i] = '\0';
            break; }
        ++i;
    }
    int res;
    cmd_t cmd = getCommand(input, &res);
    // exit if command doesn't exist
    if (res == -1)
        return 0;

    if (cmd.arg_count == 0)
        return cmd.fn(client, cursor, NULL);

    long double* args = malloc(sizeof(long double) * cmd.arg_count);

    if (args == NULL)
        return -1;

    size_t arguments = 0;
    char* arg = strtok(input + i + 1, ",");
    while (arguments < cmd.arg_count) {
        if (arg == NULL) {
            LOG_ERROR("NOT ENOUGH ARGS expected %i got %lu\n", cmd.arg_count, arguments );
            return 0;
        }
        args[arguments] = atof(arg);
        ++arguments;
        arg = strtok(NULL, ",");
    }
    cmd.fn(client, cursor, args);
    free(args);
    return 0;
}
