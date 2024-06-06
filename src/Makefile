CC       ?= gcc
# No -D_POSIX_SOURCE as that interferes with MAP_ANONYMOUS in <sys/mman.h>!
CFLAGS   ?= -std=c17 -g\
	-D_DEFAULT_SOURCE\
	-Wall -Werror -Wextra -Wpedantic
CXXFLAGS ?= -Icommon/ -Icommon/ir\
 	-Iemulator/decoders/ -Iemulator/executors/ -Iemulator/executors/immediate -Iemulator/executors/register\
 	-Iemulator/system -Iemulator\
 	-Iassembler/ -Iassembler/handlers/

SRC_DIR = .
OBJ_DIR = obj

.SUFFIXES: .c .o
.PHONY: all clean

# Find all source files
EMULATOR_SRC_FILES = $(wildcard $(SRC_DIR)/emulator/*.c) \
	$(wildcard $(SRC_DIR)/emulator/decoders/*.c) \
	$(wildcard $(SRC_DIR)/emulator/executors/*.c) \
	$(wildcard $(SRC_DIR)/emulator/executors/immediate/*.c) \
	$(wildcard $(SRC_DIR)/emulator/executors/register/*.c) \
	$(wildcard $(SRC_DIR)/emulator/system/*.c) \
	$(wildcard $(SRC_DIR)/common/*.c) \
	$(wildcard $(SRC_DIR)/emulate.c)

ASSEMBLER_SRC_FILES = $(shell find $(SRC_DIR)/assembler/ -name '*.c') \
	$(wildcard $(SRC_DIR)/common/*.c) \
	$(wildcard $(SRC_DIR)/assemble.c)

# Object files list
EMULATOR_OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(EMULATOR_SRC_FILES))
ASSEMBLER_OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(ASSEMBLER_SRC_FILES))

# Targets
EMULATOR_TARGET = emulate
ASSEMBLER_TARGET = assemble

all: assemble emulate

# Link emulator target
emulate: $(EMULATOR_OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Link assembler target
assemble: $(ASSEMBLER_OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Compile rules for all .c files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR) $(BIN_DIR) emulate assemble
