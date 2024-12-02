# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -g

# Libraries for Windows
ifeq ($(OS),Windows_NT)
    LIBS = -lws2_32
else
    LIBS = 
endif

# Source Files and Header
COMMON_SRC = common.c
COMMON_HDR = common.h

# Targets
TARGETS = index server

# Pattern Rule for Targets
%: %.c $(COMMON_SRC) $(COMMON_HDR)
	$(CC) $(CFLAGS) $< $(COMMON_SRC) -o $@ $(LIBS)

# Clean Rule
clean:
	rm -f $(TARGETS)