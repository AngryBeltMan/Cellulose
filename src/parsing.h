// contains functions for parsing different spreadsheet file formats
#pragma once
#include "client.h"
// opens and parses a csv file
int fromCSV(Cellulose *client, const char* file_path);
