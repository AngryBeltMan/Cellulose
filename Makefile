CC := gcc
C_WARNINGS := -Wall -Wpedantic -Wextra
C_FLAGS := $(C_WARNINGS) -std=c11
C_LIBS := -lncurses -lm
BUILD_DIR := ./build
SRC_DIR := src
# name of the executable
OUTPUT := cellulose

# find all of the c files
C_FILES := $(shell find $(SRC_DIR) -name '*.c' )

# append .o to all of the files
OBJ_FILES := $(C_FILES:%=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/$(OUTPUT)

$(BUILD_DIR)/$(OUTPUT): $(OBJ_FILES)
	$(CC) $(C_FLAGS) -o $@ $^ $(C_LIBS) -g

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< -o $@ -g

clean:
	rm -r $(BUILD_DIR)/$(SRC_DIR)

