PROJ_NAME = WastelandAdventures

# Include and Library paths
INCLUDE_PATHS = \
    ../vendor/raylib5/include \
    ../vendor/cjson/include \
	../vendor/fmt/include

LIBRARY_PATHS = \
    ../vendor/raylib5/lib \
    ../vendor/cjson/lib

# Libraries to link
LIBRARIES = \
    -lraylib \
    -lcjson \
    -lgdi32 \
    -lwinmm \
	-lWs2_32

# Directories
SRCDIR = ../src
BIN_INT = ../bin-int
BIN = ../bin
TARGET = $(BIN)/$(PROJ_NAME).exe

# Flags
CFLAGS = -O2 -g -s -fno-asynchronous-unwind-tables $(foreach path, $(INCLUDE_PATHS), -I$(path)) -std=gnu99
LDFLAGS = $(foreach path, $(LIBRARY_PATHS), -L$(path)) $(LIBRARIES)

# Source files in ROOT/src/ and ROOT/tests/
SRC = $(wildcard ../src/*.c) $(wildcard ./*.c)
OBJ = $(patsubst %.c,$(BIN_INT)/%.o,$(notdir $(SRC)))



#############
### RULES ###
#############

# Executables
all : $(TARGET)

run : $(TARGET)
	$(BIN)/$(TARGET)

clean: clean_bin clean_bin_int

# Units
testdray : $(SRCDIR)/memory.h $(SRCDIR)/dray.c $(SRCDIR)/dray.h testdray.c | $(BIN)
	$(CC) $(CFLAGS) testdray.c $(SRCDIR)/dray.c $(SRCDIR)/memory.c -o $(BIN)/testdray $(LDFLAGS)
	$(BIN)/testdray

test_tweens : $(SRCDIR)/flecs.h $(SRCDIR)/flecs.c $(SRCDIR)/tween.c $(SRCDIR)/tween.h test_tweens.c | $(BIN)
	$(CC) $(CFLAGS) test_tweens.c $(SRCDIR)/flecs.h $(SRCDIR)/flecs.c $(SRCDIR)/tween.h $(SRCDIR)/tween.c $(LDFLAGS)
	$(BIN)/test_tweens

clean_bin:
	rm -f $(BIN)/*

clean_bin_int:
	rm -f $(BIN_INT)/*

$(TARGET): $(OBJ) | $(BIN)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(BIN):
	mkdir -p $(BIN)

$(BIN_INT):
	mkdir -p $(BIN_INT)

$(BIN_INT)/%.o: %.c | $(BIN_INT)
	$(CC) $(CFLAGS) -c $< -o $@
