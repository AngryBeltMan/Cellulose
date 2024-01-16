// save the spreadsheet
#include "client.h"
#include "messages.h"
#include "vec.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

static int writeContents(Cellulose* client, char separator, FILE* output);
int saveSpreadsheet(Cellulose* client) {
    const char* file_name = client->name;
    // gets the file extension
    char* file_extension = strrchr(file_name, '.');
    if (file_extension == NULL) {
        LOG_ERROR("ERROR: no file extensions (%s).", file_name);
        return 0;
    }
    FILE* saved_file = fopen(file_name, "w");
    if (saved_file == NULL) {
        LOG_ERROR("Could not write to file (%s). %s", file_name, strerror(errno));
        return 0;
    }
    writeContents(client, ',', saved_file);
    fclose(saved_file);
    return 0;
}
static int writeContents(Cellulose* client, char separator, FILE* output) {
    VEC_ITER(client->spread_sheet, row_t, row) {
        int empty_space = 0;
        VEC_ITER(row, cell_t, cell) {
            switch (cell.cell_type) {
                case t_empty:
                    ++empty_space;
                    break;
                case t_str:
                    for (int i = 0; i < empty_space; ++i)
                        fputc(separator, output);
                    fprintf(output, "%s%c", cell.cell_value.str, separator);
                    break;
                case t_int:
                    for (int i = 0; i < empty_space; ++i)
                        fputc(separator, output);
                    fprintf(output, "%f%c", cell.cell_value.number, separator);
                    break;
            }
        }
        // write a new line
        fputc(0xA, output);
    }
    return 0;
}
