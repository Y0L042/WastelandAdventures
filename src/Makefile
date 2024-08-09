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
BIN_INT = ../bin-int
BIN = ../bin
TARGET = $(BIN)/$(PROJ_NAME).exe

# Flags
CFLAGS = -O2 -g -s -fno-asynchronous-unwind-tables $(foreach path, $(INCLUDE_PATHS), -I$(path)) -std=gnu99
LDFLAGS = $(foreach path, $(LIBRARY_PATHS), -L$(path)) $(LIBRARIES)

SRC = $(wildcard ./*.c)
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
testdray : memory.h dray.c dray.h testdray.c | $(BIN)
	$(CC) $(CFLAGS) testdray.c dray.c memory.c -o $(BIN)/testdray $(LDFLAGS)
	$(BIN)/testdray

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
