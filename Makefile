CC            ?= gcc
SOURCE_DIR    := src
EXTENSION_DIR := extension
OBJECT_DIR    := obj
INCLUDE_DIRS  := $(shell find $(SOURCE_DIR)/assembler -type d) \
	$(shell find $(SOURCE_DIR)/emulator -type d) \
	$(shell find $(SOURCE_DIR)/common -type d) \
	$(shell find $(EXTENSION_DIR) -type d)
INCLUDE_FLAGS := -I~/ncurses/include $(addprefix -I,$(INCLUDE_DIRS))
LDFLAGS       := -L~/ncurses/lib
# No -D_POSIX_SOURCE as that interferes with MAP_ANONYMOUS in <sys/mman.h>!
CFLAGS        ?= -std=c17 -g \
	-Wall -Werror -Wextra --pedantic-errors \
	-D_DEFAULT_SOURCE $(INCLUDE_FLAGS)

.PHONY: help all setup test testEmulate testAssemble report cleanReport cleanObject clean

# Find all source files
COMMON_SOURCES    := $(wildcard $(SOURCE_DIR)/common/*.c)

EMULATOR_SOURCES  := $(shell find $(SOURCE_DIR)/emulator/ -name '*.c')
EMULATOR_MAIN     := $(wildcard $(SOURCE_DIR)/emulate.c)

ASSEMBLER_SOURCES := $(shell find $(SOURCE_DIR)/assembler/ -name '*.c')
ASSEMBLER_MAIN    := $(wildcard $(SOURCE_DIR)/assemble.c)

GRIM_SOURCES      := $(shell find $(EXTENSION_DIR)/ -name '*.c')

# Object files list
COMMON_OBJECTS    := $(patsubst $(SOURCE_DIR)/%.c, $(OBJECT_DIR)/%.o, $(COMMON_SOURCES))
EMULATOR_OBJECTS  := $(patsubst $(SOURCE_DIR)/%.c, $(OBJECT_DIR)/%.o, $(EMULATOR_SOURCES))
ASSEMBLER_OBJECTS := $(patsubst $(SOURCE_DIR)/%.c, $(OBJECT_DIR)/%.o, $(ASSEMBLER_SOURCES))
GRIM_OBJECTS      := $(patsubst $(EXTENSION_DIR)/%.c, $(OBJECT_DIR)/%.o, $(GRIM_SOURCES))

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
help:                                                                             ## Show this help.
	@egrep -h '\s##\s' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m  %-15s\033[0m %s\n", $$1, $$2}'

all: assemble emulate grim cleanObject                                            ## Compile all programs and clean object files.

setup:                                                                            ## Setup build, test, and report compilation environment.
	@echo "=== Setting Up Submodules ==="
	@git submodule init
	@git submodule update --recursive
	@echo "=== Building ncurses Submodule ==="
	# There are easier platform specific ways of doing this, but we
	# opted for a full "manual" install to achieve maximum portability.
	# ATTENTION: Modifies ~/local.
	cd ncurses \
		&& ./configure --prefix $(shell eval echo ~$(USER))/ncurses \
			--disable-root-access --without-cxx-binding --without-manpages --without-progs \
			--without-tests --without-cxx --without-ada \
    		&& make \
    		&& make install
	@echo "=== Setting Up Testsuite ==="
	@cd testsuite && ./install
	@echo "=== Sym-linking Binaries to Testsuite ==="
	@mkdir -p testsuite/solution
	@cd testsuite/solution && ln -sf ../../assemble ./assemble
	@cd testsuite/solution && ln -sf ../../emulate ./emulate

test: all                                                                        ## Run all tests.
	@cd testsuite && ./run -p

testEmulate: emulate                                                             ## Run emulator tests.
	@cd testsuite && ./run -Ep

testAssemble: assemble                                                           ## Run assembler tests.
	@cd testsuite && ./run -Ap

emulate: $(COMMON_OBJECTS) $(EMULATOR_OBJECTS) $(EMULATOR_MAIN)                  ## Compile the emulator.
	$(CC) $(CFLAGS) -o $@ $^

assemble: $(COMMON_OBJECTS) $(ASSEMBLER_OBJECTS) $(ASSEMBLER_MAIN)               ## Compile the assembler.
	$(CC) $(CFLAGS) -o $@ $^

grim: $(COMMON_OBJECTS) $(EMULATOR_OBJECTS) $(ASSEMBLER_OBJECTS) $(GRIM_OBJECTS) ## Compile GRim. (The extension)
	$(CC) $(CFLAGS) -o $@ $^ -lncurses

# Compile rules for all .c files
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECT_DIR)/%.o: $(EXTENSION_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

report:                                                                         ## Generates checkpoint and final reports.
	cd $(REPORT_DIR) && latexmk --xelatex --shell-escape $(REPORT).tex
	cd $(REPORT_DIR) && latexmk --xelatex --shell-escape $(CHECKPOINT).tex

cleanReport:                                                                    ## Clean report generation files.
	$(RM) -r $(TO_CLEAN) $(REPORT_DIR)/_minted-checkpoint
	$(RM) $(REPORT_DIR)/$(REPORT).pdf
	$(RM) $(REPORT_DIR)/$(CHECKPOINT).pdf

cleanObject:                                                                    ## Clean all object files.
	$(RM) -r $(OBJECT_DIR)

clean: cleanObject                                                              ## Clean executables and object files.
	$(RM) emulate assemble grim
