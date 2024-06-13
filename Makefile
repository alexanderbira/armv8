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
	-I

.PHONY: all cleanObject cleanReport clean setup test testEmulate testAssemble help

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

# Report stuff
REPORT_DIR = doc
CHECKPOINT = checkpoint
REPORT = report

TO_CLEAN = $(wildcard $(REPORT_DIR)/*.aux) \
	$(wildcard $(REPORT_DIR)/*.fdb_latexmk) \
	$(wildcard $(REPORT_DIR)/*.fls) \
	$(wildcard $(REPORT_DIR)/*.listing) \
	$(wildcard $(REPORT_DIR)/*.log) \
	$(wildcard $(REPORT_DIR)/*.xdv) \

# Self documentation command modified from
# https://stackoverflow.com/a/64996042/16731239
help:                                       ## Show this help.
	@egrep -h '\s##\s' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m  %-15s\033[0m %s\n", $$1, $$2}'

all: assemble emulate cleanObject           ## Compile all programs and clean object files.

setup:                                      ## Setup test and report compilation environment.
	git submodule update --recursive
	cd testsuite && ./install
	mkdir -p testsuite/solution
	pwd
	cd testsuite/solution && ln -sf ../../assemble ./assemble
	cd testsuite/solution && ln -sf ../../emulate ./emulate

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

report:                                     ## Generates checkpoint and final reports.
	cd $(REPORT_DIR) && latexmk --xelatex --shell-escape $(REPORT).tex
	cd $(REPORT_DIR) && latexmk --xelatex --shell-escape $(CHECKPOINT).tex

cleanReport:                                ## Clean report generation files.
	$(RM) -r $(TO_CLEAN) $(REPORT_DIR)/_minted-checkpoint
	$(RM) $(REPORT_DIR)/$(REPORT).pdf
	$(RM) $(REPORT_DIR)/$(CHECKPOINT).pdf

cleanObject:                                ## Clean all object files.
	$(RM) -r $(OBJECT_DIR)

clean: cleanObject cleanReport              ## Clean executables, object files, and LaTeX files.
	$(RM) emulate assemble
