# Include and library paths
INCLUDE_PATHS = \
    ./vendor/raylib5/include \
    ./vendor/cjson/include \
	./vendor/fmt/include

LIBRARY_PATHS = \
    ./vendor/raylib5/lib \
    ./vendor/cjson/lib

# Libraries to link
LIBRARIES = \
    -lraylib \
    -lcjson \
    -lgdi32 \
    -lwinmm \
	-lWs2_32

# Exec target
TARGET = bin/main.exe
ASM_TARGET = asm/bin/main.exe

# Flags
CFLAGS = -O2 -g -s -fno-asynchronous-unwind-tables $(foreach path, $(INCLUDE_PATHS), -I$(path)) -std=gnu99
LDFLAGS = $(foreach path, $(LIBRARY_PATHS), -L$(path)) $(LIBRARIES)

SRC_C = $(wildcard ./src/*.c)
SRC_ASM = $(wildcard ./src/*.asm)
OBJ_C = $(patsubst ./src/%.c, ./bin-int/%.o, $(SRC_C))
OBJ_ASM = $(patsubst ./src/%.asm, ./bin-int/%.o, $(SRC_ASM))
OBJ = $(OBJ_C) $(OBJ_ASM)

OBJCONV = objconv

# Targets
all: $(TARGET)  # Main target for normal C build

asm_build: copy_asm $(ASM_TARGET)  # Target for building NASM-based executable

asm_run: asm_build  # Target to build and run NASM-based executable
	./$(ASM_TARGET)

run: $(TARGET)  # Target to run the C executable
	./$(TARGET)

clean: clean_bin clean_bin_int clean_asm  # Target to clean all build directories

# Build and run C program normally
$(TARGET): $(OBJ)
	mkdir -p bin
	gcc $(OBJ) -o $@ $(LDFLAGS)

# Compile C source files to object files
./bin-int/%.o: ./src/%.c
	mkdir -p bin-int
	gcc $(CFLAGS) -c $< -o $@

# Assemble ASM source files to object files
./bin-int/%.o: ./src/%.asm
	mkdir -p bin-int
	nasm -f win64 $< -o $@

# Copy ASM files from ./src to ./asm/nasm
copy_asm: $(SRC_ASM)
	mkdir -p asm/nasm
	cp $^ asm/nasm/
	@echo "Copied $^ to asm/nasm/"

# Disassemble object files to NASM assembly
disassemble: $(OBJ_C)
	mkdir -p asm/nasm
	for obj in $^; do \
		outfile=asm/nasm/`basename $$obj .o`.asm; \
		$(OBJCONV) -fnasm $$obj -o $$outfile; \
		./cleanup_asm.sh $$outfile; \
	done

# Compile NASM assembly files to object files
$(ASM_OBJ): ./asm/nasm/%.asm
	mkdir -p asm/bin-int
	nasm -f win64 $< -o $@

# Link NASM object files to create executable
$(ASM_TARGET):  $(ASM_OBJ)
	mkdir -p asm/bin
	gcc $(ASM_OBJ) -o $@ $(LDFLAGS)

# Phony targets
.PHONY: clean all run asm_build asm_run clean_asm clean_bin clean_bin_int clean_asm_bin_int copy_asm disassemble

clean_bin:
	rm -rf bin

clean_bin_int:
	rm -rf bin-int

clean_asm:
	rm -rf asm

# Rebuild everything
rebuild: clean all
