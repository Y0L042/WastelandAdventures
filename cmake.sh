# #!/bin/bash
#
# export CC=gcc
# export CCX=g++
#
# # Create and configure the build directory
# cmake -B build -G Ninja 
#
# # Build the project
# cmake --build build
#
# cp ./build/compile_commands.json ./compile_commands.json
#
# echo "Running executable:"
# echo ""
# # Run the executable
# ./bin/main.exe
# # cmake --build build --target run



#!/bin/bash

export CC=gcc
export CCX=g++

# Function to build the project
build() {
	echo "Building..."
    # Create and configure the build directory
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug

    # Check if the CMake configuration was successful
    if [ $? -ne 0 ]; then
        echo "CMake configuration failed."
        exit 1
    fi

    # Build the project
    cmake --build build -- -v

    # Check if the build was successful
    if [ $? -ne 0 ]; then
        echo "Build failed."
        exit 1
    fi

    # Copy the compile_commands.json file
	echo "Copying compile_commands.json..."
    cp ./build/compile_commands.json ./compile_commands.json
	echo "Building finished."
}

# Function to run the executable
run() {
	build
    echo "Running executable:"
    echo ""
    # Run the executable
    ./bin/main.exe
}

# Function to run the executable under GDB in Debug mode
run_debug() {
    build
    echo "Running executable under GDB (Debug mode):"
    echo ""
    # Run the executable under GDB
    gdb -ex run bin/main
}

# Function to clean the build
clean() {
    echo "Cleaning build directory..."
    rm -rf build
	rm -rf bin
	rm -rf bin-int
    rm -f compile_commands.json
	echo "Cleaning complete."
}

# Main logic to handle different commands
case "$1" in
    buildonly)
        build
        ;;
    run)
        run
        ;;
	rungdb)
		run_debug
		;;
    clean)
        clean
        ;;
    release)
        echo "Building in Release mode..."
        # Invoke the 'release' custom target defined in CMakeLists.txt
        cmake --build build --target release
        ;;
    *)
        echo "Usage: $0 {buildonly|run|rungdb|clean|release}"
        exit 1
        ;;
esac
