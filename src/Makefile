CC       ?= gcc
CFLAGS   ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic
CXXFLAGS ?= -Iemulator/instructions/ -Iemulator/io/ -Iemulator/system
VPATH    ?= emulator/instructions/:emulator/io/:emulator/system

.SUFFIXES: .c .o
.PHONY: all clean

EMULATE_OBJS = emulate.o registers.o output.o

all: assemble emulate

assemble: assemble.o

emulate: $(EMULATE_OBJS)
	$(CC) $(CFLAGS) -o $@ $(EMULATE_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o assemble emulate
	
