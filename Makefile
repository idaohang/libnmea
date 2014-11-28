# Creates a library called libnmea in the root directory. Source files for the
# library can be found in the "lib" directory.

# gcc works just as well.
CC=clang
CFLAGS=-fPIC -Wall -Wextra -O2 -g

# Ensure that we are linked as a shared library.
LDFLAGS=-shared
RM=rm -f
TARGET_LIB=libnmea.so

# Source files to be compiled into the library.
SRCS=lib/libnmea.c
OBJS=$(SRCS:.c=.o)

.PHONY: all
all: ${TARGET_LIB}

# Links the objects together into a shared library.
$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

# Compiles the source files.
$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.c=.d)

# Cleans up the target library, intermediate objects, and debug informations.
.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)
