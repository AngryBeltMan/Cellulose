#include "tests.h"
#include "csv_tests.c"
#include "cell_tests.c"
int main(void) {
    test_client client = initTestClient();
    /* appendTest(&client, &parse_single_row); */
    /* appendTest(&client, &parse_multiple_rows); */
    /* appendTest(&client, &parse_large_cells); */
    /* appendTest(&client, &many_rows_test); */
    appendTest(&client, &testStrToNum);
    appendTest(&client, &testStrToDecimal);
    /* appendTest(&client, &csvTypeTest); */
    runTests(&client);
    freeClient(client);
    return 0;
}
