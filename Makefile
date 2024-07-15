EXEC = balance

VPATH = src
CC = gcc
CFLAGS = -Wall
LNFLAGS =
DEPS = $(wildcard $(VPATH)/*.h)

BUILDDIR=build

SOURCES = $(wildcard $(VPATH)/*.c)
OBJECTS = $(patsubst $(VPATH)/%.c,$(BUILDDIR)/%.o,$(SOURCES))

WASM_DIR = wasm
WASM_EXEC = $(WASM_DIR)/$(EXEC).html

.PHONY: all debug wasm clean

all: CFLAGS += -O2
all: $(BUILDDIR) $(EXEC)

debug: CFLAGS += -Og -ggdb -DDEBUG
debug: $(BUILDDIR) $(EXEC)

wasm: CC=emcc
wasm: $(WASM_EXEC)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: %.c $(DEPS) | $(BUILDDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJECTS)
	$(CC) $^ $(LNFLAGS) -o $@

$(WASM_EXEC): $(OBJECTS)
	emcc $(OBJECTS) -o $(WASM_EXEC) --shell-file /home/cws/programs/emsdk/upstream/emscripten/src/shell_minimal.html

clean:
	rm -f $(BUILDDIR)/*.o vgcore.* $(WASM_DIR)/*
	rmdir $(BUILDDIR)
