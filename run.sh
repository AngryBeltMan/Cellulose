#!/bin/sh
MEMCHECK_FULL=0
INPUT_FILE="test.csv"
FILE_RUNNED=./build/cellulose

valgrind_mode() {
    make
    echo "running using valgrind"
    if [ $MEMCHECK_FULL ]
    then
        echo Enabling full memcheck
        valgrind --leak-check=full $FILE_RUNNED $INPUT_FILE
    else
        valgrind $FILE_RUNNED $INPUT_FILE
    fi
}
test_mode() {
    echo "running all of the tests"
    gcc -g -Wall -Wextra -Wpedantic -o tests.out tests/test.c -lncurses -lm
    ./tests.out
}
basic_run() {
    make
    $FILE_RUNNED $INPUT_FILE
}

# get command line flags execute_mode will be used to see how to execute the executable
case "${1}" in
    "-v") valgrind_mode ;;
    "-t") test_mode ;;
    "-m")
        MEMCHECK_FULL=1
        valgrind_mode ;;
    *) basic_run ;;

esac
