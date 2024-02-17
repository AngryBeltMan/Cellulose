CC := gcc
C_WARNINGS := -Wall -Wpedantic -Wextra
C_WARNINGS_TESTS := -Wall -Wextra
C_FLAGS := $(C_WARNINGS) -std=c11

C_FLAGS_TESTS := $(C_WARNINGS_TESTS) -std=c11
C_LIBS := -lncurses -lm
BUILD_DIR := ./build
TEST_DIR := ./tests
SRC_DIR := src
# name of the executable
OUTPUT := cellulose
TEST_OUTPUT := cellulose_test

# find all of the c files
C_FILES := $(shell find $(SRC_DIR) -not -name "main.c" -and -name '*.c' )

# append .o to all of the files
OBJ_FILES := $(C_FILES:%=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/$(OUTPUT)

test: $(TEST_DIR)/$(TEST_OUTPUT)

$(TEST_DIR)/$(TEST_OUTPUT): $(OBJ_FILES) tests/test.c
	$(CC) $(C_FLAGS_TESTS) -o $@ $^ $(C_LIBS) -g

$(BUILD_DIR)/$(OUTPUT): $(OBJ_FILES) src/main.c
	$(CC) $(C_FLAGS) -o $@ $^ $(C_LIBS) -g

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< -o $@ -g

clean:
	rm -r $(BUILD_DIR)/$(SRC_DIR)

