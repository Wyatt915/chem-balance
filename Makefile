CC=gcc
CFLAGS=-Wall
LNFLAGS=
DEPS = *.h
EXEC = balance

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

.PHONY: debug clean

all: CFLAGS += -O2
all: $(EXEC)

debug: CFLAGS += -ggdb -DDEBUG
debug: $(EXEC)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJECTS)
	$(CC) $^ $(LNFLAGS) -o $@

clean:
	rm *.o vgcore.*
