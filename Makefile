CC            ?= gcc
SOURCE_DIR    := src
OBJECT_DIR    := obj
INCLUDE_DIRS  := $(shell find $(SOURCE_DIR)/assembler -type d) \
	$(shell find $(SOURCE_DIR)/emulator -type d) \
	$(shell find $(SOURCE_DIR)/common -type d)
INCLUDE_FLAGS := $(addprefix -I,$(INCLUDE_DIRS))
# No -D_POSIX_SOURCE as that interferes with MAP_ANONYMOUS in <sys/mman.h>!
CFLAGS        ?= -std=c17 -g \
	-Wall -Werror -Wextra --pedantic-errors \
	-D_DEFAULT_SOURCE $(INCLUDE_FLAGS) \

.PHONY: all cleanObject clean

# Find all source files
EMULATOR_SOURCES = $(shell find $(SOURCE_DIR)/emulator/ -name '*.c') \
	$(wildcard $(SOURCE_DIR)/common/*.c) \
	$(wildcard $(SOURCE_DIR)/emulate.c)

ASSEMBLER_SOURCES = $(shell find $(SOURCE_DIR)/assembler/ -name '*.c') \
	$(wildcard $(SOURCE_DIR)/common/*.c) \
	$(wildcard $(SOURCE_DIR)/assemble.c)

# Object files list
EMULATOR_OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(OBJECT_DIR)/%.o, $(EMULATOR_SOURCES))
ASSEMBLER_OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(OBJECT_DIR)/%.o, $(ASSEMBLER_SOURCES))

# Self documentation command modified from
# https://stackoverflow.com/a/64996042/16731239
help:                                       ## Show this help.
	@egrep -h '\s##\s' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m  %-15s\033[0m %s\n", $$1, $$2}'

all: assemble emulate cleanObject           ## Compile all programs and clean object files.

setupTest:                                  ## Setup test environment.
	git submodule update --recursive
	cd testsuite && ./install
	mkdir -p testsuite/solution
	pwd
	cd testsuite/solution && ln -s ../../assemble ./assemble
	cd testsuite/solution && ln -s ../../emulate ./emulate

test: all                                   ## Run all tests.
	cd testsuite && ./run -p

testEmulate: emulate                        ## Run emulator tests.
	cd testsuite && ./run -Ep

testAssemble: assemble                      ## Run assembler tests.
	cd testsuite && ./run -Ap

emulate: $(EMULATOR_OBJECTS)                ## Compile the emulator.
	$(CC) $(CFLAGS) -o $@ $^

assemble: $(ASSEMBLER_OBJECTS)              ## Compile the assembler.
	$(CC) $(CFLAGS) -o $@ $^

# Compile rules for all .c files
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $< -o $@

cleanObject:                                ## Clean all object files.
	$(RM) -r $(OBJECT_DIR)

clean: cleanObject                          ## Clean executables and object files.
	$(RM) emulate assemble
