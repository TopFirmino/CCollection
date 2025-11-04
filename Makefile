# ------- Config -------
CC      ?= gcc
CFLAGS  ?= -std=c99 -O2 -Wall -Wextra -Wpedantic
INC     := -Isrc -Iexternal/Unity/src -Itests -Iinclude
LDFLAGS ?=
BUILD   ?= build

# Sources
APP_SRC := $(wildcard src/*.c)
APP_OBJ := $(patsubst src/%.c,$(BUILD)/src/%.o,$(APP_SRC))

# Unity (single-file)
UNITY_SRC := external/Unity/src/unity.c
UNITY_OBJ := $(BUILD)/unity.o

# Test
TEST_SRC := $(wildcard tests/*.c)
TEST_OBJ := $(patsubst tests/%.c,$(BUILD)/tests/%.o,$(TEST_SRC))
TEST_BIN := $(BUILD)/tests_runner

# ------- Targets -------
.PHONY: all test clean distclean run

all: $(TEST_BIN)

test: $(TEST_BIN)
	@echo ">>> Running tests"
	$(TEST_BIN)

run: test

$(TEST_BIN): $(APP_OBJ) $(TEST_OBJ) $(UNITY_OBJ) | $(BUILD)
	@echo ">>> Linking $@"
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# App objects
$(BUILD)/src/%.o: src/%.c | $(BUILD)/src
	@echo ">>> Compiling $<"
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# Test objects
$(BUILD)/tests/%.o: tests/%.c | $(BUILD)/tests
	@echo ">>> Compiling $<"
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# Unity object
$(BUILD)/unity.o: $(UNITY_SRC) | $(BUILD)
	@echo ">>> Compiling Unity"
	$(CC) $(CFLAGS) $(INC) -c $(UNITY_SRC) -o $@

# Dirs
$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/src:
	mkdir -p $(BUILD)/src

$(BUILD)/tests:
	mkdir -p $(BUILD)/tests

clean:
	rm -rf $(BUILD)/*.o $(BUILD)/src $(BUILD)/tests $(TEST_BIN) $(BUILD)/unity.o 2>/dev/null || true

distclean: clean
	rm -rf $(BUILD)
