#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define T_ASSERT(_cmp) if (!(_cmp)) {fprintf(stderr, "\033[38;2;255;25;0mAssertion Failed: (comparison: %s, function: %s, file: %s, line number: %i)\033[0m\n", #_cmp, __FUNCTION__, __FILE__, __LINE__); return T_FAILURE;}
#define T_SUCCESS 1
#define T_FAILURE 0
typedef struct {
    void** funcs;
    size_t func_count;
    size_t max_func_count;
} test_client;

static test_client initTestClient() {
    test_client client;
    client.func_count = 0;
    client.max_func_count = 5;
    client.funcs = malloc(sizeof(void*) * 5);
    assert(client.funcs && "ERROR: failed to malloc function pointer list");
    return client;
}
static void appendTest(test_client *client, void* function) {
    client->funcs[client->func_count] = function;
    if (++client->func_count == client->max_func_count) {
        client->max_func_count *= 2;
        client->funcs = realloc(client->funcs, client->max_func_count * sizeof(void*));
        assert(client->funcs && "ERROR: failed to malloc function pointer list");
    }
}
static void runTests(test_client *client) {
    for (size_t test_num = 0; test_num < client->func_count; ++test_num) {
        // cast the void* into a function that returns an integer representing its result
        int result = ((int (*)(void))client->funcs[test_num])();
        if (result) { printf("\033[38;2;0;255;25mRESULT: SUCCESS\033[0m\n");
        } else { printf("\033[38;2;255;25;0mRESULT: FAILED\033[0m\n"); }
    }
}

static void freeClient(test_client client) {
    free(client.funcs);
}
