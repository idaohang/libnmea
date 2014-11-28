# Creates a library called libnmea in the root directory. Source files for the
# library can be found in the "lib" directory.

# This defines the symbol used to indicate a function is linked to be externally
# visible.
LIBNMEA_PUBLIC="__attribute__ ((visibility (\"default\")))"

# gcc works just as well.
CC=clang
CFLAGS=-fPIC -Wall -O2 -g -fvisibility=hidden -D LIBNMEA_PUBLIC=$(LIBNMEA_PUBLIC)

# Ensure that we are linked as a shared library.
LDFLAGS=-shared
RM=rm -f
TARGET_LIB=libnmea.so

# Source files to be compiled into the library.
SRCS=lib/libnmea.c lib/parsing.c
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
