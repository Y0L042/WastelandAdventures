# Include and library paths
INCLUDE_PATHS = \
    ./vendor/raylib5/include \
    ./vendor/cjson/include \
	./vendor/fmt/include

LIBRARY_PATHS = \
    ./vendor/raylib5/lib/$(PLATFORM) \
    ./vendor/cjson/lib/$(PLATFORM)

# Platform detection
OS := $(shell uname)
ifeq ($(OS), Linux)
    TARGET := bin/main
    PLATFORM := linux
    LIBRARIES := -lraylib -lcjson -lGL -lm -lpthread -ldl -lrt -lX11
    CFLAGS_BASE := -O2 -g -s -std=gnu99 $(foreach path, $(INCLUDE_PATHS), -I$(path))
    LDFLAGS := $(foreach path, $(LIBRARY_PATHS), -L$(path)) $(LIBRARIES)
else
    TARGET := bin/main.exe
	PLATFORM := windows
    LIBRARIES := -lraylib -lcjson -lgdi32 -lwinmm -lWs2_32
    CFLAGS_BASE := -O2 -g -s -fno-asynchronous-unwind-tables $(foreach path, $(INCLUDE_PATHS), -I$(path)) -std=gnu99
    LDFLAGS := $(foreach path, $(LIBRARY_PATHS), -L$(path)) $(LIBRARIES)
endif

SRC_C = $(wildcard ./src/*.c)
OBJ_C = $(patsubst ./src/%.c, ./bin-int/%.o, $(SRC_C))
OBJ = $(OBJ_C)

OBJCONV = objconv

# Additional CFLAGS based on target
CFLAGS_RUN := $(CFLAGS_BASE) -DDEBUG
CFLAGS_RELEASE := $(CFLAGS_BASE) -DRELEASE

# Targets
all: CFLAGS=$(CFLAGS_RUN)
all: $(TARGET)  # Main target for normal C build

run: CFLAGS=$(CFLAGS_RUN)
run: $(TARGET)  # Target to run the C executable
	./$(TARGET)

release: CFLAGS=$(CFLAGS_RELEASE)
release: $(TARGET)

clean: clean_bin clean_bin_int clean_asm  # Target to clean all build directories

clean_all: clean_bin clean_bin_int_all clean_asm  # Target to clean all build directories
# Build and run C program normally
$(TARGET): $(OBJ)
	mkdir -p bin
	gcc $(OBJ) -o $@ $(LDFLAGS)

# Compile C source files to object files
./bin-int/%.o: ./src/%.c
	mkdir -p bin-int
	gcc $(CFLAGS) -c $< -o $@

# Disassemble object files to NASM assembly
disassemble: $(OBJ_C)
	mkdir -p asm/nasm
	for obj in $^; do \
		outfile=asm/nasm/`basename $$obj .o`.asm; \
		$(OBJCONV) -fnasm $$obj -o $$outfile; \
		./cleanup_asm.sh $$outfile; \
	done

# Release target
release: $(TARGET)
	@date=`date +%Y%m%d_%H%M`; \
	release_dir="builds/build_$$date"; \
	mkdir -p $$release_dir; \
	cp $(TARGET) $$release_dir/; \
	cp -r assets $$release_dir/; \
	git log -1 > $$release_dir/git_commit_info.txt; \
	if [ $$? -eq 0 ]; then \
		echo "Release created successfully in $$release_dir"; \
	else \
		echo "Failed to create release"; \
	fi


# Phony targets
.PHONY: clean all run asm_build asm_run clean_asm clean_bin clean_bin_int clean_asm_bin_int copy_asm disassemble

clean_bin:
	rm -rf bin

clean_bin_int:
	find bin-int -mindepth 1 ! -name 'flecs.o' ! -name 'log.o' ! -name 'mt19937-64.o' -exec rm -rf {} +

clean_bin_int_all:
	rm -rf bin-int

clean_asm:
	rm -rf asm

# Rebuild everything
rebuild: clean all
