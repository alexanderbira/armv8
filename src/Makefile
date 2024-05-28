CC       ?= gcc
# No -D_POSIX_SOURCE as that interferes with MAP_ANONYMOUS in <sys/mman.h>!
CFLAGS   ?= -std=c17 -g\
	-D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic
CXXFLAGS ?= -Icommon/ -Iemulator/instructions/ -Iemulator/io/ -Iemulator/system
VPATH    ?= common/:emulator/instructions/:emulator/io/:emulator/system

.SUFFIXES: .c .o
.PHONY: all clean

EMULATE_OBJS = emulate.o registers.o memory.o output.o

all: assemble emulate

assemble: assemble.o

emulate: $(EMULATE_OBJS)
	$(CC) $(CFLAGS) -o $@ $(EMULATE_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o assemble emulate
	
