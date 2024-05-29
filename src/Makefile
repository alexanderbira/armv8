CC       ?= gcc
# No -D_POSIX_SOURCE as that interferes with MAP_ANONYMOUS in <sys/mman.h>!
CFLAGS   ?= -std=c17 -g\
	-D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic
CXXFLAGS ?= -Icommon/ -Icommon/ir/ -Iemulator/instructions/ -Iemulator/io/ -Iemulator/system -Iassembler/insts -Iassembler/
VPATH    ?= common/:common/ir/:emulator/instructions/:emulator/io/:emulator/system:assembler/insts/:assembler/

.SUFFIXES: .c .o
.PHONY: all clean

COMMON_OBJS = helpers.o

EMULATE_OBJS = emulate.o registers.o memory.o output.o

ASSEMBLE_OBJS = classify.o proc.o immAssem.o regAssem.o sdtAssem.o

all: assemble emulate

assemble: $(COMMON_OBJS) $(ASSEMBLE_OBJS)

emulate: $(COMMON_OBJS) $(EMULATE_OBJS)
	$(CC) $(CFLAGS) -o $@ $(EMULATE_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o assemble emulate
	
